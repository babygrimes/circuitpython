// Copyright (c) 2019 David Grimes <david@grimes-family.com>

#include "shared-bindings/nighttrain/__init__.h"
#include "font5x7.h"

#include <py/mpconfig.h>
#include <math.h>

#define STD(i, n) \
{ \
	i %= (int32_t)n; \
	if (i < 0) { \
		i += n; \
	} \
}

#define INC(i, n) \
{ \
	if (++i >= (int32_t)n) {		\
		i = 0; \
	} \
}

void nighttrain_initialize(void)
{
#ifdef SAMD51
	*((volatile uint32_t *)0xe000edfc) |= 0x01000000; // DEMCT_TRCENA
	*((volatile uint32_t *)0xe0001000) |= 0x00000001; // CYCCNTNA
#endif
}

mp_float_t nighttrain_altitude_from_baro(mp_float_t p, mp_float_t t)
{
	// (pow(101325.0/p, 0.190223) - 1.0) * (t+273.15) * 504.74662
	return (MICROPY_FLOAT_C_FUN(pow)(MICROPY_FLOAT_CONST(101325.0)/p, MICROPY_FLOAT_CONST(0.190223)) - MICROPY_FLOAT_CONST(1.0)) * (t+MICROPY_FLOAT_CONST(273.15)) * MICROPY_FLOAT_CONST(504.74662);
}

void nighttrain_sk6812_render(uint8_t y, uint32_t *led, size_t n, uint8_t *g, size_t gn, int32_t gi, uint32_t *c, size_t cn, int32_t ci)
{
	STD(gi, gn);
	STD(ci, cn);

	for (size_t i=0; i < n; i++) {
		uint32_t Y = ((uint32_t)g[gi] * (uint32_t)y) >> 8;
		uint32_t C = c[ci];

		// GRB
		led[i] = (
			( ((C & 0x0000ff00) * Y) & 0x00ff0000) |
			((((C & 0x00ff0000) * Y) & 0xff000000) >> 16) |
			( ((C & 0x000000ff) * Y) >> 8)
			);

		INC(gi, gn);
		INC(ci, cn);
	}
}

void nighttrain_sk9822_render_hardware_gamma(uint8_t y, uint32_t *led, size_t n, uint8_t *g, size_t gn, int32_t gi, uint32_t *c, size_t cn, int32_t ci)
{
	STD(gi, gn);
	STD(ci, cn);

	for (size_t i=0; i < n; i++) {
		uint32_t Y = ((uint32_t)g[gi] * (uint32_t)y) >> 8;
		uint32_t C = c[ci];

		// YBGR
		led[i] = (
			0xe0000000 |
			((Y & 0x000000f8) << 21) |
			((C & 0x000000ff) << 16) |
			((C & 0x0000ff00)) |
			((C & 0x00ff0000) >> 16)
			);

		INC(gi, gn);
		INC(ci, cn);
	}
}

void nighttrain_sk9822_render_software_gamma(uint8_t y, uint32_t *led, size_t n, uint8_t *g, size_t gn, int32_t gi, uint32_t *c, size_t cn, int32_t ci)
{
	uint32_t B = (0xe0000000 | (((uint32_t)y & 0x000000f8) << 21));

	STD(gi, gn);
	STD(ci, cn);

	for (size_t i=0; i < n; i++) {
		uint32_t Y = g[gi];
		uint32_t C = c[ci];

		// YBGR
		led[i] = (
			B |
			((((C & 0x000000ff) * Y) & 0x0000ff00) << 8) |
			((((C & 0x0000ff00) * Y) & 0x00ff0000) >> 8) |
			( ((C & 0x00ff0000) * Y) >> 24)
			);

		INC(gi, gn);
		INC(ci, cn);
	}
}

void nighttrain_skxxxx_reverse(uint32_t *led, size_t n)
{
	uint32_t *p=led, *e=(led + (n - 1));

	while (p < e) {
		uint32_t x=*p;

		*p++ = *e;
		*e-- = x;
	}
}

static inline uint32_t __skxxxx_scale_color(uint32_t c, uint8_t f)
{
	return ((((c & 0x00ff0000) * f) & 0xff000000) |
		(((c & 0x0000ff00) * f) & 0x00ff0000) |
		(((c & 0x000000ff) * f) & 0x0000ff00)) >> 8;
}

static inline uint32_t __sk9822_scale_gamma(uint32_t g, uint8_t f)
{
	return ((((g & 0x1f000000) >> 8) * f) & 0x1f000000);
}

void nighttrain_sk6812_blend(uint32_t *dst, uint32_t *src, size_t n, uint8_t b)
{
	while (n-- > 0) {
		*dst = __skxxxx_scale_color(*dst, b) + __skxxxx_scale_color(*src, (255 - b));
		dst++;
		src++;
	}
}

void nighttrain_sk9822_blend_hardware_gamma(uint32_t *dst, uint32_t *src, size_t n, uint8_t b)
{
	while (n-- > 0) {
		uint32_t v;

		v = (0xe0000000 |
		     (__sk9822_scale_gamma(*dst, b) + __sk9822_scale_gamma(*src, (255 - b))) |
		     (__skxxxx_scale_color(*dst, b) + __skxxxx_scale_color(*src, (255 - b))));

		*dst++ = v;
		src++;
	}
}

void nighttrain_sk9822_blend_software_gamma(uint32_t *dst, uint32_t *src, size_t n, uint8_t b)
{
	while (n-- > 0) {
		uint32_t v;

		v = (0xff000000 |
		     (__skxxxx_scale_color(*dst, b) + __skxxxx_scale_color(*src, (255 - b))));

		*dst++ = v;
		src++;
	}
}

void nighttrain_print(
	uint8_t y,
	uint8_t p,
	uint8_t *t,
	size_t n,
	uint8_t *s0,
	uint8_t *s1,
	uint8_t *s2,
	uint8_t *s3,
	uint8_t *s4)
{
	for (size_t i=0; i < n; i++) {
		const uint8_t *ch = &font5x7[(*t++) << 3];

		for (size_t j=0; j < 7; j++) {
			uint8_t bm = *ch++;

			*s0++ = (bm & 0x10) ? y : 0;
			*s1++ = (bm & 0x08) ? y : 0;
			*s2++ = (bm & 0x04) ? y : 0;
			*s3++ = (bm & 0x02) ? y : 0;
			*s4++ = (bm & 0x01) ? y : 0;
		}

		s0 += p;
		s1 += p;
		s2 += p;
		s3 += p;
		s4 += p;
	}
}

static inline void __termio_str(uint8_t *p, uint8_t ch)
{
	uint8_t v;

	v = ch / 100;
	*p++ = v + (uint8_t)'0';
	ch -= v * 100;

	v = ch / 10;
	*p++ = v + (uint8_t)'0';
	ch -= v * 10;

	*p = ch + (uint8_t)'0';
}

static inline uint8_t __termio_ord(uint8_t *p)
{
	uint8_t ch=0;

	ch += (*p++ - (uint8_t)'0') * 100;
	ch += (*p++ - (uint8_t)'0') * 10;
	ch += (*p - (uint8_t)'0');

	return ch;
}

void nighttrain_termio_render(uint8_t *buf, size_t n, uint8_t *g, size_t gn, int32_t gi, uint32_t *c, size_t cn, int32_t ci)
{
	STD(gi, gn);
	STD(ci, cn);

	for (size_t i=0; i < n; i++) {
		uint8_t *p = buf + (i*20);
		uint32_t Y = g[gi];
		uint32_t C = c[ci];

		__termio_str((p +  7), (uint8_t)(((C & 0x00ff0000) * Y) >> 24)); // [7:10]
		__termio_str((p + 11), (uint8_t)(((C & 0x0000ff00) * Y) >> 16)); // [11:14]
		__termio_str((p + 15), (uint8_t)(((C & 0x000000ff) * Y) >> 8));  // [15:18]

		INC(gi, gn);
		INC(ci, cn);
	}
}

void nighttrain_termio_reverse(uint8_t *buf, size_t n)
{
	uint8_t *p=buf, *e=(buf + (20 * (n - 1)));

	while (p < e) {
		for (size_t i=0; i < 20; i++) {
			uint8_t x;

			x = *p;
			*p++ = *e;
			*e++ = x;
		}
		e -= 40;
	}
}

static inline uint8_t __termio_scale_color(uint32_t c, uint8_t f)
{
	return (uint8_t)((c * f) >> 8);
}

void nighttrain_termio_blend(uint8_t *dst, uint8_t *src, size_t n, uint8_t b)
{
	while (n-- > 0) {
		uint8_t v;

		v = __termio_scale_color(__termio_ord(dst +  7), b) + __termio_scale_color(__termio_ord(src +  7), (255 - b));
		__termio_str((dst +  7), v);

		v = __termio_scale_color(__termio_ord(dst + 11), b) + __termio_scale_color(__termio_ord(src + 11), (255 - b));
		__termio_str((dst + 11), v);

		v = __termio_scale_color(__termio_ord(dst + 15), b) + __termio_scale_color(__termio_ord(src + 15), (255 - b));
		__termio_str((dst + 15), v);

		dst += 20;
		src += 20;
	}
}

#ifdef SAMD51
#define PORT0	(0x41008000UL)
#define PORT1	(0x41008080UL)
#undef	PORT
#define PORT	PORT0

#define CYCCNT	(0xe0001004UL)

#define BIT(x)	(1 << x)

#define CLKMASK	BIT(7)	// D2

#define LED0	BIT(18) // D7
#define LED1	BIT(19) // D9
#define LED2	BIT(20) // D10
#define LED3	BIT(21) // D11
#define LED4	BIT(23) // D12
#define LED5	BIT(22) // D13
#define LED6	BIT(14) // D4
#define LED7	BIT(15) // D5

#define PINMASK	(LED0|LED1|LED2|LED3|LED4|LED5|LED6|LED7)

static inline void delay_until(uint32_t n)
{
	while (*((volatile uint32_t *)CYCCNT) < n) {
		// do nothing
	}
}

static inline void delay_mcu(uint32_t n)
{
	if (n > 2) {
		*((volatile uint32_t *)CYCCNT) = 3U;
		delay_until(n);
	}
}

void nighttrain_sk6812_parallel_spi(
	size_t ct,
	uint32_t *s0,
	uint32_t *s1,
	uint32_t *s2,
	uint32_t *s3,
	uint32_t *s4,
	uint32_t *s5,
	uint32_t *s6,
	uint32_t *s7)
{
	while (ct-- > 0) {
		uint32_t v0 = *s0++;
		uint32_t v1 = *s1++;
		uint32_t v2 = *s2++;
		uint32_t v3 = *s3++;
		uint32_t v4 = *s4++;
		uint32_t v5 = *s5++;
		uint32_t v6 = *s6++;
		uint32_t v7 = *s7++;

		asm volatile("cpsid i":::"memory");
		for (uint32_t pos=0x00800000U; pos; pos >>= 1) {
			uint32_t clrmask = 0x00000000;

			*((volatile uint32_t *)(PORT + 0x18)) = PINMASK; /* set all pins high */

			// reset cycle counter for precise timing of ws2812 protocol
			*((volatile uint32_t *)CYCCNT) = 0U;

#define CLRMASK(v, p) if (!(v & pos)) { clrmask |= p; }
			CLRMASK(v0, LED0);
			CLRMASK(v1, LED1);
			CLRMASK(v2, LED2);
			CLRMASK(v3, LED3);
			CLRMASK(v4, LED4);
			CLRMASK(v5, LED5);
			CLRMASK(v6, LED6);
			CLRMASK(v7, LED7);

			delay_until(40);  // ~350ns

			*((volatile uint32_t *)(PORT + 0x14)) = clrmask; /* set low pins low */

			delay_until(80);  // ~700ns

			*((volatile uint32_t *)(PORT + 0x14)) = PINMASK; /* set all pins low */

			delay_until(125); // ~1083ns
		}
		asm volatile("cpsie i":::"memory");
	}
	// reset cycle counter for precise timing of ws2812 protocol
	*((volatile uint32_t *)CYCCNT) = 0U;

	delay_until(580); // ~5us
}

void nighttrain_sk9822_parallel_spi(
	size_t ct,
	uint32_t *s0,
	uint32_t *s1,
	uint32_t *s2,
	uint32_t *s3,
	uint32_t *s4,
	uint32_t *s5,
	uint32_t *s6,
	uint32_t *s7)
{
	*((volatile uint32_t *)(PORT + 0x14)) = (CLKMASK | PINMASK); /* set clock and all data pins low */

	while (ct-- > 0) {
		uint32_t v0 = *s0++;
		uint32_t v1 = *s1++;
		uint32_t v2 = *s2++;
		uint32_t v3 = *s3++;
		uint32_t v4 = *s4++;
		uint32_t v5 = *s5++;
		uint32_t v6 = *s6++;
		uint32_t v7 = *s7++;

		asm volatile("cpsid i":::"memory");
		for (uint32_t pos=0x80000000U; pos; pos >>= 1) {
			uint32_t setmask = 0x00000000;

			*((volatile uint32_t *)CYCCNT) = 0U;
#define SETMASK(v, p) if (v & pos) { setmask |= p; }
			SETMASK(v0, LED0);
			SETMASK(v1, LED1);
			SETMASK(v2, LED2);
			SETMASK(v3, LED3);
			SETMASK(v4, LED4);
			SETMASK(v5, LED5);
			SETMASK(v6, LED6);
			SETMASK(v7, LED7);

			*((volatile uint32_t *)(PORT + 0x18)) = setmask; /* set high pins high */

			*((volatile uint32_t *)(PORT + 0x18)) = CLKMASK; /* set clock high */

			delay_until(55); /* ~450ns */

			*((volatile uint32_t *)(PORT + 0x14)) = (CLKMASK | PINMASK); /* set clock and all data pins low */
		}
		asm volatile("cpsie i":::"memory");
	}
}

void nighttrain_set_pin_high(uint8_t pin)
{
	*((volatile uint32_t *)(PORT + 0x18)) = (1 << (pin  & 0x3f));
}

void nighttrain_set_pin_low(uint8_t pin)
{
	*((volatile uint32_t *)(PORT + 0x14)) = (1 << (pin  & 0x3f));
}
#endif
