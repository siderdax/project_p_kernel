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

#ifndef __CODEGEN_PINCTRL_H
#define __CODEGEN_PINCTRL_H

#define hdmi_hdmi_hpd_pinctrl	pinctrl_0
#define hdmi_hdmi_hpd_pin	"gpa3-0"
#define hdmi_hdmi_hpd_con	3
#define hdmi_hdmi_hpd_pull	PIN_PULL_DOWN
#define hdmi_hdmi_hpd_drv	0

#define hdmi_ext_hpd_pinctrl	pinctrl_0
#define hdmi_ext_hpd_pin	"gpa3-0"
#define hdmi_ext_hpd_con	PIN_CON_INT
#define hdmi_ext_hpd_pull	PIN_PULL_DOWN
#define hdmi_ext_hpd_drv	0

#define hdmi_cec_pinctrl	pinctrl_0
#define hdmi_cec_pin 	"gpa3-1"
#define hdmi_cec_con 	3
#define hdmi_cec_pull	PIN_PULL_DOWN
#define hdmi_cec_drv	0
#if 1 //bcm4358 BT
#define cfg_hostwake_pinctrl		pinctrl_0
#define cfg_hostwake_pin 		"gpa2-6"
#define cfg_hostwake_con 		PIN_CON_INPUT
#define cfg_hostwake_pull 		PIN_PULL_RESERVE	
#define cfg_hostwake_con_pdn 	PIN_PDN_INPUT
#define cfg_hostwake_pull_pdn	PIN_PULL_NONE
#else //bcm4339
#define cfg_hostwake_pinctrl		pinctrl_0
#define cfg_hostwake_pin 		"gpa1-4"
#define cfg_hostwake_con 		PIN_CON_INPUT
#define cfg_hostwake_pull 		PIN_PULL_RESERVE	
#define cfg_hostwake_con_pdn 	PIN_PDN_INPUT
#define cfg_hostwake_pull_pdn	PIN_PULL_NONE
#endif


#if  1 //BT
#define cfg_btwake_pinctrl	pinctrl_3 
#define cfg_btwake_pin		"gpf2-3"
#define cfg_btwake_con 		PIN_CON_OUTPUT
#define cfg_btwake_pull		PIN_PULL_DOWN
#define cfg_btwake_con_pdn 	PIN_PDN_OUT0
#define cfg_btwake_pull_pdn	PIN_PULL_DOWN
#else
#define cfg_btwake_pinctrl	pinctrl_3 
#define cfg_btwake_pin		"gpf4-3"
#define cfg_btwake_con 		PIN_CON_OUTPUT
#define cfg_btwake_pull		PIN_PULL_DOWN
#define cfg_btwake_con_pdn 	PIN_PDN_OUT0
#define cfg_btwake_pull_pdn	PIN_PULL_DOWN
#endif


#if 1 // WLAN4358
#define cfg_wlanhostwake_pinctrl	pinctrl_0
#define cfg_wlanhostwake_pin 	"gpa2-3"
#define cfg_wlanhostwake_con	PIN_CON_INPUT
#define cfg_wlanhostwake_pull	PIN_PULL_NONE
#else
#define cfg_wlanhostwake_pinctrl	pinctrl_0
#define cfg_wlanhostwake_pin 	"gpa2-2"
#define cfg_wlanhostwake_con	PIN_CON_INPUT
#define cfg_wlanhostwake_pull	PIN_PULL_NONE
#endif

#define dwc3_b_sess_pinctrl	pinctrl_0 
#define dwc3_b_sess_pin		"gpa1-0"
#define dwc3_b_sess_con		PIN_CON_INT
#define dwc3_b_sess_pull		PIN_PULL_NONE

#define dwc3_id_pinctrl	pinctrl_0
#define dwc3_id_pin 		"gpa1-1"
#define dwc3_id_con		PIN_CON_INT
#define dwc3_id_pull		PIN_PULL_NONE

#define pmic_irq_pinctrl 	pinctrl_0
#define pmic_irq_pin		"gpa0-2"
#define pmic_irq_pull		PIN_PULL_UP
#define pmic_irq_drv		3
#define pmic_irq_pin_bank	gpa0
#define pmic_irq_pin_num	2

#define sub_pmic_irq_pinctrl 	pinctrl_0
#define sub_pmic_irq_pin		"gpa3-7"
#define sub_pmic_irq_pull		PIN_PULL_UP
#define sub_pmic_irq_drv		3
#define sub_pmic_irq_pin_bank	gpa3
#define sub_pmic_irq_pin_num	7


#define fpc_irq_pinctrl 	pinctrl_0
#define fpc_irq_pin		    "gpa1-6"
#define fpc_irq_con		    PIN_CON_INT
#define fpc_irq_pull		PIN_PULL_NONE
#define fpc_irq_drv		    0

#define gpio_keys_pullup_pinctrl	pinctrl_0
#define gpio_keys_pullup_pin0	"gpa2-0"
#define gpio_keys_pullup_pin1	"gpa2-1"
#define gpio_keys_pullup_pin2	"gpa2-5"
#define gpio_keys_pullup_con		PIN_CON_INT
#define gpio_keys_pullup_pull		PIN_PULL_UP
#define gpio_keys_pullup_drv		0

#define attn_irq_pinctrl	pinctrl_0 
#define attn_irq_pin 		"gpa0-3"
#define attn_irq_con		PIN_CON_INT
#define attn_irq_pull		PIN_PULL_UP
#define attn_irq_drv		0


#define attn_input_pinctrl	pinctrl_0
#define attn_input_pin	"gpa0-3"
#define attn_input_con	PIN_CON_INPUT
#define attn_input_pull	PIN_PULL_DOWN
#define attn_input_drv	0

#define touchpanel_fts_irq_pinctrl	pinctrl_0
#define touchpanel_fts_irq_pin	"gpd4-3"
#define touchpanel_fts_irq_con	PIN_CON_INT
#define touchpanel_fts_irq_pull	PIN_PULL_UP
#define touchpanel_fts_irq_drv	0

#define touchpanel_irq_pinctrl	pinctrl_0
#define touchpanel_irq_pin		"gpa0-3"
#define touchpanel_irq_con		PIN_CON_INT
#define touchpanel_irq_pull		PIN_PULL_UP
#define touchpanel_irq_drv		0

#define touchpanel_input_pinctrl	pinctrl_0
#define touchpanel_input_pin		"gpa0-3"
#define touchpanel_input_con		PIN_CON_INPUT
#define touchpanel_input_pull		PIN_PULL_DOWN
#define touchpanel_input_drv		0

#define muic_int_pinctrl		pinctrl_0
#define muic_int_pin			"gpa0-0"
#define muic_int_con			PIN_CON_INT
#define muic_int_pull			PIN_PULL_NONE
#define muic_int_drv			0

#define chg_irq_pinctrl	pinctrl_0
#define chg_irq_pin		"gpa0-0"
#define chg_irq_con		PIN_CON_INT
#define chg_irq_pull		PIN_PULL_NONE
#define chg_irq_drv		0

#define bat_alrt_pinctrl 	pinctrl_0
#define bat_alrt_pin		"gpa3-4"
#define bat_alrt_con		PIN_CON_INT
#define bat_alrt_pull		PIN_PULL_NONE
#define bat_alrt_drv		0

#define lp_irq_pinctrl		pinctrl_0 
#define lp_irq_pin		"gpa3-6"
#define lp_irq_con		PIN_CON_INT
#define lp_irq_pull		PIN_PULL_NONE
#define lp_irq_drv		0

#define gyro_irq_pinctrl	pinctrl_0
#define gyro_irq_pin		"gpa1-2"
#define gyro_irq_con		PIN_CON_INT
#define gyro_irq_pull		PIN_PULL_NONE
#define gyro_irq_drv		0

#define codec_irq_pinctrl	pinctrl_0
#define codec_irq_pin	"gpa0-5"
#define codec_irq_con	PIN_CON_INT
#define codec_irq_pull	PIN_PULL_UP

#define codec_irq_idle_pinctrl	pinctrl_0 
#define codec_irq_idle_pin	"gpa0-5"
#define codec_irq_idle_con	PIN_CON_INPUT
#define codec_irq_idle_pull	PIN_PULL_UP

#define i2s0_bus_pinctrl	pinctrl_1
#define i2s0_bus_pin0	"gpz0-0"
#define i2s0_bus_pin1	"gpz0-1"
#define i2s0_bus_pin2	"gpz0-2"
#define i2s0_bus_pin3	"gpz0-3"
#define i2s0_bus_pin4	"gpz0-4"
#define i2s0_bus_pin5	"gpz0-5"
#define i2s0_bus_pin6	"gpz0-6"
#define i2s0_bus_con		2
#define i2s0_bus_pull		PIN_PULL_DOWN
#define i2s0_bus_drv		0

#define i2s0_bus_idle_pinctrl	pinctrl_1 
#define i2s0_bus_idle_pin0	"gpz0-0"
#define i2s0_bus_idle_pin1	"gpz0-1"
#define i2s0_bus_idle_pin2	"gpz0-2"
#define i2s0_bus_idle_pin3	"gpz0-3"
#define i2s0_bus_idle_pin4	"gpz0-4"
#define i2s0_bus_idle_pin5	"gpz0-5"
#define i2s0_bus_idle_pin6	"gpz0-6"
#define i2s0_bus_idle_con	PIN_CON_INPUT
#define i2s0_bus_idle_pull	PIN_PULL_DOWN
#define i2s0_bus_idle_drv	0

#define rt5647_i2s_bus_sd0_pinctrl	pinctrl_1 
#define rt5647_i2s_bus_sd0_pin0	"gpz0-0"
#define rt5647_i2s_bus_sd0_pin1	"gpz0-1"
#define rt5647_i2s_bus_sd0_pin2	"gpz0-2"
#define rt5647_i2s_bus_sd0_pin3	"gpz0-3"
#define rt5647_i2s_bus_sd0_pin4	"gpz0-4"
#define rt5647_i2s_bus_sd0_con	2
#define rt5647_i2s_bus_sd0_pull	PIN_PULL_DOWN
#define rt5647_i2s_bus_sd0_drv		0

#define rt5647_i2s_bus_idle_sd0_pinctrl	pinctrl_1 
#define rt5647_i2s_bus_idle_sd0_pin0	"gpz0-0"
#define rt5647_i2s_bus_idle_sd0_pin1	"gpz0-1"
#define rt5647_i2s_bus_idle_sd0_pin2	"gpz0-2"
#define rt5647_i2s_bus_idle_sd0_pin3	"gpz0-3"
#define rt5647_i2s_bus_idle_sd0_pin4	"gpz0-4"
#define rt5647_i2s_bus_idle_sd0_con	PIN_CON_INPUT
#define rt5647_i2s_bus_idle_sd0_pull	PIN_PULL_DOWN
#define rt5647_i2s_bus_idle_sd0_drv	0

#define wm5110_i2s_bus_sd0_pinctrl	pinctrl_1 
#define wm5110_i2s_bus_sd0_pin0	"gpz0-0"
#define wm5110_i2s_bus_sd0_pin1	"gpz0-1"
#define wm5110_i2s_bus_sd0_pin2	"gpz0-2"
#define wm5110_i2s_bus_sd0_pin3	"gpz0-3"
#define wm5110_i2s_bus_sd0_pin4	"gpz0-4"
#define wm5110_i2s_bus_sd0_con	2
#define wm5110_i2s_bus_sd0_pull	PIN_PULL_DOWN
#define wm5110_i2s_bus_sd0_drv		0

#define wm5110_i2s_bus_idle_sd0_pinctrl	pinctrl_1 
#define wm5110_i2s_bus_idle_sd0_pin0	"gpz0-0"
#define wm5110_i2s_bus_idle_sd0_pin1	"gpz0-1"
#define wm5110_i2s_bus_idle_sd0_pin2	"gpz0-2"
#define wm5110_i2s_bus_idle_sd0_pin3	"gpz0-3"
#define wm5110_i2s_bus_idle_sd0_pin4	"gpz0-4"
#define wm5110_i2s_bus_idle_sd0_con	PIN_CON_INPUT
#define wm5110_i2s_bus_idle_sd0_pull	PIN_PULL_DOWN
#define wm5110_i2s_bus_idle_sd0_drv	0

#define pcm0_bus_pinctrl	pinctrl_1
#define pcm0_bus_pin0	"gpz1-0"
#define pcm0_bus_pin1	"gpz1-1"
#define pcm0_bus_pin2	"gpz1-2"
#define pcm0_bus_pin3	"gpz1-3"
#define pcm0_bus_con	3
#define pcm0_bus_pull	PIN_PULL_DOWN
#define pcm0_bus_drv	0

#define uart_aud_bus_pinctrl		pinctrl_1
#define uart_aud_bus_pin0		"gpz1-3"
#define uart_aud_bus_pin1		"gpz1-2"
#define uart_aud_bus_pin2		"gpz1-1"
#define uart_aud_bus_pin3		"gpz1-0"
#define uart_aud_bus_con		2
#define uart_aud_bus_pull		PIN_PULL_NONE
#define uart_aud_bus_con_pdn	PIN_PDN_INPUT
#define uart_aud_bus_pull_pdn	PIN_PULL_DOWN

#define uart_aud_bus_idle_pinctrl               pinctrl_1
#define uart_aud_bus_idle_pin           "gpz1-3"
#define uart_aud_bus_idle_con           PIN_CON_OUTPUT
#define uart_aud_bus_idle_val           1
#define uart_aud_bus_idle_pull          PIN_PULL_NONE
#define uart_aud_bus_idle_con_pdn       PIN_PDN_OUT1
#define uart_aud_bus_idle_pud_pdn       PIN_PULL_UP

#define uart_aud_bus_lpm_pinctrl                pinctrl_1
#define uart_aud_bus_lpm_pin            "gpz1-3"
#define uart_aud_bus_lpm_con            PIN_CON_OUTPUT
#define uart_aud_bus_lpm_val            1
#define uart_aud_bus_lpm_pull           PIN_PULL_NONE
#define uart_aud_bus_lpm_con_pdn        PIN_PDN_OUT1
#define uart_aud_bus_lpm_pull_pdn       PIN_PULL_UP

#define uart_bt_active_pinctrl		pinctrl_1
#define uart_bt_active_pin0		"gpz1-3"
#define uart_bt_active_pin1		"gpz1-2"
#define uart_bt_active_pin2		"gpz1-1"
#define uart_bt_active_pin3		"gpz1-0"
#define uart_bt_active_con		2
#define uart_bt_active_pull		PIN_PULL_UP

#define uart_bt_output_sleep_pinctrl		pinctrl_1
#define uart_bt_output_sleep_pin0		"gpz1-3"
#define uart_bt_output_sleep_pin3		"gpz1-0"
#define uart_bt_output_sleep_con_pdn		1
#define uart_bt_output_sleep_pull_pdn		PIN_PULL_UP

#define uart_bt_input_sleep_pinctrl		pinctrl_1
#define uart_bt_input_sleep_pin1			"gpz1-2"
#define uart_bt_input_sleep_pin2			"gpz1-1"
#define uart_bt_input_sleep_con_pdn		2
#define uart_bt_input_sleep_pull_pdn		PIN_PULL_UP


#define dbg_if_pinctrl		pinctrl_1 
#define dbg_if_pin		"gpz0-6"
#define dbg_if_con		PIN_CON_OUTPUT
#define dbg_if_pull		PIN_PULL_NONE
#define dbg_if_drv		0
#define dbg_if_con_pdn	PIN_PDN_OUT1
#define dbg_if_pull_pdn	PIN_PULL_NONE
#define dbg_if_val		1

#define hs_i2c0_bus_pinctrl	pinctrl_2
#define hs_i2c0_bus_pin0		"gpd2-1"
#define hs_i2c0_bus_pin1		"gpd2-0"
#define hs_i2c0_bus_con		2
#define hs_i2c0_bus_pull		PIN_PULL_UP
#define hs_i2c0_bus_drv		0

#define hs_i2c1_bus_pinctrl	pinctrl_2
#define hs_i2c1_bus_pin0		"gpd2-3"
#define hs_i2c1_bus_pin1		"gpd2-2"
#define hs_i2c1_bus_con		2
#define hs_i2c1_bus_pull		PIN_PULL_UP
#define hs_i2c1_bus_drv		0

#define hs_i2c2_bus_pinctrl	pinctrl_2 
#define hs_i2c2_bus_pin0		"gpd0-3"
#define hs_i2c2_bus_pin1		"gpd0-2"
#define hs_i2c2_bus_con		3
#define hs_i2c2_bus_pull		PIN_PULL_UP
#define hs_i2c2_bus_drv		0

#define hs_i2c3_bus_pinctrl	pinctrl_2
#define hs_i2c3_bus_pin0		"gpd1-3"
#define hs_i2c3_bus_pin1		"gpd1-2"
#define hs_i2c3_bus_con		3
#define hs_i2c3_bus_pull		PIN_PULL_UP
#define hs_i2c3_bus_drv		0

#define hs_i2c4_bus_pinctrl 	pinctrl_2
#define hs_i2c4_bus_pin0		"gpg3-1"
#define hs_i2c4_bus_pin1		"gpg3-0"
#define hs_i2c4_bus_con		2
#define hs_i2c4_bus_pull		PIN_PULL_UP
#define hs_i2c4_bus_drv		0

#define hs_i2c4_apm_pinctrl 	pinctrl_2
#define hs_i2c4_apm_pin0	"gpg3-1"
#define hs_i2c4_apm_pin1	"gpg3-0"
#define hs_i2c4_apm_con		PIN_CON_OUTPUT
#define hs_i2c4_apm_pull		PIN_PULL_UP
#define hs_i2c4_apm_drv		0

#define hs_i2c5_bus_pinctrl 	pinctrl_2
#define hs_i2c5_bus_pin0		"gpg3-3"
#define hs_i2c5_bus_pin1		"gpg3-2"
#define hs_i2c5_bus_con		2
#define hs_i2c5_bus_pull		PIN_PULL_UP
#define hs_i2c5_bus_drv		0

#define hs_i2c8_bus_pinctrl	pinctrl_2
#define hs_i2c8_bus_pin0		"gpd5-3"
#define hs_i2c8_bus_pin1		"gpd5-2"
#define hs_i2c8_bus_con		3
#define hs_i2c8_bus_pull		PIN_PULL_UP
#define hs_i2c8_bus_drv		0

#define hs_i2c9_bus_pinctrl	pinctrl_2
#define hs_i2c9_bus_pin0		"gpd2-7"
#define hs_i2c9_bus_pin1		"gpd2-6"
#define hs_i2c9_bus_con		2
#define hs_i2c9_bus_pull		PIN_PULL_UP
#define hs_i2c9_bus_drv		0

#define hs_i2c10_bus_pinctrl 	pinctrl_2
#define hs_i2c10_bus_pin0	"gpb0-1"
#define hs_i2c10_bus_pin1	"gpb0-0"
#define hs_i2c10_bus_con	2
#define hs_i2c10_bus_pull	PIN_PULL_UP
#define hs_i2c10_bus_drv		0

#define hs_i2c11_bus_pinctrl	pinctrl_2 
#define hs_i2c11_bus_pin0	"gpb0-3"
#define hs_i2c11_bus_pin1	"gpb0-2"
#define hs_i2c11_bus_con	2
#define hs_i2c11_bus_pull	PIN_PULL_UP
#define hs_i2c11_bus_drv		0

#define uart_isp_bus_pinctrl	pinctrl_2
#define uart_isp_bus_pin0	"gpc0-7"
#define uart_isp_bus_pin1	"gpc0-6"
#define uart_isp_bus_pin2	"gpc1-1"
#define uart_isp_bus_pin3	"gpc1-0"
#define uart_isp_bus_con		3
#define uart_isp_bus_pull		PIN_PULL_NONE

#define uart0_bus_pinctrl		pinctrl_2
#if 0
#define uart0_bus_pin0		"gpd0-3"
#define uart0_bus_pin1		"gpd0-2"
#define uart0_bus_pin2		"gpd0-1"
#define uart0_bus_pin3		"gpd0-0"
#else //mv7420
#define uart0_bus_pin0		"gpd0-1"
#define uart0_bus_pin1		"gpd0-0"
#endif
#define uart0_bus_con		2
#define uart0_bus_pull		PIN_PULL_NONE

#define uart1_bus_pinctrl		pinctrl_2
#if 0
#define uart1_bus_pin0		"gpd1-3"
#define uart1_bus_pin1		"gpd1-2"
#define uart1_bus_pin2		"gpd1-1"
#define uart1_bus_pin3		"gpd1-0"
#else //mv7420
#define uart1_bus_pin0		"gpd1-1"
#define uart1_bus_pin1		"gpd1-0"
#endif

#define uart1_bus_con		2
#define uart1_bus_pull		PIN_PULL_NONE

#define uart2_bus_pinctrl 	pinctrl_2
#define uart2_bus_pin0		"gpd1-5"
#define uart2_bus_pin1		"gpd1-4"
#define uart2_bus_con		2
#define uart2_bus_pull		PIN_PULL_NONE

#define uart3_bus_pinctrl		pinctrl_2 
#define uart3_bus_pin0		"gpd5-1"
#define uart3_bus_pin1		"gpd5-0"
#define uart3_bus_con		3
#define uart3_bus_pull		PIN_PULL_NONE    

#define spi0_bus_pinctrl		pinctrl_2
#define spi0_bus_pin0		"gpd8-0"
#define spi0_bus_pin1		"gpd6-0"
#define spi0_bus_pin2		"gpd6-1"
#define spi0_bus_con			2
#define spi0_bus_pull			PIN_PULL_UP
#define spi0_bus_drv			0

#define spi0_cs_func_pinctrl	pinctrl_2
#define spi0_cs_func_pin		"gpd8-1"
#define spi0_cs_func_con		2
#define spi0_cs_func_pull		PIN_PULL_UP
#define spi0_cs_func_drv		0

#define spi0_cs_pinctrl	pinctrl_2
#define spi0_cs_pin		"gpd8-1"
#define spi0_cs_con		PIN_CON_OUTPUT
#define spi0_cs_pull		PIN_PULL_UP
#define spi0_cs_drv		0

#define spi1_bus_pinctrl	pinctrl_2
#define spi1_bus_pin0	"gpd6-2"
#define spi1_bus_pin1	"gpd6-4"
#define spi1_bus_pin2	"gpd6-5"
#define spi1_bus_con		2
#define spi1_bus_pull		PIN_PULL_UP
#define spi1_bus_drv		0

#define spi1_cs_func_pinctrl	pinctrl_2
#define spi1_cs_func_pin		"gpd6-3"
#define spi1_cs_func_con		2
#define spi1_cs_func_pull		PIN_PULL_UP
#define spi1_cs_func_drv		0

#define spi1_cs_pinctrl	pinctrl_2
#define spi1_cs_pin		"gpd6-3"
#define spi1_cs_con		PIN_CON_OUTPUT
#define spi1_cs_pull		PIN_PULL_UP
#define spi1_cs_drv		0

#define spi2_bus_pinctrl	pinctrl_2 
#define spi2_bus_pin0	"gpd5-0"
#define spi2_bus_pin1	"gpd5-2"
#define spi2_bus_pin2	"gpd5-3"
#define spi2_bus_con		2
#define spi2_bus_pull		PIN_PULL_UP
#define spi2_bus_drv		0

#define spi2_cs_func_pinctrl	pinctrl_2
#define spi2_cs_func_pin		"gpd5-1"
#define spi2_cs_func_con		2
#define spi2_cs_func_pull		PIN_PULL_UP
#define spi2_cs_func_drv		0

#define spi2_cs_pinctrl	pinctrl_2 
#define spi2_cs_pin		"gpd5-1"
#define spi2_cs_con		PIN_CON_OUTPUT
#define spi2_cs_pull		PIN_PULL_UP
#define spi2_cs_drv		0

#define disp_teson_pinctrl	pinctrl_2
#define disp_teson_pin		"gpg0-1"
#define disp_teson_con		2

#define disp_tesoff_pinctrl	pinctrl_2 
#define disp_tesoff_pin		"gpg0-1"
#define disp_tesoff_con		PIN_CON_INPUT

#define disp1_teson_pinctrl	pinctrl_3
#define disp1_teson_pin		"gpf5-0"
#define disp1_teson_con		2

#define disp1_tesoff_pinctrl	pinctrl_3
#define disp1_tesoff_pin		"gpf5-0"
#define disp1_tesoff_con		PIN_CON_INPUT

#define fimc_is_uart_pinctrl	pinctrl_2 
#define fimc_is_uart_pin0		"gpc1-1"
#define fimc_is_uart_pin1		"gpc0-7"
#define fimc_is_uart_con		2
#define fimc_is_uart_pull		PIN_PULL_NONE
#define fimc_is_uart_drv		0

#define fimc_is_mclk0_in_pinctrl	pinctrl_2 
#define fimc_is_mclk0_in_pin		"gpd7-0"
#define fimc_is_mclk0_in_con		PIN_CON_INPUT
#define fimc_is_mclk0_in_pull		PIN_PULL_NONE
#define fimc_is_mclk0_in_drv		2

#define fimc_is_mclk1_in_pinctrl	pinctrl_2
#define fimc_is_mclk1_in_pin		"gpd7-1"
#define fimc_is_mclk1_in_con		PIN_CON_INPUT
#define fimc_is_mclk1_in_pull		PIN_PULL_NONE
#define fimc_is_mclk1_in_drv		2

#define fimc_is_mclk2_in_pinctrl	pinctrl_2
#define fimc_is_mclk2_in_pin		"gpd7-2"
#define fimc_is_mclk2_in_con		PIN_CON_INPUT
#define fimc_is_mclk2_in_pull		PIN_PULL_NONE
#define fimc_is_mclk2_in_drv		2


#define fimc_is_mclk0_out_pinctrl	pinctrl_2 
#define fimc_is_mclk0_out_pin		"gpd7-0"
#define fimc_is_mclk0_out_con		PIN_CON_INPUT
#define fimc_is_mclk0_out_pull		PIN_PULL_DOWN
#define fimc_is_mclk0_out_drv		2

#define fimc_is_mclk1_out_pinctrl	pinctrl_2
#define fimc_is_mclk1_out_pin		"gpd7-1"
#define fimc_is_mclk1_out_con		PIN_CON_INPUT
#define fimc_is_mclk1_out_pull		PIN_PULL_DOWN
#define fimc_is_mclk1_out_drv		2

#define fimc_is_mclk2_out_pinctrl	pinctrl_2
#define fimc_is_mclk2_out_pin		"gpd7-2"
#define fimc_is_mclk2_out_con		PIN_CON_INPUT
#define fimc_is_mclk2_out_pull		PIN_PULL_DOWN
#define fimc_is_mclk2_out_drv		2


#define fimc_is_mclk0_fn_pinctrl	pinctrl_2
#define fimc_is_mclk0_fn_pin		"gpd7-0"
#define fimc_is_mclk0_fn_con		2
#define fimc_is_mclk0_fn_pull		PIN_PULL_NONE
#define fimc_is_mclk0_fn_drv		2

#define fimc_is_mclk1_fn_pinctrl	pinctrl_2 
#define fimc_is_mclk1_fn_pin		"gpd7-1"
#define fimc_is_mclk1_fn_con		2
#define fimc_is_mclk1_fn_pull		PIN_PULL_NONE
#define fimc_is_mclk1_fn_drv		2

#define fimc_is_mclk2_fn_pinctrl	pinctrl_2
#define fimc_is_mclk2_fn_pin		"gpd7-2"
#define fimc_is_mclk2_fn_con		2
#define fimc_is_mclk2_fn_pull		PIN_PULL_NONE
#define fimc_is_mclk2_fn_drv		2

#define fimc_is_i2c0_pinctrl	pinctrl_2
#define fimc_is_i2c0_pin0		"gpc2-1"
#define fimc_is_i2c0_pin1		"gpc2-0"
#define fimc_is_i2c0_con		2
#define fimc_is_i2c0_pull		PIN_PULL_NONE
#define fimc_is_i2c0_drv		0

#define fimc_is_i2c1_pinctrl	pinctrl_2
#define fimc_is_i2c1_pin0		"gpc2-3"
#define fimc_is_i2c1_pin1		"gpc2-2"
#define fimc_is_i2c1_con		2
#define fimc_is_i2c1_pull		PIN_PULL_NONE
#define fimc_is_i2c1_drv		0

#define fimc_is_i2c2_pinctrl	pinctrl_2
#define fimc_is_i2c2_pin0		"gpc2-5"
#define fimc_is_i2c2_pin1		"gpc2-4"
#define fimc_is_i2c2_con		2
#define fimc_is_i2c2_pull		PIN_PULL_NONE
#define fimc_is_i2c2_drv		0

#define fimc_is_vision_pinctrl	pinctrl_3
#define fimc_is_vision_pin0	"gpf5-3"
#define fimc_is_vision_pin1	"gpf5-2"
#define fimc_is_vision_con	PIN_CON_INPUT
#define fimc_is_vision_pull	PIN_PULL_NONE
#define fimc_is_vision_drv	0

#define fimc_is_flash_pinctrl	pinctrl_2
#define fimc_is_flash_pin0	"gpc0-0"
#define fimc_is_flash_pin1	"gpc0-2"
#define fimc_is_flash_con		2
#define fimc_is_flash_pull		PIN_PULL_NONE
#define fimc_is_flash_drv		0

#define fimc_is_spi_pin0_pinctrl	pinctrl_2 
#define fimc_is_spi_pin0_pin0		"gpc3-3"
#define fimc_is_spi_pin0_pin1		"gpc3-2"
#define fimc_is_spi_pin0_pin2		"gpc3-0"
#define fimc_is_spi_pin0_con		2
#define fimc_is_spi_pin0_pull		PIN_PULL_NONE
#define fimc_is_spi_pin0_drv		0

#define fimc_is_spi_pin1_pinctrl	pinctrl_2
#define fimc_is_spi_pin1_pin0		"gpc3-7"
#define fimc_is_spi_pin1_pin1		"gpc3-6"
#define fimc_is_spi_pin1_pin2		"gpc3-4"
#define fimc_is_spi_pin1_con		2
#define fimc_is_spi_pin1_pull		PIN_PULL_NONE
#define fimc_is_spi_pin1_drv		0

#define i2s1_bus_pinctrl	pinctrl_2
#define i2s1_bus_pin0	"gpd4-0"
#define i2s1_bus_pin1	"gpd4-1"
#define i2s1_bus_pin2	"gpd4-2"
#define i2s1_bus_pin3	"gpd4-3"
#define i2s1_bus_pin4	"gpd4-4"
#define i2s1_bus_con		2
#define i2s1_bus_pull		PIN_PULL_DOWN
#define i2s1_bus_drv		0

#define pcm1_bus_pinctrl	pinctrl_2
#define pcm1_bus_pin0	"gpd4-0"
#define pcm1_bus_pin1	"gpd4-1"
#define pcm1_bus_pin2	"gpd4-2"
#define pcm1_bus_pin3	"gpd4-3"
#define pcm1_bus_pin4	"gpd4-4"
#define pcm1_bus_con	3
#define pcm1_bus_pull	PIN_PULL_DOWN
#define pcm1_bus_drv	0

#define spdif_bus_pinctrl	pinctrl_2
#define spdif_bus_pin0	"gpd4-3"
#define spdif_bus_pin1	"gpd4-4"
#define spdif_bus_con	4
#define spdif_bus_pull	PIN_PULL_DOWN
#define spdif_bus_drv	0

#define pcie0_clkreq_pinctrl		pinctrl_3
#define pcie0_clkreq_pin			"gpv6-1"
#define pcie0_clkreq_con			2
#define pcie0_clkreq_pull			PIN_PULL_NONE
#define pcie0_clkreq_drv			3
#define pcie0_clkreq_con_pdn		PIN_PDN_PREV
#define pcie0_clkreq_pull_pdn	PIN_PULL_NONE

#define pcie0_perst_pinctrl		pinctrl_3 
#define pcie0_perst_pin			"gph1-2"
#define pcie0_perst_con			PIN_CON_OUTPUT
#define pcie0_perst_drv			3
#define pcie0_perst_con_pdn		PIN_PDN_PREV

#define pcie0_reset_pinctrl		pinctrl_3
#define pcie0_reset_pin			"gph1-3"
#define pcie0_reset_con			2
#define pcie0_reset_pull			PIN_PULL_DOWN
#define pcie0_reset_con_pdn		PIN_PDN_PREV
#define pcie0_reset_pull_pdn		PIN_PULL_DOWN

#define pcie0_reset_clear_pinctrl		pinctrl_3
#define pcie0_reset_clear_pin			"gph1-3"
#define pcie0_reset_clear_con		2
#define pcie0_reset_clear_pull		PIN_PULL_UP
#define pcie0_reset_clear_con_pdn	PIN_PDN_PREV
#define pcie0_reset_clear_pull_pdn	PIN_PULL_UP

#define pcie1_clkreq_pinctrl		pinctrl_3
#define pcie1_clkreq_pin			"gpf0-6"
#define pcie1_clkreq_con			2
#define pcie1_clkreq_pull			PIN_PULL_NONE
#define pcie1_clkreq_drv			3
#define pcie1_clkreq_con_pdn		PIN_PDN_PREV
#define pcie1_clkreq_pull_pdn	PIN_PULL_NONE

#define pcie1_perst_pinctrl		pinctrl_3
#define pcie1_perst_pin			"gpg2-0"
#define pcie1_perst_con			PIN_CON_OUTPUT
#define pcie1_perst_drv			3
#define pcie1_perst_con_pdn		PIN_PDN_PREV

#define pcie1_reset_pinctrl 		pinctrl_3
#define pcie1_reset_pin			"gpg2-2"
#define pcie1_reset_con			2
#define pcie1_reset_pull			PIN_PULL_DOWN
#define pcie1_reset_con_pdn			PIN_PDN_PREV
#define pcie1_reset_pull_pdn		PIN_PULL_DOWN

#define pcie1_reset_clear_pinctrl		pinctrl_3
#define pcie1_reset_clear_pin			"gpg2-2"
#define pcie1_reset_clear_con		2
#define pcie1_reset_clear_pull		PIN_PULL_UP
#define pcie1_reset_clear_con_pdn	PIN_PDN_PREV
#define pcie1_reset_clear_pull_pdn	PIN_PULL_UP

#define wifi_pcie_clkreq_pinctrl		pinctrl_3
#define wifi_pcie_clkreq_pin			"gpf0-6"
#define wifi_pcie_clkreq_con			2
#define wifi_pcie_clkreq_pull			PIN_PULL_NONE
#define wifi_pcie_clkreq_drv			3
#define wifi_pcie_clkreq_con_pdn		PIN_PDN_PREV
#define wifi_pcie_clkreq_pull_pdn	PIN_PULL_NONE

#define wifi_pcie_perst_pinctrl		pinctrl_3
#define wifi_pcie_perst_pin			"gpg2-0"
#define wifi_pcie_perst_con			PIN_CON_OUTPUT
#define wifi_pcie_perst_drv			3
#define wifi_pcie_perst_con_pdn		PIN_PDN_PREV

#define wifi_pcie_reset_pinctrl 		pinctrl_3
#define wifi_pcie_reset_pin			"gpg2-2"
#define wifi_pcie_reset_con			2
#define wifi_pcie_reset_pull			PIN_PULL_DOWN
#define wifi_pcie_reset_con_pdn			PIN_PDN_PREV
#define wifi_pcie_reset_pull_pdn		PIN_PULL_DOWN

#define wifi_pcie_reset_clear_pinctrl		pinctrl_3
#define wifi_pcie_reset_clear_pin			"gpg2-2"
#define wifi_pcie_reset_clear_con		2
#define wifi_pcie_reset_clear_pull		PIN_PULL_UP
#define wifi_pcie_reset_clear_con_pdn	PIN_PDN_PREV
#define wifi_pcie_reset_clear_pull_pdn	PIN_PULL_UP

#define spi5_bus_pinctrl	pinctrl_3 
#define spi5_bus_pin0	"gpf2-3"
#define spi5_bus_pin1	"gpf2-2"
#define spi5_bus_pin2	"gpf2-0"
#define spi5_bus_con		2
#define spi5_bus_pull		PIN_PULL_UP
#define spi5_bus_drv		0

#define spi5_cs_func_pinctrl	pinctrl_3
#define spi5_cs_func_pin		"gpf2-1"
#define spi5_cs_func_con		2
#define spi5_cs_func_pull		PIN_PULL_UP
#define spi5_cs_func_drv		0

#define spi5_cs_pinctrl	pinctrl_3
#define spi5_cs_pin		"gpf2-1"
#define spi5_cs_con		PIN_CON_OUTPUT
#define spi5_cs_pull		PIN_PULL_UP
#define spi5_cs_drv		0

#define ufs_refclk_out_pinctrl	pinctrl_3 
#define ufs_refclk_out_pin	"gpg2-4"
#define ufs_refclk_out_con	2
#define ufs_refclk_out_pull	PIN_PULL_NONE
#define ufs_refclk_out_drv	2

#define ufs_rst_n_pinctrl	pinctrl_3
#define ufs_rst_n_pin		"gph1-5"
#define ufs_rst_n_con	2
#define ufs_rst_n_pull	PIN_PULL_UP
#define ufs_rst_n_drv	0

#if 1
#define cfg_en_pinctrl	pinctrl_3
#define cfg_en_pin		"gpf1-0"
#define cfg_en_con		PIN_CON_OUTPUT
#define cfg_en_pull		PIN_PULL_DOWN
#define cfg_en_con_pdn	PIN_PDN_PREV
#define cfg_en_pull_pdn	PIN_PULL_DOWN
#else //BCM4339
#define cfg_en_pinctrl  pinctrl_3
#define cfg_en_pin              "gpf4-2"
#define cfg_en_con              PIN_CON_OUTPUT
#define cfg_en_pull             PIN_PULL_UP
#define cfg_en_con_pdn  PIN_PDN_PREV
#define cfg_en_pull_pdn PIN_PULL_DOWN
#endif

#define g3d_dvs_pinctrl	pinctrl_3
#define g3d_dvs_pin		"gpg1-1"
#define g3d_dvs_con		2

#define pm_wrsti_pinctrl	pinctrl_3
#define pm_wrsti_pin		"gpf0-2"
#define pm_wrsti_con		3

#if 1
#define cfg_wlanen_pinctrl	pinctrl_3 
#define cfg_wlanen_pin		"gpf0-4"
#define cfg_wlanen_con		PIN_CON_OUTPUT
#define cfg_wlanen_drv		3
#define cfg_wlanen_con_pdn	PIN_PDN_PREV
#else //bcm4339
#define cfg_wlanen_pinctrl	pinctrl_3 
#define cfg_wlanen_pin		"gpf4-1"
#define cfg_wlanen_con		PIN_CON_OUTPUT
#define cfg_wlanen_drv		3
#define cfg_wlanen_con_pdn	PIN_PDN_PREV
#endif

#define cfg_wlanen_modem_pinctrl	pinctrl_3
#define cfg_wlanen_modem_pin		"gpf3-1"
#define cfg_wlanen_modem_con		PIN_CON_OUTPUT
#define cfg_wlanen_modem_drv		3
#define cfg_wlanen_modem_con_pdn	PIN_PDN_PREV

#define codec_reset_pinctrl	pinctrl_3
#define codec_reset_pin		"gpf2-1"
#define codec_reset_con		PIN_CON_OUTPUT
#define codec_reset_pull		PIN_PULL_DOWN
#define codec_reset_con_pdn	PIN_PDN_PREV
#define codec_reset_pull_pdn	PIN_PULL_DOWN
#define codec_reset_val		0

#define codec_ldoena_pinctrl		pinctrl_3
#define codec_ldoena_pin			"gpf2-2"
#define codec_ldoena_con		PIN_CON_OUTPUT
#define codec_ldoena_pull		PIN_PULL_DOWN
#define codec_ldoena_con_pdn	PIN_PDN_PREV
#define codec_ldoena_pull_pdn	PIN_PULL_DOWN
#define codec_ldoena_val			1

#define vbus_en_pinctrl	pinctrl_3
#define vbus_en_pin		"gpf1-2"
#define vbus_en_con		PIN_CON_INT
#define vbus_en_pull		PIN_PULL_NONE

#define hs_i2c6_bus_pinctrl	pinctrl_4
#define hs_i2c6_bus_pin0		"gpj0-1"
#define hs_i2c6_bus_pin1		"gpj0-0"
#define hs_i2c6_bus_con		2
#define hs_i2c6_bus_pull		PIN_PULL_UP
#define hs_i2c6_bus_drv		0

#define hs_i2c7_bus_pinctrl 	pinctrl_5
#define hs_i2c7_bus_pin0		"gpj1-1"
#define hs_i2c7_bus_pin1		"gpj1-0"
#define hs_i2c7_bus_con		2
#define hs_i2c7_bus_pull		PIN_PULL_UP
#define hs_i2c7_bus_drv		0

#define spi3_bus_pinctrl 	pinctrl_6
#define spi3_bus_pin0	"gpg4-3"
#define spi3_bus_pin1	"gpg4-2"
#define spi3_bus_pin2	"gpg4-0"
#define spi3_bus_con		2
#define spi3_bus_pull		PIN_PULL_UP
#define spi3_bus_drv		0

#define spi3_cs_func_pinctrl	pinctrl_6
#define spi3_cs_func_pin		"gpg4-1"
#define spi3_cs_func_con		2
#define spi3_cs_func_pull		PIN_PULL_UP
#define spi3_cs_func_drv		0

#define spi3_cs_pinctrl	pinctrl_6
#define spi3_cs_pin		"gpg4-1"
#define spi3_cs_con		PIN_CON_OUTPUT
#define spi3_cs_pull		PIN_PULL_UP
#define spi3_cs_drv		0

#define spi4_bus_pinctrl	pinctrl_7
#define spi4_bus_pin0	"gpv7-3"
#define spi4_bus_pin1	"gpv7-2"
#define spi4_bus_pin2	"gpv7-0"
#define spi4_bus_con		2
#define spi4_bus_pull		PIN_PULL_UP
#define spi4_bus_drv		0

#define spi4_cs_func_pinctrl	pinctrl_7
#define spi4_cs_func_pin		"gpv7-1"
#define spi4_cs_func_con		2
#define spi4_cs_func_pull		PIN_PULL_UP
#define spi4_cs_func_drv		0

#define spi4_cs_pinctrl	pinctrl_7
#define spi4_cs_pin		"gpv7-1"
#define spi4_cs_con		PIN_CON_OUTPUT
#define spi4_cs_pull		PIN_PULL_UP
#define spi4_cs_drv		0

#define sd2_clk_pinctrl	pinctrl_8
#define sd2_clk_pin		"gpr4-0"
#define sd2_clk_con		2
#define sd2_clk_pull		PIN_PULL_NONE
#define sd2_clk_drv		2

#define sd2_cmd_pinctrl	pinctrl_8
#define sd2_cmd_pin		"gpr4-1"
#define sd2_cmd_con		2
#define sd2_cmd_pull		PIN_PULL_NONE
#define sd2_cmd_drv		2

#define sd2_cd_pinctrl	pinctrl_8
#define sd2_cd_pin		"gpr4-2"
#define sd2_cd_con		2
#define sd2_cd_pull		PIN_PULL_NONE
#define sd2_cd_drv		2

#define sd2_bus1_pinctrl	pinctrl_8
#define sd2_bus1_pin		"gpr4-3"
#define sd2_bus1_con	2
#define sd2_bus1_pull	PIN_PULL_NONE
#define sd2_bus1_drv	2

#define sd2_bus4_pinctrl	pinctrl_8
#define sd2_bus4_pin0	"gpr4-4"
#define sd2_bus4_pin1	"gpr4-5"
#define sd2_bus4_pin2	"gpr4-6"
#define sd2_bus4_con	2
#define sd2_bus4_pull	PIN_PULL_NONE
#define sd2_bus4_drv	2

#define sd0_clk_pinctrl	pinctrl_9
#define sd0_clk_pin		"gpr0-0"
#define sd0_clk_con		2
#define sd0_clk_pull		PIN_PULL_NONE
#define sd0_clk_drv		4

#define sd0_cmd_pinctrl	pinctrl_9
#define sd0_cmd_pin		"gpr0-1"
#define sd0_cmd_con		2
#define sd0_cmd_pull		PIN_PULL_UP
#define sd0_cmd_drv		4

#define sd0_rdqs_pinctrl	pinctrl_9
#define sd0_rdqs_pin		"gpr0-2"
#define sd0_rdqs_con		2
#define sd0_rdqs_pull	PIN_PULL_DOWN
#define sd0_rdqs_drv		4

#define sd0_qrdy_pinctrl	pinctrl_9
#define sd0_qrdy_pin		"gpr0-3"
#define sd0_qrdy_con	2
#define sd0_qrdy_pull	PIN_PULL_DOWN
#define sd0_qrdy_drv		4

#define sd0_bus1_pinctrl	pinctrl_9
#define sd0_bus1_pin		"gpr1-0"
#define sd0_bus1_con	2
#define sd0_bus1_pull	PIN_PULL_UP
#define sd0_bus1_drv	4

#define sd0_bus4_pinctrl	pinctrl_9
#define sd0_bus4_pin0	"gpr1-1"
#define sd0_bus4_pin1	"gpr1-2"
#define sd0_bus4_pin2	"gpr1-3"
#define sd0_bus4_con	2
#define sd0_bus4_pull	PIN_PULL_UP
#define sd0_bus4_drv	4

#define sd0_bus8_pinctrl	pinctrl_9
#define sd0_bus8_pin0	"gpr1-4"
#define sd0_bus8_pin1	"gpr1-5"
#define sd0_bus8_pin2	"gpr1-6"
#define sd0_bus8_pin3	"gpr1-7"
#define sd0_bus8_con	2
#define sd0_bus8_pull	PIN_PULL_UP
#define sd0_bus8_drv	4

#define sd1_clk_pinctrl	pinctrl_9
#define sd1_clk_pin		"gpr2-0"
#define sd1_clk_con		2
#define sd1_clk_pull		PIN_PULL_NONE
#define sd1_clk_drv		4

#define sd1_cmd_pinctrl	pinctrl_9
#define sd1_cmd_pin		"gpr2-1"
#define sd1_cmd_con		2
#define sd1_cmd_pull		PIN_PULL_UP
#define sd1_cmd_drv		4

#define sd1_bus1_pinctrl	pinctrl_9
#define sd1_bus1_pin		"gpr3-0"
#define sd1_bus1_con	2
#define sd1_bus1_pull	PIN_PULL_UP
#define sd1_bus1_drv	4

#define sd1_bus4_pinctrl	pinctrl_9
#define sd1_bus4_pin0	"gpr3-1"
#define sd1_bus4_pin1	"gpr3-2"
#define sd1_bus4_pin2	"gpr3-3"
#define sd1_bus4_con	2
#define sd1_bus4_pull	PIN_PULL_UP
#define sd1_bus4_drv	4

#define sd1_bus8_pinctrl	pinctrl_9
#define sd1_bus8_pin0	"gpr3-4"
#define sd1_bus8_pin1	"gpr3-5"
#define sd1_bus8_pin2	"gpr3-6"
#define sd1_bus8_pin3	"gpr3-7"
#define sd1_bus8_con	2
#define sd1_bus8_pull	PIN_PULL_UP
#define sd1_bus8_drv	4

#define pcie_eint_pinctrl		pinctrl_9
#define pcie_eint_pin			"gpr2-0"
#define pcie_eint_con			PIN_CON_INT
#define pcie_eint_pull		PIN_PULL_NONE
#define pcie_eint_con_pdn	PIN_PDN_PREV
#define pcie_eint_pull_pdn	PIN_PULL_NONE

#define pcie0_eint_pinctrl		pinctrl_9
#define pcie0_eint_pin		"gpr2-1"
#define pcie0_eint_con		PIN_CON_INPUT
#define pcie0_eint_pull		PIN_PULL_NONE
#define pcie0_eint_con_pdn	PIN_PDN_PREV
#define pcie0_eint_pull_pdn	PIN_PULL_NONE

#endif	/*Endif __CODEGEN_PINCTRL_H*/
