/* linux/drivers/video/backlight/s6e3fa3_mipi_lcd.c
 *
 * exynos7420 SoC MIPI LCD driver.
 *
 * Copyright (c) 2012 MEIZU
 *
 * WangBo, <wangbo@meizu.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/module.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <video/mipi_display.h>
#include <linux/platform_device.h>
#include "lcd_ctrl.h"
#include "decon_lcd.h"
#include "../dsim.h"

static int lcd_id[3] = {0};
module_param_array(lcd_id, int, NULL, S_IRUGO | S_IWUSR | S_IWGRP);

bool is_white_lcd(void)
{
	return ((lcd_id[0] & 0x03) ? true : false);
}

struct lcd_cmd {
	unsigned int type;
	unsigned char *data;
	unsigned int size;
	unsigned int delay;
};

#define INIT_CMD(cmd, type_v, delay_v) \
	static struct lcd_cmd cmd##_s = { \
		.type = type_v, \
		.data = cmd, \
		.size = ARRAY_SIZE(cmd), \
		.delay = delay_v, \
	}

#define WRITE_LCD(id, cmd) \
	do { \
		switch (cmd##_s.type) { \
		case MIPI_DSI_GENERIC_SHORT_WRITE_2_PARAM: \
		case MIPI_DSI_DCS_SHORT_WRITE: \
		case MIPI_DSI_DCS_SHORT_WRITE_PARAM: \
			if (dsim_wr_data(id, cmd##_s.type, cmd##_s.data[0], cmd##_s.data[1])) \
				pr_err("Error: failed to write %s command!\r\n", #cmd); \
			break; \
		default: \
			if (dsim_wr_data(id, cmd##_s.type, (unsigned long)cmd##_s.data, cmd##_s.size)) \
				pr_err("Error: failed to write %s command!\r\n", #cmd); \
			break; \
		}; \
		if (cmd##_s.delay) \
			usleep_range(cmd##_s.delay * 1000, cmd##_s.delay * 1000); \
	} while(0)

#define READ_LCD(dsim, addr, count, buf, ret) \
	do { \
		ret = dsim_read_data(dsim, MIPI_DSI_DCS_READ, addr, count, buf); \
	} while (0)

static unsigned char TEON[]	     = {0x35, 0x00};
static unsigned char SLEEP_OUT[]     = {0x11, 0x00};
static unsigned char DISP_ON[]	     = {0x29, 0x00};
static unsigned char DISP_OFF[]      = {0x28, 0x00};
static unsigned char SLEEP_IN[]      = {0x10, 0x00};

static unsigned char DIMMING_SPEED[] = {0x53, 0x20};
static unsigned char ACL_MODE[]      = {0x55, 0x00};
static unsigned char LUMINANCE[]     = {0x51, 0x00};

INIT_CMD(TEON, 0x15, 20);
INIT_CMD(SLEEP_OUT, 0x05, 20);
INIT_CMD(DISP_ON, 0x05, 0);
INIT_CMD(DISP_OFF, 0x05, 10);
INIT_CMD(SLEEP_IN, 0x05, 150);

INIT_CMD(DIMMING_SPEED, 0x15, 0);
INIT_CMD(ACL_MODE, 0x15, 0);
INIT_CMD(LUMINANCE, 0x15, 80);


#define MAX_BRIGHTNESS 255
#define MIN_BRIGHTNESS 0
#define DEF_BRIGHTNESS 180

static struct backlight_device *bd;

static int s6e3fa3_get_brightness(struct backlight_device *bd)
{
	return bd->props.brightness;
}

static int update_brightness(struct dsim_device *dsim, int brightness)
{
	unsigned char brightness_60nit[2] = {
		0x51, DEF_BRIGHTNESS
	};

	if (dsim == NULL) {
		printk("dsim struct has been set to NULL,check it\n");
		BUG();
	}

	if (brightness < MIN_BRIGHTNESS) brightness = MIN_BRIGHTNESS;
	if (brightness > MAX_BRIGHTNESS) brightness = MAX_BRIGHTNESS;

	brightness_60nit[1] = brightness;
	dsim_wr_data(dsim->id, MIPI_DSI_DCS_LONG_WRITE,
			(unsigned long)brightness_60nit, 2);
	return 1;
}

static int s6e3fa3_set_brightness(struct backlight_device *bd)
{
	int brightness = bd->props.brightness;
	struct dsim_device *dsim = dev_get_drvdata(&bd->dev);

	if (brightness < MIN_BRIGHTNESS || brightness > MAX_BRIGHTNESS) {
		pr_info("Brightness should be in the range of 0 ~ 255\n");
		return -EINVAL;
	}

	update_brightness(dsim, brightness);

	return 1;
}

static const struct backlight_ops s6e3fa3_backlight_ops = {
	.get_brightness = s6e3fa3_get_brightness,
	.update_status = s6e3fa3_set_brightness,
};

static int s6e3fa3_dsim_probe(struct dsim_device *dsim)
{
	int i, ret = 0;
	int count = 3;
	u8 buf[4] = {0};
	u32 rd_addr = 0x04;

	if (dsim->id == 0){
		bd = backlight_device_register("pwm-backlight.0", NULL,
			dsim, &s6e3fa3_backlight_ops, NULL);
		if (IS_ERR(bd))
			pr_info("failed to register backlight device!\n");
	}else if (dsim->id == 1){
		bd = backlight_device_register("pwm-backlight.1", NULL,
			dsim, &s6e3fa3_backlight_ops, NULL);
		if (IS_ERR(bd))
			pr_info("failed to register backlight device!\n");
	}
	
	bd->props.max_brightness = MAX_BRIGHTNESS;
	bd->props.brightness = DEF_BRIGHTNESS;

	READ_LCD(dsim, rd_addr, count, buf, ret);
	if (ret < 0) {
		printk("[LCD] ERROR: failed to read lcd ID!\r\n");
		goto out;
	}

	for (i = 0; i < 3; i++)
		lcd_id[i] = (int)buf[i];

	printk("[LCD] INFO: succeed to communicate with lcd and it is %s [0x%x,0x%x,0x%x].\r\n", (is_white_lcd() ? "white lcd" : "black lcd"), lcd_id[0], lcd_id[1], lcd_id[2]);

out:
	return 1;
}

void lcd_init(int id, struct decon_lcd *lcd)
{
	WRITE_LCD(id, SLEEP_OUT);
	WRITE_LCD(id, TEON);
	WRITE_LCD(id, DIMMING_SPEED);
	WRITE_LCD(id, ACL_MODE);
	WRITE_LCD(id, LUMINANCE);
	WRITE_LCD(id, DISP_ON);
}

static int s6e3fa3_displayon(struct dsim_device *dsim)
{
	lcd_init(dsim->id, &dsim->lcd_info);
	update_brightness(dsim, DEF_BRIGHTNESS);
	return 1;
}

static int s6e3fa3_suspend(struct dsim_device *dsim)
{
	WRITE_LCD(dsim->id, DISP_OFF);
	WRITE_LCD(dsim->id, SLEEP_IN);

	return 1;
}

static int s6e3fa3_resume(struct dsim_device *dsim)
{
	return 1;
}

#ifdef CONFIG_SUPPORT_CODEGEN
static int s6e3fa3_probe(struct platform_device *pdev)
{
	mipi_lcd_driver.probe		= s6e3fa3_dsim_probe;
	mipi_lcd_driver.displayon = s6e3fa3_displayon;
	mipi_lcd_driver.suspend	= s6e3fa3_suspend;
	mipi_lcd_driver.resume		= s6e3fa3_resume;

	return 0;
}

static int s6e3fa3_remove(struct platform_device *pdev)
{
	return 0;
}

static const struct of_device_id s6e3fa3_device_table[] = {
	        { .compatible = "samsung,s6e3fa3" },
		{},
};
static struct platform_driver s6e3fa3_driver __refdata = {
	.probe		= s6e3fa3_probe,
	.remove		= s6e3fa3_remove,
	.driver = {
		.name	= "exynos-mipi-dsi-s6e3fa3",
		.owner	= THIS_MODULE,
		.of_match_table = of_match_ptr(s6e3fa3_device_table),
	}
};

static int exynos_s6e3fa3_register(void)
{
	platform_driver_register(&s6e3fa3_driver);

	return 0;
}

static void exynos_s6e3fa3_unregister(void)
{
	platform_driver_unregister(&s6e3fa3_driver);
}
module_init(exynos_s6e3fa3_register);
module_exit(exynos_s6e3fa3_unregister);

#else	
struct mipi_dsim_lcd_driver s6e3fa3_mipi_lcd_driver= {
	.probe		= s6e3fa3_dsim_probe,
	.displayon	= s6e3fa3_displayon,
	.suspend	= s6e3fa3_suspend,
	.resume		= s6e3fa3_resume,
};
#endif
