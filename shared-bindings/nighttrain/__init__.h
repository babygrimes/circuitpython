// Copyright (c) 2019 David Grimes <david@grimes-family.com>

#ifndef MICROPY_INCLUDED_SHARED_BINDINGS_NIGHTTRAIN___INIT___H
#define MICROPY_INCLUDED_SHARED_BINDINGS_NIGHTTRAIN___INIT___H

#include "py/runtime.h"

void nighttrain_initialize(void);

mp_float_t nighttrain_altitude_from_baro(mp_float_t p, mp_float_t t);

void nighttrain_sk6812_render(uint8_t y, uint32_t *led, size_t n, uint8_t *g, size_t gn, int32_t gi, uint32_t *c, size_t cn, int32_t ci);

void nighttrain_sk6812_blend(uint32_t *dst, uint32_t *src, size_t n, uint8_t f);

void nighttrain_sk9822_render_hardware_gamma(uint8_t y, uint32_t *led, size_t n, uint8_t *g, size_t gn, int32_t gi, uint32_t *c, size_t cn, int32_t ci);

void nighttrain_sk9822_render_software_gamma(uint8_t y, uint32_t *led, size_t n, uint8_t *g, size_t gn, int32_t gi, uint32_t *c, size_t cn, int32_t ci);

void nighttrain_sk9822_blend_hardware_gamma(uint32_t *dst, uint32_t *src, size_t n, uint8_t f);

void nighttrain_sk9822_blend_software_gamma(uint32_t *dst, uint32_t *src, size_t n, uint8_t f);

void nighttrain_skxxxx_reverse(uint32_t *led, size_t n);

void nighttrain_print(
	uint8_t y,
	uint8_t p,
	uint8_t *t,
	size_t n,
	uint8_t *s0,
	uint8_t *s1,
	uint8_t *s2,
	uint8_t *s3,
	uint8_t *s4
	);

void nighttrain_termio_render(uint8_t *led, size_t n, uint8_t *g, size_t gn, int32_t gi, uint32_t *c, size_t cn, int32_t ci);

void nighttrain_termio_blend(uint8_t *dst, uint8_t *src, size_t n, uint8_t f);

void nighttrain_termio_reverse(uint8_t *led, size_t n);

#ifdef SAMD51
void nighttrain_sk6812_parallel_spi(
	size_t ct,
	uint32_t *s0,
	uint32_t *s1,
	uint32_t *s2,
	uint32_t *s3,
	uint32_t *s4,
	uint32_t *s5,
	uint32_t *s6,
	uint32_t *s7
	);

void nighttrain_sk9822_parallel_spi(
	size_t ct,
	uint32_t *s0,
	uint32_t *s1,
	uint32_t *s2,
	uint32_t *s3,
	uint32_t *s4,
	uint32_t *s5,
	uint32_t *s6,
	uint32_t *s7
	);

void nighttrain_set_pin_high(uint8_t pin);

void nighttrain_set_pin_low(uint8_t pin);
#endif

#endif // MICROPY_INCLUDED_SHARED_BINDINGS_NIGHTTRAIN___INIT___H
