// Copyright (c) 2019 David Grimes <david@grimes-family.com>

#include <assert.h>
#include "__init__.h"

//| :mod:`nighttrain` --- Low-level animation and neopixel/dotstar parallel renderer
//| ===========================================================
//|
//| .. module:: nighttrain
//|   :synopsis: Low-level animation and neopixel/dotstar parallel renderer
//|   :platform: SAMD51
//|
STATIC mp_obj_t mod_nighttrain_initialize(void)
{
	nighttrain_initialize();

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_nighttrain_initialize_obj, mod_nighttrain_initialize);

STATIC mp_obj_t mod_nighttrain_altitude_from_baro(mp_obj_t p, mp_obj_t t)
{
	mp_float_t alt = nighttrain_altitude_from_baro(mp_obj_get_float(p), mp_obj_get_float(t));

	return mp_obj_new_float(alt);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_nighttrain_altitude_from_baro_obj, mod_nighttrain_altitude_from_baro);

STATIC mp_obj_t mod_nighttrain_sk6812_render(size_t n_args, const mp_obj_t *args)
{
	mp_buffer_info_t led, g, c;

	assert(n_args == 6);
	mp_get_buffer_raise(args[1], &led, MP_BUFFER_WRITE);
	mp_get_buffer_raise(args[2], &g, MP_BUFFER_READ);
	mp_get_buffer_raise(args[4], &c, MP_BUFFER_READ);

	nighttrain_sk6812_render(
		(uint8_t)mp_obj_get_int(args[0]),
		(uint32_t *)led.buf,
		led.len >> 2,
		(uint8_t *)g.buf,
		g.len,
		mp_obj_get_int(args[3]),
		(uint32_t *)c.buf,
		c.len >> 2,
		mp_obj_get_int(args[5])
		);

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_nighttrain_sk6812_render_obj, 6, 6, mod_nighttrain_sk6812_render);

STATIC mp_obj_t mod_nighttrain_sk6812_blend(mp_obj_t dst, mp_obj_t src, mp_obj_t f)
{
	mp_buffer_info_t dstbi, srcbi;

	mp_get_buffer_raise(dst, &dstbi, MP_BUFFER_WRITE);
	mp_get_buffer_raise(src, &srcbi, MP_BUFFER_READ);

	// XXX: confirm dst.len == src.len
	nighttrain_sk6812_blend(
		(uint32_t *)dstbi.buf,
		(uint32_t *)srcbi.buf,
		dstbi.len >> 2,
		(uint8_t)mp_obj_get_int(f)
		);

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_nighttrain_sk6812_blend_obj, mod_nighttrain_sk6812_blend);

STATIC mp_obj_t mod_nighttrain_skxxxx_reverse(mp_obj_t buf)
{
	mp_buffer_info_t led;

	mp_get_buffer_raise(buf, &led, MP_BUFFER_WRITE);

	nighttrain_skxxxx_reverse(
		(uint32_t *)led.buf,
		led.len >> 2
		);

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_nighttrain_skxxxx_reverse_obj, mod_nighttrain_skxxxx_reverse);

STATIC mp_obj_t mod_nighttrain_print(size_t n_args, const mp_obj_t *args)
{
	mp_buffer_info_t t, s0, s1, s2, s3, s4;

	assert(n_args == 8);
	mp_get_buffer_raise(args[2], &t, MP_BUFFER_READ);
	mp_get_buffer_raise(args[3], &s0, MP_BUFFER_WRITE);
	mp_get_buffer_raise(args[4], &s1, MP_BUFFER_WRITE);
	mp_get_buffer_raise(args[5], &s2, MP_BUFFER_WRITE);
	mp_get_buffer_raise(args[6], &s3, MP_BUFFER_WRITE);
	mp_get_buffer_raise(args[7], &s4, MP_BUFFER_WRITE);

	nighttrain_print(
		(uint8_t)mp_obj_get_int(args[0]),
		(uint8_t)mp_obj_get_int(args[1]),
		(uint8_t *)t.buf,
		t.len,
		(uint8_t *)s0.buf,
		(uint8_t *)s1.buf,
		(uint8_t *)s2.buf,
		(uint8_t *)s3.buf,
		(uint8_t *)s4.buf
		);

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_nighttrain_print_obj, 8, 8, mod_nighttrain_print);

STATIC mp_obj_t mod_nighttrain_sk9822_render_hardware_gamma(size_t n_args, const mp_obj_t *args)
{
	mp_buffer_info_t led, g, c;

	assert(n_args == 6);
	mp_get_buffer_raise(args[1], &led, MP_BUFFER_WRITE);
	mp_get_buffer_raise(args[2], &g, MP_BUFFER_READ);
	mp_get_buffer_raise(args[4], &c, MP_BUFFER_READ);

	nighttrain_sk9822_render_hardware_gamma(
		(uint8_t)mp_obj_get_int(args[0]),
		(uint32_t *)led.buf,
		led.len >> 2,
		(uint8_t *)g.buf,
		g.len,
		mp_obj_get_int(args[3]),
		(uint32_t *)c.buf,
		c.len >> 2,
		mp_obj_get_int(args[5])
		);

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_nighttrain_sk9822_render_hardware_gamma_obj, 6, 6, mod_nighttrain_sk9822_render_hardware_gamma);

STATIC mp_obj_t mod_nighttrain_sk9822_render_software_gamma(size_t n_args, const mp_obj_t *args)
{
	mp_buffer_info_t led, g, c;

	assert(n_args == 6);
	mp_get_buffer_raise(args[1], &led, MP_BUFFER_WRITE);
	mp_get_buffer_raise(args[2], &g, MP_BUFFER_READ);
	mp_get_buffer_raise(args[4], &c, MP_BUFFER_READ);

	nighttrain_sk9822_render_software_gamma(
		(uint8_t)mp_obj_get_int(args[0]),
		(uint32_t *)led.buf,
		led.len >> 2,
		(uint8_t *)g.buf,
		g.len,
		mp_obj_get_int(args[3]),
		(uint32_t *)c.buf,
		c.len >> 2,
		mp_obj_get_int(args[5])
		);

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_nighttrain_sk9822_render_software_gamma_obj, 6, 6, mod_nighttrain_sk9822_render_software_gamma);

STATIC mp_obj_t mod_nighttrain_sk9822_blend_hardware_gamma(mp_obj_t dst, mp_obj_t src, mp_obj_t f)
{
	mp_buffer_info_t dstbi, srcbi;

	mp_get_buffer_raise(dst, &dstbi, MP_BUFFER_WRITE);
	mp_get_buffer_raise(src, &srcbi, MP_BUFFER_READ);

	// XXX: confirm dst.len == src.len
	nighttrain_sk9822_blend_hardware_gamma(
		(uint32_t *)dstbi.buf,
		(uint32_t *)srcbi.buf,
		dstbi.len >> 2,
		(uint8_t)mp_obj_get_int(f)
		);

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_nighttrain_sk9822_blend_hardware_gamma_obj, mod_nighttrain_sk9822_blend_hardware_gamma);

STATIC mp_obj_t mod_nighttrain_sk9822_blend_software_gamma(mp_obj_t dst, mp_obj_t src, mp_obj_t f)
{
	mp_buffer_info_t dstbi, srcbi;

	mp_get_buffer_raise(dst, &dstbi, MP_BUFFER_WRITE);
	mp_get_buffer_raise(src, &srcbi, MP_BUFFER_READ);

	// XXX: confirm dst.len == src.len
	nighttrain_sk9822_blend_software_gamma(
		(uint32_t *)dstbi.buf,
		(uint32_t *)srcbi.buf,
		dstbi.len >> 2,
		(uint8_t)mp_obj_get_int(f)
		);

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_nighttrain_sk9822_blend_software_gamma_obj, mod_nighttrain_sk9822_blend_software_gamma);

STATIC mp_obj_t mod_nighttrain_termio_render(size_t n_args, const mp_obj_t *args)
{
	mp_buffer_info_t led, g, c;

	assert(n_args == 5);
	mp_get_buffer_raise(args[0], &led, MP_BUFFER_WRITE);
	mp_get_buffer_raise(args[1], &g, MP_BUFFER_READ);
	mp_get_buffer_raise(args[3], &c, MP_BUFFER_READ);

	nighttrain_termio_render(
		(uint8_t *)led.buf,
		led.len / 20,
		(uint8_t *)g.buf,
		g.len,
		mp_obj_get_int(args[2]),
		(uint32_t *)c.buf,
		c.len >> 2,
		mp_obj_get_int(args[4])
		);

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_nighttrain_termio_render_obj, 5, 5, mod_nighttrain_termio_render);

STATIC mp_obj_t mod_nighttrain_termio_blend(mp_obj_t dst, mp_obj_t src, mp_obj_t f)
{
	mp_buffer_info_t dstbi, srcbi;

	mp_get_buffer_raise(dst, &dstbi, MP_BUFFER_WRITE);
	mp_get_buffer_raise(src, &srcbi, MP_BUFFER_READ);

	// XXX: confirm dst.len == src.len
	nighttrain_termio_blend(
		(uint8_t *)dstbi.buf,
		(uint8_t *)srcbi.buf,
		dstbi.len / 20,
		(uint8_t)mp_obj_get_int(f)
		);

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_nighttrain_termio_blend_obj, mod_nighttrain_termio_blend);

STATIC mp_obj_t mod_nighttrain_termio_reverse(mp_obj_t buf)
{
	mp_buffer_info_t led;

	mp_get_buffer_raise(buf, &led, MP_BUFFER_WRITE);

	nighttrain_termio_reverse(
		(uint8_t *)led.buf,
		led.len / 20
		);

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_nighttrain_termio_reverse_obj, mod_nighttrain_termio_reverse);

#ifdef SAMD51
STATIC mp_obj_t mod_nighttrain_sk6812_parallel_spi(size_t n_args, const mp_obj_t *args)
{
	mp_buffer_info_t bi[8];
	size_t i;

	for (i=0; i < n_args; i++) {
		mp_get_buffer_raise(args[i], &bi[i], MP_BUFFER_READ);
	}
	for ( ; i < 8; i++) {
		bi[i] = bi[n_args-1];
	}

	nighttrain_sk6812_parallel_spi(
		(bi[0].len >> 2),
		(uint32_t *)bi[0].buf,
		(uint32_t *)bi[1].buf,
		(uint32_t *)bi[2].buf,
		(uint32_t *)bi[3].buf,
		(uint32_t *)bi[4].buf,
		(uint32_t *)bi[5].buf,
		(uint32_t *)bi[6].buf,
		(uint32_t *)bi[7].buf
		);

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_nighttrain_sk6812_parallel_spi_obj, 1, 8, mod_nighttrain_sk6812_parallel_spi);

STATIC mp_obj_t mod_nighttrain_sk9822_parallel_spi(size_t n_args, const mp_obj_t *args)
{
	mp_buffer_info_t bi[8];
	size_t i;

	for (i=0; i < n_args; i++) {
		mp_get_buffer_raise(args[i], &bi[i], MP_BUFFER_READ);
	}
	for (; i < 8; i++) {
		bi[i] = bi[n_args-1];
	}

	nighttrain_sk9822_parallel_spi(
		(bi[0].len >> 2),
		(uint32_t *)bi[0].buf,
		(uint32_t *)bi[1].buf,
		(uint32_t *)bi[2].buf,
		(uint32_t *)bi[3].buf,
		(uint32_t *)bi[4].buf,
		(uint32_t *)bi[5].buf,
		(uint32_t *)bi[6].buf,
		(uint32_t *)bi[7].buf
		);

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_nighttrain_sk9822_parallel_spi_obj, 1, 8, mod_nighttrain_sk9822_parallel_spi);

STATIC mp_obj_t mod_nighttrain_set_pin_high(mp_obj_t pin)
{
	nighttrain_set_pin_high((uint8_t)mp_obj_get_int(pin));

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_nighttrain_set_pin_high_obj, mod_nighttrain_set_pin_high);

STATIC mp_obj_t mod_nighttrain_set_pin_low(mp_obj_t pin)
{
	nighttrain_set_pin_low((uint8_t)mp_obj_get_int(pin));

	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_nighttrain_set_pin_low_obj, mod_nighttrain_set_pin_low);
#endif

STATIC const mp_rom_map_elem_t mp_module_nighttrain_globals_table[] = {
	{ MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_nighttrain) },
	{ MP_ROM_QSTR(MP_QSTR_initialize), MP_ROM_PTR(&mod_nighttrain_initialize_obj) },
	{ MP_ROM_QSTR(MP_QSTR_altitude_from_baro), MP_ROM_PTR(&mod_nighttrain_altitude_from_baro_obj) },
	{ MP_ROM_QSTR(MP_QSTR_sk6812_render), MP_ROM_PTR(&mod_nighttrain_sk6812_render_obj) },
	{ MP_ROM_QSTR(MP_QSTR_sk6812_blend), MP_ROM_PTR(&mod_nighttrain_sk6812_blend_obj) },
#ifdef SAMD51
	{ MP_ROM_QSTR(MP_QSTR_sk6812_parallel_spi), MP_ROM_PTR(&mod_nighttrain_sk6812_parallel_spi_obj) },
#endif
	{ MP_ROM_QSTR(MP_QSTR_sk9822_render_hardware_gamma), MP_ROM_PTR(&mod_nighttrain_sk9822_render_hardware_gamma_obj) },
	{ MP_ROM_QSTR(MP_QSTR_sk9822_render_software_gamma), MP_ROM_PTR(&mod_nighttrain_sk9822_render_software_gamma_obj) },
	{ MP_ROM_QSTR(MP_QSTR_sk9822_blend_hardware_gamma), MP_ROM_PTR(&mod_nighttrain_sk9822_blend_hardware_gamma_obj) },
	{ MP_ROM_QSTR(MP_QSTR_sk9822_blend_software_gamma), MP_ROM_PTR(&mod_nighttrain_sk9822_blend_software_gamma_obj) },
#ifdef SAMD51
	{ MP_ROM_QSTR(MP_QSTR_sk9822_parallel_spi), MP_ROM_PTR(&mod_nighttrain_sk9822_parallel_spi_obj) },
#endif
	{ MP_ROM_QSTR(MP_QSTR_skxxxx_reverse), MP_ROM_PTR(&mod_nighttrain_skxxxx_reverse_obj) },
	{ MP_ROM_QSTR(MP_QSTR_print), MP_ROM_PTR(&mod_nighttrain_print_obj) },
	{ MP_ROM_QSTR(MP_QSTR_termio_render), MP_ROM_PTR(&mod_nighttrain_termio_render_obj) },
	{ MP_ROM_QSTR(MP_QSTR_termio_blend), MP_ROM_PTR(&mod_nighttrain_termio_blend_obj) },
	{ MP_ROM_QSTR(MP_QSTR_termio_reverse), MP_ROM_PTR(&mod_nighttrain_termio_reverse_obj) },
#ifdef SAMD51
	{ MP_ROM_QSTR(MP_QSTR_set_pin_high), MP_ROM_PTR(&mod_nighttrain_set_pin_high_obj) },
	{ MP_ROM_QSTR(MP_QSTR_set_pin_low), MP_ROM_PTR(&mod_nighttrain_set_pin_low_obj) },
#endif
};

STATIC MP_DEFINE_CONST_DICT(mp_module_nighttrain_globals, mp_module_nighttrain_globals_table);

const mp_obj_module_t mp_module_nighttrain = {
	.base = { &mp_type_module },
	.globals = (mp_obj_dict_t*)&mp_module_nighttrain_globals,
};
