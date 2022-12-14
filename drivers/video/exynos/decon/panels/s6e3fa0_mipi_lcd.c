/* drivers/video/exynos/panels/s6e3fa0_mipi_lcd.c
 *
 * Samsung SoC MIPI LCD driver.
 *
 * Copyright (c) 2014 Samsung Electronics
 *
 * Haowei Li, <haowei.li@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/delay.h>
#include <linux/gpio.h>
#include <video/mipi_display.h>
#include <linux/platform_device.h>

#include "s6e3fa0_param.h"
#include "lcd_ctrl.h"
#include "decon_lcd.h"
#include "../dsim.h"

#define MAX_BRIGHTNESS 255
#define MIN_BRIGHTNESS 0
#define DEFAULT_BRIGHTNESS 0
#ifdef CONFIG_SUPPORT_CODEGEN
extern struct mipi_dsim_lcd_driver mipi_lcd_driver;
#endif
static struct dsim_device *dsim_base;
static struct backlight_device *bd;

static int s6e3fa0_get_brightness(struct backlight_device *bd)
{
	return bd->props.brightness;
}

static int get_backlight_level(int brightness)
{
	int backlightlevel;

	switch (brightness) {
	case 0:
		backlightlevel = 0;
		break;
	case 1 ... 29:
		backlightlevel = 0;
		break;
	case 30 ... 34:
		backlightlevel = 1;
		break;
	case 35 ... 39:
		backlightlevel = 2;
		break;
	case 40 ... 44:
		backlightlevel = 3;
		break;
	case 45 ... 49:
		backlightlevel = 4;
		break;
	case 50 ... 54:
		backlightlevel = 5;
		break;
	case 55 ... 64:
		backlightlevel = 6;
		break;
	case 65 ... 74:
		backlightlevel = 7;
		break;
	case 75 ... 83:
		backlightlevel = 8;
		break;
	case 84 ... 93:
		backlightlevel = 9;
		break;
	case 94 ... 103:
		backlightlevel = 10;
		break;
	case 104 ... 113:
		backlightlevel = 11;
		break;
	case 114 ... 122:
		backlightlevel = 12;
		break;
	case 123 ... 132:
		backlightlevel = 13;
		break;
	case 133 ... 142:
		backlightlevel = 14;
		break;
	case 143 ... 152:
		backlightlevel = 15;
		break;
	case 153 ... 162:
		backlightlevel = 16;
		break;
	case 163 ... 171:
		backlightlevel = 17;
		break;
	case 172 ... 181:
		backlightlevel = 18;
		break;
	case 182 ... 191:
		backlightlevel = 19;
		break;
	case 192 ... 201:
		backlightlevel = 20;
		break;
	case 202 ... 210:
		backlightlevel = 21;
		break;
	case 211 ... 220:
		backlightlevel = 22;
		break;
	case 221 ... 230:
		backlightlevel = 23;
		break;
	case 231 ... 240:
		backlightlevel = 24;
		break;
	case 241 ... 250:
		backlightlevel = 25;
		break;
	case 251 ... 255:
		backlightlevel = 26;
		break;
	default:
		backlightlevel = 12;
		break;
	}

	return backlightlevel;
}

static int update_brightness(int brightness)
{
	int backlightlevel;

	backlightlevel = get_backlight_level(brightness);
	/* Need to implemnt
	while (s5p_mipi_dsi_wr_data(dsim_base, MIPI_DSI_DCS_LONG_WRITE,
			(unsigned int)gamma22_table[backlightlevel],
				GAMMA_PARAM_SIZE) == -1)
		printk(KERN_ERR "fail to write gamma value.\n");

	while (s5p_mipi_dsi_wr_data(dsim_base, MIPI_DSI_DCS_LONG_WRITE,
			(unsigned int)SEQ_GAMMA_UPDATE,
				ARRAY_SIZE(SEQ_GAMMA_UPDATE)) == -1)
		printk(KERN_ERR "fail to update gamma value.\n");
	*/
	return 0;
}

static int s6e3fa0_set_brightness(struct backlight_device *bd)
{
	int brightness = bd->props.brightness;

	if (brightness < MIN_BRIGHTNESS || brightness > MAX_BRIGHTNESS) {
		printk(KERN_ALERT "Brightness should be in the range of 0 ~ 255\n");
		return -EINVAL;
	}

	update_brightness(brightness);

	return 1;
}

static const struct backlight_ops s6e3fa0_backlight_ops = {
	.get_brightness = s6e3fa0_get_brightness,
	.update_status = s6e3fa0_set_brightness,
};

static int s6e3fa0_displayon(struct dsim_device *dsim)
{
	lcd_s6e3fa0_init(dsim->id, &dsim->lcd_info);
	lcd_enable(dsim->id);
	return 1;
}

static int s6e3fa0_suspend(struct dsim_device *dsim)
{
	return 1;
}

static int s6e3fa0_resume(struct dsim_device *dsim)
{
	return 1;
}

static int s6e3fa0_dsim_probe(struct dsim_device *dsim)
{
	char bl_name[SZ_64];
	dsim_base = dsim;

	snprintf(bl_name, SZ_64, "pwm-backlight.%d", dsim->id);
	bd = backlight_device_register(bl_name, NULL,
		NULL, &s6e3fa0_backlight_ops, NULL);
	if (IS_ERR(bd))
		printk(KERN_ALERT "failed to register backlight device!\n");

	bd->props.max_brightness = MAX_BRIGHTNESS;
	bd->props.brightness = DEFAULT_BRIGHTNESS;

	return 1;
}

#ifdef CONFIG_SUPPORT_CODEGEN
static int s6e3fa0_probe(struct platform_device *pdev)
{
	mipi_lcd_driver.probe		= s6e3fa0_dsim_probe;
	mipi_lcd_driver.displayon = s6e3fa0_displayon;
	mipi_lcd_driver.suspend	= s6e3fa0_suspend;
	mipi_lcd_driver.resume		= s6e3fa0_resume;

	return 0;
}

static int s6e3fa0_remove(struct platform_device *pdev)
{
	return 0;
}

static const struct of_device_id s6e3fa0_device_table[] = {
	        { .compatible = "samsung,s6e3fa0" },
		{},
};
static struct platform_driver s6e3fa0_driver __refdata = {
	.probe		= s6e3fa0_probe,
	.remove		= s6e3fa0_remove,
	.driver = {
		.name	= "exynos-mipi-dsi-s6e3fa0",
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(s6e3fa0_device_table),
	}
};

static int exynos_s6e3fa0_register(void)
{
	platform_driver_register(&s6e3fa0_driver);

	return 0;
}

static void exynos_s6e3fa0_unregister(void)
{
	platform_driver_unregister(&s6e3fa0_driver);
}
module_init(exynos_s6e3fa0_register);
module_exit(exynos_s6e3fa0_unregister);

#else
struct mipi_dsim_lcd_driver mipi_lcd_driver = {
	.probe		= s6e3fa0_dsim_probe,
	.displayon	= s6e3fa0_displayon,
	.suspend	= s6e3fa0_suspend,
	.resume		= s6e3fa0_resume,
};
#endif
