/* linux/drivers/video/exynos/decon/dpu_reg_7420.c
 *
 * Copyright 2013-2015 Samsung Electronics
 *      Haowei Li <haowei.li@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include "dpu_reg.h"
#include "regs-dpu.h"

/* contrast LUT */

const u32 gamma_table1[][65] = {
	{   0,  3,    6,   9,  12,  16,  19,  22,  25,  29,
	   32,  35,  39,  43,  46,  50,  54,  58,  62,  66,
	   71,  75,  80,  84,  89,  94,  99, 103, 108, 113,
	  118, 123, 128, 133, 138, 143, 148, 153, 158, 162,
	  167, 172, 176, 181, 185, 190, 194, 198, 202, 206,
	  210, 213, 217, 221, 224, 227, 231, 234, 237, 240,
	  243, 247, 250, 253, 256 },
	{   0,   3,   6,   9,  12,  16,  19,  22,  25,  29,
	   32,  35,  39,  43,  46,  50,  54,  58,  62,  66,
	   71,  75,  80,  84,  89,  94,  99, 103, 108, 113,
	  118, 123, 128, 133, 138, 143, 148, 153, 158, 162,
	  167, 172, 176, 181, 185, 190, 194, 198, 202, 206,
	  210, 213, 217, 221, 224, 227, 231, 234, 237, 240,
	  243, 247, 250, 253, 256 },
	{   0,   3,   6,   9,  12,  16,  19,  22,  25,  29,
	   32,  35,  39,  43,  46,  50,  54,  58,  62,  66,
	   71,  75,  80,  84,  89,  94,  99, 103, 108, 113,
	  118, 123, 128, 133, 138, 143, 148, 153, 158, 162,
	  167, 172, 176, 181, 185, 190, 194, 198, 202, 206,
	  210, 213, 217, 221, 224, 227, 231, 234, 237, 240,
	  243, 247, 250, 253, 256 },
};

const u32 gamma_table2[][65] = {
	{   0,   2,   5,   7,  10,  12,  15,  17,  20,  23,
	   26,  29,  32,  35,  39,  42,  46,  50,  54,  58,
	   63,  68,  73,  78,  83,  88,  94,  99, 105, 110,
	  116, 122, 127, 133, 139, 145, 150, 156, 161, 167,
	  172, 177, 182, 187, 192, 197, 201, 205, 209, 213,
	  217, 220, 223, 227, 230, 232, 235, 238, 241, 243,
	  246, 248, 251, 253, 256 },
	{   0,   2,   5,   7,  10,  12,  15,  17,  20,  23,
	   26,  29,  32,  35,  39,  42,  46,  50,  54,  58,
	   63,  68,  73,  78,  83,  88,  94,  99, 105, 110,
	  116, 122, 127, 133, 139, 145, 150, 156, 161, 167,
	  172, 177, 182, 187, 192, 197, 201, 205, 209, 213,
	  217, 220, 223, 227, 230, 232, 235, 238, 241, 243,
	  246, 248, 251, 253, 256 },
	{   0,   2,   5,   7,  10,  12,  15,  17,  20,  23,
	   26,  29,  32,  35,  39,  42,  46,  50,  54,  58,
	   63,  68,  73,  78,  83,  88,  94,  99, 105, 110,
	  116, 122, 127, 133, 139, 145, 150, 156, 161, 167,
	  172, 177, 182, 187, 192, 197, 201, 205, 209, 213,
	  217, 220, 223, 227, 230, 232, 235, 238, 241, 243,
	  246, 248, 251, 253, 256 },
};

const u32 gamma_table3[][65] = {
	{   0,   2,   3,   5,   6,   8,  10,  12,  14,  16,
	   18,  21,  24,  27,  30,  33,  37,  41,  46,  50,
	   55,  61,  66,  72,  78,  84,  91,  98, 104, 111,
	  118, 125, 132, 139, 146, 152, 159, 166, 172, 178,
	  184, 190, 196, 201, 206, 211, 215, 219, 222, 226,
	  229, 232, 234, 237, 239, 241, 243, 245, 247, 248,
	  250, 251, 253, 254, 255 },
	{   0,   2,   3,   5,   6,   8,  10,  12,  14,  16,
	   18,  21,  24,  27,  30,  33,  37,  41,  46,  50,
	   55,  61,  66,  72,  78,  84,  91,  98, 104, 111,
	  118, 125, 132, 139, 146, 152, 159, 166, 172, 178,
	  184, 190, 196, 201, 206, 211, 215, 219, 222, 226,
	  229, 232, 234, 237, 239, 241, 243, 245, 247, 248,
	  250, 251, 253, 254, 255 },
	{   0,   2,   3,   5,   6,   8,  10,  12,  14,  16,
	   18,  21,  24,  27,  30,  33,  37,  41,  46,  50,
	   55,  61,  66,  72,  78,  84,  91,  98, 104, 111,
	  118, 125, 132, 139, 146, 152, 159, 166, 172, 178,
	  184, 190, 196, 201, 206, 211, 215, 219, 222, 226,
	  229, 232, 234, 237, 239, 241, 243, 245, 247, 248,
	  250, 251, 253, 254, 255 },
};

void dpu_reg_update_mask(u32 mask)
{
	dpu_write_mask(DPU_MASK_CTRL, mask, DPU_MASK_CTRL_MASK);
}

void dpu_reg_en_dither(u32 en)
{
	dpu_write_mask(DPUCON, en << DPU_DITHER_ON, DPU_DITHER_ON_MASK);
}

void dpu_reg_en_scr(u32 en)
{
	dpu_write_mask(DPUCON, en << DPU_SCR_ON, DPU_SCR_ON_MASK);
}

void dpu_reg_en_gamma(u32 en)
{
	dpu_write_mask(DPUCON, en << DPU_GAMMA_ON, DPU_GAMMA_ON_MASK);
}

void dpu_reg_en_hsc(u32 en)
{
	dpu_write_mask(DPUCON, en << DPU_HSC_ON, DPU_HSC_ON_MASK);
}

void dpu_reg_en_ad(u32 en)
{
	dpu_write_mask(DPUCON, en << DPU_AD_ON, DPU_AD_ON_MASK);
}

void dpu_reg_set_image_size(u32 width, u32 height)
{
	u32 data;

	data = (height << 16) | (width);
	dpu_write(DPUIMG_SIZESET, data);
}

void dpu_reg_module_on_off(bool en1, bool en2, bool en3, bool en4, bool en5)
{
	dpu_reg_update_mask(1);

	dpu_reg_en_dither(en1);
	dpu_reg_en_scr(en2);
	dpu_reg_en_gamma(en3);
	dpu_reg_en_hsc(en4);
	dpu_reg_en_ad(en5);

	dpu_reg_update_mask(0);
}

void dpu_reg_set_bright(u32 val)
{
	dpu_reg_update_mask(1);
	dpu_write_mask(DPUAD_BACKLIGHT, val, DPUAD_BACKLIGHT_MASK);
	dpu_reg_update_mask(0);
}

void dpu_reg_set_contrast_onoff(u32 val)
{
	dpu_reg_update_mask(1);
	dpu_reg_en_gamma(val);
	dpu_reg_update_mask(0);
}

void dpu_reg_set_contrast(u32 offset, u32 mask, u32 val)
{
	dpu_reg_update_mask(1);
	dpu_write_mask((DPUGAMMALUT_X_Y_BASE + offset), val, mask);
	dpu_reg_update_mask(0);
}

void dpu_reg_set_ad_onoff(u32 val)
{
	dpu_reg_update_mask(1);
	dpu_reg_en_ad(val);
	dpu_write_mask(DPUAD_NVP_CONTROL2, DPU_AD_OPT_SEL_FUNC(0x1), OPTION_SELECT_FUNCTION);
	dpu_reg_update_mask(0);
}

void dpu_reg_set_ad_backlight(u32 val)
{
	dpu_reg_update_mask(1);
	dpu_write_mask(DPUAD_BACKLIGHT, val, DPUAD_BACKLIGHT_MASK);
	dpu_reg_update_mask(0);
}

void dpu_reg_set_ad_ambient(u32 val)
{
	dpu_reg_update_mask(1);
	dpu_write_mask(DPUAD_AMBIENT_LIGHT, val, DPUAD_AMBIENT_LIGHT_MASK);
	dpu_reg_update_mask(0);
}

void dpu_reg_set_saturation_onoff(u32 val)
{
	dpu_reg_update_mask(1);
	dpu_reg_en_hsc(val);
	dpu_write_mask(DPUHSC_CONTROL, (val << 3), TSC_ON);
	dpu_reg_update_mask(0);
}

void dpu_reg_set_saturation_tscgain(u32 val)
{
	dpu_reg_update_mask(1);
	dpu_write_mask(DPUHSC_TSCGAIN_YRATIO, val, TSC_GAIN);
	dpu_reg_update_mask(0);
}

void dpu_reg_set_saturation_rgb(u32 mask, u32 val)
{
	dpu_reg_update_mask(1);
	dpu_write_mask(DPUHSC_PAIMGAIN2_1_0, val, mask);
	dpu_reg_update_mask(0);
}

void dpu_reg_set_saturation_cmy(u32 mask, u32 val)
{
	dpu_reg_update_mask(1);
	dpu_write_mask(DPUHSC_PAIMGAIN5_4_3, val, mask);
	dpu_reg_update_mask(0);
}

void dpu_reg_set_saturation_shift(u32 mask, u32 val)
{
	dpu_reg_update_mask(1);
	dpu_write_mask(DPUHSC_PAIMSCALE_SHIFT, val, mask);
	dpu_reg_update_mask(0);
}

void dpu_reg_set_hue_rgb(u32 mask, u32 val)
{
	dpu_reg_update_mask(1);
	dpu_write_mask(DPUHSC_PPHCGAIN2_1_0, val, mask);
	dpu_reg_update_mask(0);
}

void dpu_reg_set_hue_cmy(u32 mask, u32 val)
{
	dpu_reg_update_mask(1);
	dpu_write_mask(DPUHSC_PPHCGAIN5_4_3, val, mask);
	dpu_reg_update_mask(0);
}

void dpu_reg_set_hue_onoff(u32 val)
{
	dpu_reg_update_mask(1);
	dpu_reg_en_hsc(val);
	dpu_write_mask(DPUHSC_CONTROL, val << 1, PPHC_ON);
	dpu_reg_update_mask(0);
}

void dpu_reg_start(u32 w, u32 h)
{
	u32 id = 0;

	decon_reg_enable_apb_clk(id, 1);

	dpu_reg_module_on_off(0, 0, 0, 0, 0);

	dpu_reg_set_image_size(w, h);
	decon_reg_set_pixel_count_se(id, w, h);
	decon_reg_set_image_size_se(id, w, h);
	decon_reg_set_porch_se(id, DPU_VFP, DPU_VSA, DPU_VBP,
				DPU_HFP, DPU_HSA, DPU_HBP);
	decon_reg_set_bit_order_se(id, DPU_R1G1B1R0G0B0, DPU_R0G0B0R1G1B1);

	decon_reg_enable_dpu(id, 1);
}

void dpu_reg_stop(void)
{
	u32 id = 0;

	decon_reg_enable_dpu(id, 0);
	decon_reg_enable_apb_clk(id, 0);
}

