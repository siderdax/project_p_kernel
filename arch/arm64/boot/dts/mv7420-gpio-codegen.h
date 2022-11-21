/*
 * Copyright (C) 1997-1999 Russell King
 * Copyright (C) 2012 ARM Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef __CODEGEN_GPIO_H
#define __CODEGEN_GPIO_H

#define gpioi2c0_sda_bank	gpj1
#define gpioi2c0_sda_pin	0
#define gpioi2c0_sda_func	0

#define gpioi2c0_scl_bank	gpj1
#define gpioi2c0_scl_pin	1
#define gpioi2c0_scl_func	0

#define gpioi2c1_sda_bank	gpb0
#define gpioi2c1_sda_pin	2
#define gpioi2c1_sda_func	0

#define gpioi2c1_scl_bank	gpb0
#define gpioi2c1_scl_pin	3
#define gpioi2c1_scl_func	0

#define gpioi2c2_sda_bank	gpv7
#define gpioi2c2_sda_pin	0
#define gpioi2c2_sda_func	0

#define gpioi2c2_scl_bank	gpv7
#define gpioi2c2_scl_pin	1
#define gpioi2c2_scl_func	0

#define gpioi2c3_sda_bank	gpd1
#define gpioi2c3_sda_pin	2
#define gpioi2c3_sda_func	0

#define gpioi2c3_scl_bank	gpd1
#define gpioi2c3_scl_pin	3
#define gpioi2c3_scl_func	0


#define gpioi2c4_sda_bank	gpd0
#define gpioi2c4_sda_pin	2
#define gpioi2c4_sda_func	0

#define gpioi2c4_scl_bank	gpd0
#define gpioi2c4_scl_pin	3
#define gpioi2c4_scl_func	0



#define pmic_gpio0_bank	gpg1
#define pmic_gpio0_pin	0
#define pmic_gpio0_func	0

#define pmic_gpio1_bank	gpb0
#define pmic_gpio1_pin	4
#define pmic_gpio1_func	0x2

#define pmic_gpio2_bank	gpg1
#define pmic_gpio2_pin	1
#define pmic_gpio2_func	0x1

#define pmic_buck6en_gpio 	gpb0-4


#define sub_pmic_bank gpa3
#define sub_pmic_pin 7
#define sub_pmic_func 0xf


#define ts_irq_bank	gpa0
#define ts_irq_pin	3
#define ts_irq_func	0

#define ts_rst_bank	gpf5
#define ts_rst_pin	1
#define ts_rst_func	0

#define fts_ts_rst_bank	gpd4
#define fts_ts_rst_pin	3
#define fts_ts_rst_func	0x0F

#define bat_alr_bank	gpa3
#define bat_alr_pin	4
#define bat_alr_func	0

#define chg_int_bank	gpa0
#define chg_int_pin	0
#define chg_int_func	0

#define audio_rt5647_ear_bank	gpa0
#define audio_rt5647_ear_pin	1
#define audio_rt5647_ear_func	0

#define audio_rt5647_ldoena_bank	gpf2
#define audio_rt5647_ldoena_pin	2
#define audio_rt5647_ldoena_func	0

#define audio_rt5647_rst_bank	gpf2
#define audio_rt5647_rst_pin	1
#define audio_rt5647_rst_func	 0

#define audio_wm5110_ldoena_bank	gpf2
#define audio_wm5110_ldoena_pin	2
#define audio_wm5110_ldoena_func	0

#define audio_wm5110_rst_bank	gpf2
#define audio_wm5110_rst_pin	1
#define audio_wm5110_rst_func	 0

#define mpu_irq_bank gpa1
#define mpu_irq_pin  2
#define mpu_irq_func  0

#define tmd_lp_int_bank	gpa3
#define tmd_lp_int_pin	6
#define tmd_lp_int_func	0

#define fimc_6b2_i2c_sda_bank gpb0
#define fimc_6b2_i2c_sda_pin  0
#define fimc_6b2_i2c_sda_func  0

#define fimc_6b2_i2c_scl_bank gpb0
#define fimc_6b2_i2c_scl_pin  1
#define fimc_6b2_i2c_scl_func  0
#if 1
#define bt_en_bank	gpf1
#define bt_en_pin	0
#define bt_en_func	0

#define bt_wake_bank	gpf2
#define bt_wake_pin		3
#define bt_wake_func	0
#else // bcm4339
#define bt_en_bank	gpf4
#define bt_en_pin	2
#define bt_en_func	1

#define bt_wake_bank	gpf4
#define bt_wake_pin	3
#define bt_wake_func	1
#endif

#if 1
#define bt_host_wake_bank	gpa2
#define bt_host_wake_pin		6
#define bt_host_wake_func	0xF
#else//bcm4339
#define bt_host_wake_bank	gpa1
#define bt_host_wake_pin	4
#define bt_host_wake_func	0xF
#endif

#define hdmi_cec_gpio_bank	gpa3
#define hdmi_cec_gpio_pin	1
#define hdmi_cec_gpio_func	0xf

#define pcie0_perst_gpio_bank	gph1
#define pcie0_perst_gpio_pin		2
#define pcie0_perst_gpio_func	0x1

#define pcie0_cp2ap_wake_bank		gpa3
#define pcie0_cp2ap_wake_pin		1
#define pcie0_cp2ap_wake_func		0xf

#define pcie1_perst_gpio_bank		gpg2
#define pcie1_perst_gpio_pin		0
#define pcie1_perst_gpio_func		0x1

#define pcie1_eint_bank		gpr2
#define pcie1_eint_pin		0
#define pcie1_eint_func		0x1

#if 1
#define wifi_reg_on_bank	gpf0
#define wifi_reg_on_pin		4
#define wifi_reg_on_func	0

#define wifi_host_wake_bank	gpa2
#define wifi_host_wake_pin	 	3
#define wifi_host_wake_func		0xf

#else
#define wifi_reg_on_bank	gpf4
#define wifi_reg_on_pin		1
#define wifi_reg_on_func	1

#define wifi_host_wake_bank	gpa1
#define wifi_host_wake_pin	 	6
#define wifi_host_wake_func		0xf
#endif



#define dwmmc0_clk_gpio  gpr0-0

#define dwmmc0_gpio_bank	gph1
#define dwmmc0_gpio_pin	4
#define dwmmc0_gpio_func	0

#define dwmmc1_clk_gpio		gpr2-0
#if 1
#define dwmmc2_cd_bank		gpa3
#define dwmmc2_cd_pin		5
#define dwmmc2_cd_func		0xf
#else
#define dwmmc2_cd_bank		gpr4
#define dwmmc2_cd_pin		2
#define dwmmc2_cd_func		0x3
#endif
#define dwmmc2_clk_gpio		 gpr4-0

#define key_button1_bank	gpa2
#define key_button1_pin	 7
#define key_button1_func	  0xf

#define key_button2_bank	gpa2
#define key_button2_pin		1
#define key_button2_func		0xf

#define key_button3_bank	gpa2
#define key_button3_pin		0
#define key_button3_func		0xf

#define key_button4_bank	gpa2
#define key_button4_pin		5
#define key_button4_func		0xf

#define dwc3_boost5v_en_bank	gpf5
#define dwc3_boost5v_en_pin	 	3
#define dwc3_boost5v_en_func	 1

#define dwc3_vbus_bank		gph1
#define dwc3_vbus_pin		1
#define dwc3_vbus_func		1

#define usb_bsess_bank		gpa1
#define usb_bsess_pin		0
#define usb_bsess_func		0xf

#define usb_id_bank		gpa1
#define usb_id_pin		1
#define usb_id_func		0xf
#if 1
#define decon0_TE_bank		gpf4
#define decon0_TE_pin		4
#define decon0_TE_func		0xf
#else
#define decon0_TE_bank		gpd4
#define decon0_TE_pin		3
#define decon0_TE_func		0xf
#endif

#define s6e3fa3_dsim_lcd_rst_bank		gpd4
#define s6e3fa3_dsim_lcd_rst_pin			0
#define s6e3fa3_dsim_lcd_rst_func		0x1

#define s6e3fa3_dsim_lcd_power_bank	gpr2
#define s6e3fa3_dsim_lcd_power_pin		3
#define s6e3fa3_dsim_lcd_power_func		0x1

#define s6e3fa0_dsim_lcd_rst_bank		gpd4
#define s6e3fa0_dsim_lcd_rst_pin			0
#define s6e3fa0_dsim_lcd_rst_func		0x1

#define s6e3fa0_dsim_lcd_power_bank	gpr2
#define s6e3fa0_dsim_lcd_power_pin		3
#define s6e3fa0_dsim_lcd_power_func		0x1

#define s6e8aa0_dsim_lcd_rst_bank		gpd4
#define s6e8aa0_dsim_lcd_rst_pin			0
#define s6e8aa0_dsim_lcd_rst_func		0x1

#define s6e8aa0_dsim_lcd_power_bank	gpr2
#define s6e8aa0_dsim_lcd_power_pin		3
#define s6e8aa0_dsim_lcd_power_func	0x1

#define s6e8fa0_dsim_lcd_rst_bank		gpd4
#define s6e8fa0_dsim_lcd_rst_pin			0
#define s6e8fa0_dsim_lcd_rst_func		0x1

#define s6e8fa0_dsim_lcd_power_bank	gpr2
#define s6e8fa0_dsim_lcd_power_pin		3
#define s6e8fa0_dsim_lcd_power_func		0x1

#define dsim1_lcd_rst_bank		gpd4
#define dsim1_lcd_rst_pin			2
#define dsim1_lcd_rst_func		0x1

#define dsim1_lcd_power_bank		gpr2
#define dsim1_lcd_power_pin		 	4
#define dsim1_lcd_power_func		0x1

#define fimc_2p8_reset_bank		gpf3
#define fimc_2p8_reset_pin		2
#define fimc_2p8_reset_func		0x1

#define fimc_3h7_reset_bank		gpf3
#define fimc_3h7_reset_pin		2
#define fimc_3h7_reset_func		0x1


#define fimc_6b2_reset_bank		gpf3
#define fimc_6b2_reset_pin		3
#define fimc_6b2_reset_func		0x1


#define hdmi_hpd_bank	gpa3
#define hdmi_hpd_pin	0
#define hdmi_hpd_func	0xf

#define hdmi_ls_en_bank		gpf4
#define hdmi_ls_en_pin		5
#define hdmi_ls_en_func		0x1

#define hdmi_dcdc_en_bank		gpf4
#define hdmi_dcdc_en_pin		6
#define hdmi_dcdc_en_func		0x1

#define audio_rt5647_detect_bank		gpa0
#define audio_rt5647_detect_pin		5
#define audio_rt5647_detect_func		0

#define audio_wm5110_irq_bank		gpa0
#define audio_wm5110_irq_pin		5
#define audio_wm5110_irq_func		0

#define audio_wm5110_cs_bank		gpd8
#define audio_wm5110_cs_pin		1
#define audio_wm5110_cs_func		2


#define fpc1020_irq_bank		gpa1
#define fpc1020_irq_pin		6
#define fpc1020_irq_func		0

#define fpc1020_reset_bank		gpv7
#define fpc1020_reset_pin		4
#define fpc1020_reset_func		1

#define fpc1020_cs_bank		gpv7
#define fpc1020_cs_pin		1
#define fpc1020_cs_func		2

#define tsu6721_muic_bank	gpa0
#define tsu6721_muic_pin	 	0
#define tsu6721_muic_func	0xf

#endif	/*Endif __CODEGEN_GPIO_H*/
