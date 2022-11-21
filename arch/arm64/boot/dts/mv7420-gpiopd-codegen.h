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
#ifndef __CODEGEN_GPIOPD_H
#define __CODEGEN_GPIOPD_H

#define gpa0_0_pdn_mode		PIN_CON_INT
#define gpa0_0_pdn_pull		PIN_PULL_NONE
#define gpa0_0_pdn_val		0

#define gpa0_1_pdn_mode		PIN_CON_INT
#define gpa0_1_pdn_pull		PIN_PULL_NONE
#define gpa0_1_pdn_val		0

#define gpa0_2_pdn_mode		PIN_CON_INPUT
#define gpa0_2_pdn_pull		PIN_PULL_UP
#define gpa0_2_pdn_val		0

#define gpa0_3_pdn_mode		PIN_CON_INT
#define gpa0_3_pdn_pull		PIN_PULL_UP
#define gpa0_3_pdn_val		0

#define gpa0_4_pdn_mode		PIN_CON_INT
#define gpa0_4_pdn_pull		PIN_PULL_NONE
#define gpa0_4_pdn_val		0

#define gpa0_5_pdn_mode		PIN_CON_INPUT
#define gpa0_5_pdn_pull		PIN_PULL_UP
#define gpa0_5_pdn_val		0

#define gpa0_6_pdn_mode		PIN_CON_OUTPUT
#define gpa0_6_pdn_pull		PIN_PULL_DOWN
#define gpa0_6_pdn_val		0

#define gpa0_7_pdn_mode		PIN_CON_INT
#define gpa0_7_pdn_pull		PIN_PULL_NONE
#define gpa0_7_pdn_val		0

#define gpa1_0_pdn_mode		PIN_CON_INT
#define gpa1_0_pdn_pull		PIN_PULL_NONE
#define gpa1_0_pdn_val		0

#define gpa1_1_pdn_mode		PIN_CON_INT
#define gpa1_1_pdn_pull		PIN_PULL_NONE
#define gpa1_1_pdn_val		0

#define gpa1_2_pdn_mode		PIN_CON_INT
#define gpa1_2_pdn_pull		PIN_PULL_NONE
#define gpa1_2_pdn_val		0

#define gpa1_3_pdn_mode		PIN_CON_INT
#define gpa1_3_pdn_pull		PIN_PULL_NONE
#define gpa1_3_pdn_val		0

#define gpa1_4_pdn_mode		PIN_CON_INT
#define gpa1_4_pdn_pull		PIN_PULL_NONE
#define gpa1_4_pdn_val		0

#define gpa1_5_pdn_mode		PIN_CON_INT
#define gpa1_5_pdn_pull		PIN_PULL_NONE
#define gpa1_5_pdn_val		0

#define gpa1_6_pdn_mode		PIN_CON_INT
#define gpa1_6_pdn_pull		PIN_PULL_NONE
#define gpa1_6_pdn_val		0

#define gpa1_7_pdn_mode		PIN_CON_INT
#define gpa1_7_pdn_pull		PIN_PULL_NONE
#define gpa1_7_pdn_val		0

#define gpa2_0_pdn_mode		PIN_CON_INT
#define gpa2_0_pdn_pull		PIN_PULL_UP
#define gpa2_0_pdn_val		0

#define gpa2_1_pdn_mode		PIN_CON_INT
#define gpa2_1_pdn_pull		PIN_PULL_UP
#define gpa2_1_pdn_val		0

#define gpa2_2_pdn_mode		PIN_CON_INT
#define gpa2_2_pdn_pull		PIN_PULL_NONE
#define gpa2_2_pdn_val		0

#define gpa2_3_pdn_mode		PIN_CON_INPUT
#define gpa2_3_pdn_pull		PIN_PULL_NONE
#define gpa2_3_pdn_val		0

#define gpa2_4_pdn_mode		PIN_CON_INPUT
#define gpa2_4_pdn_pull		PIN_PULL_NONE
#define gpa2_4_pdn_val		0

#define gpa2_5_pdn_mode		PIN_CON_INPUT
#define gpa2_5_pdn_pull		PIN_PULL_NONE
#define gpa2_5_pdn_val		0

#define gpa2_6_pdn_mode		PIN_CON_INPUT
#define gpa2_6_pdn_pull		PIN_PULL_RESERVE
#define gpa2_6_pdn_val		0

#define gpa2_7_pdn_mode		PIN_CON_INT
#define gpa2_7_pdn_pull		PIN_PULL_UP
#define gpa2_7_pdn_val		0

#define gpa3_0_pdn_mode		PIN_CON_INT
#define gpa3_0_pdn_pull		PIN_PULL_DOWN
#define gpa3_0_pdn_val		0

#define gpa3_1_pdn_mode		3
#define gpa3_1_pdn_pull		PIN_PULL_DOWN
#define gpa3_1_pdn_val		0

#define gpa3_2_pdn_mode		PIN_CON_INPUT
#define gpa3_2_pdn_pull		PIN_PULL_NONE
#define gpa3_2_pdn_val		0

#define gpa3_3_pdn_mode		PIN_CON_INPUT
#define gpa3_3_pdn_pull		PIN_PULL_NONE
#define gpa3_3_pdn_val		0

#define gpa3_4_pdn_mode		PIN_CON_INT
#define gpa3_4_pdn_pull		PIN_PULL_NONE
#define gpa3_4_pdn_val		0

#define gpa3_5_pdn_mode		PIN_CON_INT
#define gpa3_5_pdn_pull		PIN_PULL_NONE
#define gpa3_5_pdn_val		0

#define gpa3_6_pdn_mode		PIN_CON_INT
#define gpa3_6_pdn_pull		PIN_PULL_NONE
#define gpa3_6_pdn_val		0

#define gpa3_7_pdn_mode		PIN_CON_INT
#define gpa3_7_pdn_pull		PIN_PULL_NONE
#define gpa3_7_pdn_val		0


#define gpz0_0_pdn_mode	PIN_PDN_INPUT
#define gpz0_0_pdn_pull		PIN_PULL_DOWN
#define gpz0_1_pdn_mode	PIN_PDN_INPUT
#define gpz0_1_pdn_pull		PIN_PULL_DOWN
#define gpz0_2_pdn_mode	PIN_PDN_INPUT
#define gpz0_2_pdn_pull		PIN_PULL_DOWN
#define gpz0_3_pdn_mode	PIN_PDN_INPUT
#define gpz0_3_pdn_pull		PIN_PULL_DOWN
#define gpz0_4_pdn_mode	PIN_PDN_INPUT
#define gpz0_4_pdn_pull		PIN_PULL_DOWN
#define gpz0_5_pdn_mode	PIN_PDN_INPUT
#define gpz0_5_pdn_pull		PIN_PULL_DOWN
#define gpz0_6_pdn_mode	PIN_PDN_INPUT
#define gpz0_6_pdn_pull		PIN_PULL_DOWN

#define gpz1_0_pdn_mode	PIN_PDN_INPUT
#define gpz1_0_pdn_pull		PIN_PULL_DOWN
#define gpz1_1_pdn_mode	PIN_PDN_INPUT
#define gpz1_1_pdn_pull		PIN_PULL_DOWN
#define gpz1_2_pdn_mode	PIN_PDN_INPUT
#define gpz1_2_pdn_pull		PIN_PULL_DOWN
#define gpz1_3_pdn_mode	PIN_PDN_INPUT
#define gpz1_3_pdn_pull		PIN_PULL_DOWN

#define gpb0_0_pdn_mode	PIN_PDN_INPUT
#define gpb0_0_pdn_pull		PIN_PULL_DOWN
#define gpb0_1_pdn_mode	PIN_PDN_INPUT
#define gpb0_1_pdn_pull		PIN_PULL_DOWN
#define gpb0_2_pdn_mode	PIN_PDN_INPUT
#define gpb0_2_pdn_pull		PIN_PULL_DOWN
#define gpb0_3_pdn_mode	PIN_PDN_INPUT
#define gpb0_3_pdn_pull		PIN_PULL_DOWN
#define gpb0_4_pdn_mode	PIN_PDN_INPUT
#define gpb0_4_pdn_pull		PIN_PULL_DOWN

#define gpc0_0_pdn_mode	PIN_PDN_OUT0
#define gpc0_0_pdn_pull		PIN_PULL_DOWN
#define gpc0_1_pdn_mode	PIN_PDN_OUT0
#define gpc0_1_pdn_pull		PIN_PULL_DOWN
#define gpc0_2_pdn_mode	PIN_PDN_OUT0
#define gpc0_2_pdn_pull		PIN_PULL_DOWN
#define gpc0_3_pdn_mode	PIN_PDN_OUT0
#define gpc0_3_pdn_pull		PIN_PULL_DOWN
#define gpc0_4_pdn_mode	PIN_PDN_INPUT
#define gpc0_4_pdn_pull		PIN_PULL_NONE
#define gpc0_5_pdn_mode	PIN_PDN_INPUT
#define gpc0_5_pdn_pull		PIN_PULL_NONE
#define gpc0_6_pdn_mode	PIN_PDN_OUT0
#define gpc0_6_pdn_pull		PIN_PULL_DOWN
#define gpc0_7_pdn_mode	PIN_PDN_INPUT
#define gpc0_7_pdn_pull		PIN_PULL_DOWN

#define gpc1_0_pdn_mode	PIN_PDN_OUT0
#define gpc1_0_pdn_pull		PIN_PULL_DOWN
#define gpc1_1_pdn_mode	PIN_PDN_INPUT
#define gpc1_1_pdn_pull		PIN_PULL_DOWN

#define gpc2_0_pdn_mode	PIN_PDN_INPUT
#define gpc2_0_pdn_pull		PIN_PULL_NONE
#define gpc2_1_pdn_mode	PIN_PDN_INPUT
#define gpc2_1_pdn_pull		PIN_PULL_NONE
#define gpc2_2_pdn_mode	PIN_PDN_INPUT
#define gpc2_2_pdn_pull		PIN_PULL_NONE
#define gpc2_3_pdn_mode	PIN_PDN_INPUT
#define gpc2_3_pdn_pull		PIN_PULL_NONE
#define gpc2_4_pdn_mode	PIN_PDN_INPUT
#define gpc2_4_pdn_pull		PIN_PULL_NONE
#define gpc2_5_pdn_mode	PIN_PDN_INPUT
#define gpc2_5_pdn_pull		PIN_PULL_NONE

#define gpc3_0_pdn_mode	PIN_PDN_INPUT
#define gpc3_0_pdn_pull		PIN_PULL_DOWN
#define gpc3_1_pdn_mode	PIN_PDN_INPUT
#define gpc3_1_pdn_pull		PIN_PULL_DOWN
#define gpc3_2_pdn_mode	PIN_PDN_INPUT
#define gpc3_2_pdn_pull		PIN_PULL_DOWN
#define gpc3_3_pdn_mode	PIN_PDN_INPUT
#define gpc3_3_pdn_pull		PIN_PULL_DOWN
#define gpc3_4_pdn_mode	PIN_PDN_INPUT
#define gpc3_4_pdn_pull		PIN_PULL_DOWN
#define gpc3_5_pdn_mode	PIN_PDN_INPUT
#define gpc3_5_pdn_pull		PIN_PULL_DOWN
#define gpc3_6_pdn_mode	PIN_PDN_INPUT
#define gpc3_6_pdn_pull		PIN_PULL_DOWN
#define gpc3_7_pdn_mode	PIN_PDN_INPUT
#define gpc3_7_pdn_pull		PIN_PULL_DOWN

#define gpd0_0_pdn_mode	PIN_PDN_INPUT
#define gpd0_0_pdn_pull		PIN_PULL_DOWN
#define gpd0_1_pdn_mode	PIN_PDN_INPUT
#define gpd0_1_pdn_pull		PIN_PULL_DOWN
#define gpd0_2_pdn_mode	PIN_PDN_INPUT
#define gpd0_2_pdn_pull		PIN_PULL_DOWN
#define gpd0_3_pdn_mode	PIN_PDN_INPUT
#define gpd0_3_pdn_pull		PIN_PULL_DOWN

#define gpd1_0_pdn_mode	PIN_PDN_INPUT
#define gpd1_0_pdn_pull		PIN_PULL_DOWN
#define gpd1_1_pdn_mode	PIN_PDN_INPUT
#define gpd1_1_pdn_pull		PIN_PULL_DOWN
#define gpd1_2_pdn_mode	PIN_PDN_INPUT
#define gpd1_2_pdn_pull		PIN_PULL_DOWN
#define gpd1_3_pdn_mode	PIN_PDN_INPUT
#define gpd1_3_pdn_pull		PIN_PULL_DOWN
#define gpd1_4_pdn_mode	PIN_PDN_INPUT
#define gpd1_4_pdn_pull		PIN_PULL_DOWN
#define gpd1_5_pdn_mode	PIN_PDN_INPUT
#define gpd1_5_pdn_pull		PIN_PULL_DOWN

#define gpd2_0_pdn_mode	PIN_PDN_INPUT
#define gpd2_0_pdn_pull		PIN_PULL_NONE
#define gpd2_1_pdn_mode	PIN_PDN_INPUT
#define gpd2_1_pdn_pull		PIN_PULL_NONE
#define gpd2_2_pdn_mode	PIN_PDN_INPUT
#define gpd2_2_pdn_pull		PIN_PULL_NONE
#define gpd2_3_pdn_mode	PIN_PDN_INPUT
#define gpd2_3_pdn_pull		PIN_PULL_NONE
#define gpd2_4_pdn_mode	PIN_PDN_INPUT
#define gpd2_4_pdn_pull		PIN_PULL_DOWN
#define gpd2_5_pdn_mode	PIN_PDN_INPUT
#define gpd2_5_pdn_pull		PIN_PULL_DOWN
#define gpd2_6_pdn_mode	PIN_PDN_INPUT
#define gpd2_6_pdn_pull		PIN_PULL_NONE
#define gpd2_7_pdn_mode	PIN_PDN_INPUT
#define gpd2_7_pdn_pull		PIN_PULL_NONE

#define gpd4_0_pdn_mode	PIN_PDN_INPUT
#define gpd4_0_pdn_pull		PIN_PULL_DOWN
#define gpd4_1_pdn_mode	PIN_PDN_INPUT
#define gpd4_1_pdn_pull		PIN_PULL_DOWN
#define gpd4_2_pdn_mode	PIN_PDN_INPUT
#define gpd4_2_pdn_pull		PIN_PULL_DOWN
#define gpd4_3_pdn_mode	PIN_PDN_INPUT
#define gpd4_3_pdn_pull		PIN_PULL_DOWN
#define gpd4_4_pdn_mode	PIN_PDN_INPUT
#define gpd4_4_pdn_pull		PIN_PULL_DOWN

#define gpd5_0_pdn_mode	PIN_PDN_INPUT
#define gpd5_0_pdn_pull		PIN_PULL_NONE
#define gpd5_1_pdn_mode	PIN_PDN_INPUT
#define gpd5_1_pdn_pull		PIN_PULL_NONE
#define gpd5_2_pdn_mode	PIN_PDN_INPUT
#define gpd5_2_pdn_pull		PIN_PULL_NONE
#define gpd5_3_pdn_mode	PIN_PDN_INPUT
#define gpd5_3_pdn_pull		PIN_PULL_NONE

#define gpd6_0_pdn_mode	PIN_PDN_INPUT
#define gpd6_0_pdn_pull		PIN_PULL_DOWN
#define gpd6_1_pdn_mode	PIN_PDN_INPUT
#define gpd6_1_pdn_pull		PIN_PULL_DOWN
#define gpd6_2_pdn_mode	PIN_PDN_OUT0
#define gpd6_2_pdn_pull		PIN_PULL_DOWN
#define gpd6_3_pdn_mode	PIN_PDN_OUT0
#define gpd6_3_pdn_pull		PIN_PULL_DOWN
#define gpd6_4_pdn_mode	PIN_PDN_INPUT
#define gpd6_4_pdn_pull		PIN_PULL_DOWN
#define gpd6_5_pdn_mode	PIN_PDN_INPUT
#define gpd6_5_pdn_pull		PIN_PULL_DOWN

#define gpd7_0_pdn_mode	PIN_PDN_INPUT
#define gpd7_0_pdn_pull		PIN_PULL_DOWN
#define gpd7_1_pdn_mode	PIN_PDN_INPUT
#define gpd7_1_pdn_pull		PIN_PULL_DOWN
#define gpd7_2_pdn_mode	PIN_PDN_INPUT
#define gpd7_2_pdn_pull		PIN_PULL_DOWN

#define gpd8_0_pdn_mode	PIN_PDN_INPUT
#define gpd8_0_pdn_pull		PIN_PULL_DOWN
#define gpd8_1_pdn_mode	PIN_PDN_INPUT
#define gpd8_1_pdn_pull		PIN_PULL_DOWN

#define gpg0_0_pdn_mode	PIN_PDN_INPUT
#define gpg0_0_pdn_pull		PIN_PULL_DOWN
#define gpg0_1_pdn_mode	PIN_PDN_INPUT
#define gpg0_1_pdn_pull		PIN_PULL_DOWN

#define gpg3_0_pdn_mode	PIN_PDN_INPUT
#define gpg3_0_pdn_pull		PIN_PULL_NONE
#define gpg3_1_pdn_mode	PIN_PDN_INPUT
#define gpg3_1_pdn_pull		PIN_PULL_NONE
#define gpg3_2_pdn_mode	PIN_PDN_INPUT
#define gpg3_2_pdn_pull		PIN_PULL_NONE
#define gpg3_3_pdn_mode	PIN_PDN_INPUT
#define gpg3_3_pdn_pull		PIN_PULL_NONE

#define gpf0_0_pdn_mode	PIN_PDN_INPUT
#define gpf0_0_pdn_pull		PIN_PULL_DOWN
#define gpf0_1_pdn_mode	PIN_PDN_INPUT
#define gpf0_1_pdn_pull		PIN_PULL_DOWN
#define gpf0_2_pdn_mode	PIN_PDN_PREV
#define gpf0_2_pdn_pull		PIN_PULL_NONE
#define gpf0_3_pdn_mode	PIN_PDN_INPUT
#define gpf0_3_pdn_pull		PIN_PULL_DOWN
#define gpf0_4_pdn_mode	PIN_PDN_PREV
#define gpf0_4_pdn_pull		PIN_PULL_NONE
#define gpf0_5_pdn_mode	PIN_PDN_INPUT
#define gpf0_5_pdn_pull		PIN_PULL_DOWN
#define gpf0_6_pdn_mode	PIN_PDN_INPUT
#define gpf0_6_pdn_pull		PIN_PULL_NONE
#define gpf0_7_pdn_mode	PIN_PDN_INPUT
#define gpf0_7_pdn_pull		PIN_PULL_NONE

#define gpf1_0_pdn_mode	PIN_PDN_PREV
#define gpf1_0_pdn_pull		PIN_PULL_DOWN
#define gpf1_1_pdn_mode	PIN_PDN_INPUT
#define gpf1_1_pdn_pull		PIN_PULL_NONE
#define gpf1_2_pdn_mode	PIN_PDN_OUT1
#define gpf1_2_pdn_pull		PIN_PULL_NONE
#define gpf1_3_pdn_mode	PIN_PDN_INPUT
#define gpf1_3_pdn_pull		PIN_PULL_DOWN
#define gpf1_4_pdn_mode	PIN_PDN_OUT1
#define gpf1_4_pdn_pull		PIN_PULL_NONE
#define gpf1_5_pdn_mode	PIN_PDN_INPUT
#define gpf1_5_pdn_pull		PIN_PULL_DOWN
#define gpf1_6_pdn_mode	PIN_PDN_INPUT
#define gpf1_6_pdn_pull		PIN_PULL_DOWN
#define gpf1_7_pdn_mode	PIN_PDN_INPUT
#define gpf1_7_pdn_pull		PIN_PULL_DOWN

#define gpf2_0_pdn_mode	PIN_PDN_INPUT
#define gpf2_0_pdn_pull		PIN_PULL_DOWN
#define gpf2_1_pdn_mode	PIN_PDN_INPUT
#define gpf2_1_pdn_pull		PIN_PULL_DOWN
#define gpf2_2_pdn_mode	PIN_PDN_INPUT
#define gpf2_2_pdn_pull		PIN_PULL_DOWN
#define gpf2_3_pdn_mode	PIN_PDN_INPUT
#define gpf2_3_pdn_pull		PIN_PULL_DOWN

#define gpf3_0_pdn_mode	PIN_PDN_INPUT
#define gpf3_0_pdn_pull		PIN_PULL_DOWN
#define gpf3_1_pdn_mode	PIN_PDN_PREV
#define gpf3_1_pdn_pull		PIN_PULL_NONE
#define gpf3_2_pdn_mode	PIN_PDN_INPUT
#define gpf3_2_pdn_pull		PIN_PULL_DOWN
#define gpf3_3_pdn_mode	PIN_PDN_INPUT
#define gpf3_3_pdn_pull		PIN_PULL_DOWN
#define gpf3_4_pdn_mode	PIN_PDN_INPUT
#define gpf3_4_pdn_pull		PIN_PULL_DOWN

#define gpf4_0_pdn_mode	PIN_PDN_INPUT
#define gpf4_0_pdn_pull		PIN_PULL_DOWN
#define gpf4_1_pdn_mode	PIN_PDN_INPUT
#define gpf4_1_pdn_pull		PIN_PULL_DOWN
#define gpf4_2_pdn_mode	PIN_PDN_INPUT
#define gpf4_2_pdn_pull		PIN_PULL_DOWN
#define gpf4_3_pdn_mode	PIN_PDN_INPUT
#define gpf4_3_pdn_pull		PIN_PULL_DOWN
#define gpf4_4_pdn_mode	PIN_PDN_INPUT
#define gpf4_4_pdn_pull		PIN_PULL_DOWN
#define gpf4_5_pdn_mode	PIN_PDN_INPUT
#define gpf4_5_pdn_pull		PIN_PULL_DOWN
#define gpf4_6_pdn_mode	PIN_PDN_INPUT
#define gpf4_6_pdn_pull		PIN_PULL_DOWN
#define gpf4_7_pdn_mode	PIN_PDN_INPUT
#define gpf4_7_pdn_pull		PIN_PULL_DOWN

#define gpf5_0_pdn_mode	PIN_PDN_INPUT
#define gpf5_0_pdn_pull		PIN_PULL_DOWN
#define gpf5_1_pdn_mode	PIN_PDN_INPUT
#define gpf5_1_pdn_pull		PIN_PULL_DOWN
#define gpf5_2_pdn_mode	PIN_PDN_INPUT
#define gpf5_2_pdn_pull		PIN_PULL_DOWN
#define gpf5_3_pdn_mode	PIN_PDN_INPUT
#define gpf5_3_pdn_pull		PIN_PULL_DOWN
#define gpf5_4_pdn_mode	PIN_PDN_INPUT
#define gpf5_4_pdn_pull		PIN_PULL_DOWN
#define gpf5_5_pdn_mode	PIN_PDN_INPUT
#define gpf5_5_pdn_pull		PIN_PULL_DOWN
#define gpf5_6_pdn_mode	PIN_PDN_INPUT
#define gpf5_6_pdn_pull		PIN_PULL_DOWN
#define gpf5_7_pdn_mode	PIN_PDN_INPUT
#define gpf5_7_pdn_pull		PIN_PULL_DOWN

#define gpg1_0_pdn_mode	PIN_PDN_INPUT
#define gpg1_0_pdn_pull		PIN_PULL_DOWN
#define gpg1_1_pdn_mode	PIN_PDN_INPUT
#define gpg1_1_pdn_pull		PIN_PULL_DOWN
#define gpg1_2_pdn_mode	PIN_PDN_INPUT
#define gpg1_2_pdn_pull		PIN_PULL_DOWN
#define gpg1_3_pdn_mode	PIN_PDN_INPUT
#define gpg1_3_pdn_pull		PIN_PULL_DOWN
#define gpg1_4_pdn_mode	PIN_PDN_INPUT
#define gpg1_4_pdn_pull		PIN_PULL_DOWN

#define gpg2_0_pdn_mode	PIN_PDN_INPUT
#define gpg2_0_pdn_pull		PIN_PULL_DOWN
#define gpg2_1_pdn_mode	PIN_PDN_INPUT
#define gpg2_1_pdn_pull		PIN_PULL_DOWN
#define gpg2_2_pdn_mode	PIN_PDN_INPUT
#define gpg2_2_pdn_pull		PIN_PULL_DOWN
#define gpg2_3_pdn_mode	PIN_PDN_INPUT
#define gpg2_3_pdn_pull		PIN_PULL_DOWN
#define gpg2_4_pdn_mode	PIN_PDN_INPUT
#define gpg2_4_pdn_pull		PIN_PULL_DOWN

#define gph1_0_pdn_mode	PIN_PDN_INPUT
#define gph1_0_pdn_pull		PIN_PULL_NONE
#define gph1_1_pdn_mode	PIN_PDN_INPUT
#define gph1_1_pdn_pull		PIN_PULL_DOWN
#define gph1_2_pdn_mode	PIN_PDN_INPUT
#define gph1_2_pdn_pull		PIN_PULL_DOWN
#define gph1_3_pdn_mode	PIN_PDN_INPUT
#define gph1_3_pdn_pull		PIN_PULL_UP
#define gph1_4_pdn_mode	PIN_PDN_OUT1
#define gph1_4_pdn_pull		PIN_PULL_UP
#define gph1_5_pdn_mode	PIN_PDN_INPUT
#define gph1_5_pdn_pull		PIN_PULL_DOWN

#define gpv6_0_pdn_mode	PIN_PDN_INPUT
#define gpv6_0_pdn_pull		PIN_PULL_DOWN
#define gpv6_1_pdn_mode	PIN_PDN_INPUT
#define gpv6_1_pdn_pull		PIN_PULL_NONE
#define gpv6_2_pdn_mode	PIN_PDN_INPUT
#define gpv6_2_pdn_pull		PIN_PULL_DOWN

#define gpj0_0_pdn_mode	PIN_PDN_INPUT
#define gpj0_0_pdn_pull		PIN_PULL_NONE
#define gpj0_1_pdn_mode	PIN_PDN_INPUT
#define gpj0_1_pdn_pull		PIN_PULL_NONE
#define gpj0_2_pdn_mode	PIN_PDN_INPUT
#define gpj0_2_pdn_pull		PIN_PULL_DOWN

#define gpj1_0_pdn_mode	PIN_PDN_INPUT
#define gpj1_0_pdn_pull		PIN_PULL_NONE
#define gpj1_1_pdn_mode	PIN_PDN_INPUT
#define gpj1_1_pdn_pull		PIN_PULL_NONE
#define gpj1_2_pdn_mode	PIN_PDN_INPUT
#define gpj1_2_pdn_pull		PIN_PULL_DOWN

#define gpg4_0_pdn_mode	PIN_PDN_INPUT
#define gpg4_0_pdn_pull		PIN_PULL_DOWN
#define gpg4_1_pdn_mode	PIN_PDN_INPUT
#define gpg4_1_pdn_pull		PIN_PULL_DOWN
#define gpg4_2_pdn_mode	PIN_PDN_INPUT
#define gpg4_2_pdn_pull		PIN_PULL_DOWN
#define gpg4_3_pdn_mode	PIN_PDN_INPUT
#define gpg4_3_pdn_pull		PIN_PULL_DOWN

#define gpv7_0_pdn_mode	PIN_PDN_INPUT
#define gpv7_0_pdn_pull		PIN_PULL_DOWN
#define gpv7_1_pdn_mode	PIN_PDN_INPUT
#define gpv7_1_pdn_pull		PIN_PULL_DOWN
#define gpv7_2_pdn_mode	PIN_PDN_INPUT
#define gpv7_2_pdn_pull		PIN_PULL_DOWN
#define gpv7_3_pdn_mode	PIN_PDN_INPUT
#define gpv7_3_pdn_pull		PIN_PULL_DOWN
#define gpv7_4_pdn_mode	PIN_PDN_INPUT
#define gpv7_4_pdn_pull		PIN_PULL_DOWN

#define gpr4_0_pdn_mode	PIN_PDN_INPUT
#define gpr4_0_pdn_pull		PIN_PULL_DOWN
#define gpr4_1_pdn_mode	PIN_PDN_INPUT
#define gpr4_1_pdn_pull		PIN_PULL_NONE
#define gpr4_2_pdn_mode	PIN_PDN_INPUT
#define gpr4_2_pdn_pull		PIN_PULL_NONE
#define gpr4_3_pdn_mode	PIN_PDN_INPUT
#define gpr4_3_pdn_pull		PIN_PULL_NONE
#define gpr4_4_pdn_mode	PIN_PDN_INPUT
#define gpr4_4_pdn_pull		PIN_PULL_NONE
#define gpr4_5_pdn_mode	PIN_PDN_INPUT
#define gpr4_5_pdn_pull		PIN_PULL_NONE
#define gpr4_6_pdn_mode	PIN_PDN_INPUT
#define gpr4_6_pdn_pull		PIN_PULL_NONE

#define gpr0_0_pdn_mode	PIN_PDN_INPUT
#define gpr0_0_pdn_pull		PIN_PULL_DOWN
#define gpr0_1_pdn_mode	PIN_PDN_INPUT
#define gpr0_1_pdn_pull		PIN_PULL_NONE
#define gpr0_2_pdn_mode	PIN_PDN_INPUT
#define gpr0_2_pdn_pull		PIN_PULL_DOWN
#define gpr0_3_pdn_mode	PIN_PDN_INPUT
#define gpr0_3_pdn_pull		PIN_PULL_DOWN

#define gpr1_0_pdn_mode	PIN_PDN_INPUT
#define gpr1_0_pdn_pull		PIN_PULL_NONE
#define gpr1_1_pdn_mode	PIN_PDN_INPUT
#define gpr1_1_pdn_pull		PIN_PULL_NONE
#define gpr1_2_pdn_mode	PIN_PDN_INPUT
#define gpr1_2_pdn_pull		PIN_PULL_NONE
#define gpr1_3_pdn_mode	PIN_PDN_INPUT
#define gpr1_3_pdn_pull		PIN_PULL_NONE
#define gpr1_4_pdn_mode	PIN_PDN_INPUT
#define gpr1_4_pdn_pull		PIN_PULL_NONE
#define gpr1_5_pdn_mode	PIN_PDN_INPUT
#define gpr1_5_pdn_pull		PIN_PULL_NONE
#define gpr1_6_pdn_mode	PIN_PDN_INPUT
#define gpr1_6_pdn_pull		PIN_PULL_NONE
#define gpr1_7_pdn_mode	PIN_PDN_INPUT
#define gpr1_7_pdn_pull		PIN_PULL_NONE

#define gpr2_0_pdn_mode	PIN_PDN_INPUT
#define gpr2_0_pdn_pull		PIN_PULL_DOWN
#define gpr2_1_pdn_mode	PIN_PDN_INPUT
#define gpr2_1_pdn_pull		PIN_PULL_NONE
#define gpr2_2_pdn_mode	PIN_PDN_INPUT
#define gpr2_2_pdn_pull		PIN_PULL_DOWN
#define gpr2_3_pdn_mode	PIN_PDN_INPUT
#define gpr2_3_pdn_pull		PIN_PULL_DOWN
#define gpr2_4_pdn_mode	PIN_PDN_INPUT
#define gpr2_4_pdn_pull		PIN_PULL_DOWN

#define gpr3_0_pdn_mode	PIN_PDN_INPUT
#define gpr3_0_pdn_pull		PIN_PULL_NONE
#define gpr3_1_pdn_mode	PIN_PDN_INPUT
#define gpr3_1_pdn_pull		PIN_PULL_NONE
#define gpr3_2_pdn_mode	PIN_PDN_INPUT
#define gpr3_2_pdn_pull		PIN_PULL_NONE
#define gpr3_3_pdn_mode	PIN_PDN_INPUT
#define gpr3_3_pdn_pull		PIN_PULL_NONE
#define gpr3_4_pdn_mode	PIN_PDN_INPUT
#define gpr3_4_pdn_pull		PIN_PULL_DOWN
#define gpr3_5_pdn_mode	PIN_PDN_INPUT
#define gpr3_5_pdn_pull		PIN_PULL_DOWN
#define gpr3_6_pdn_mode	PIN_PDN_INPUT
#define gpr3_6_pdn_pull		PIN_PULL_DOWN
#define gpr3_7_pdn_mode	PIN_PDN_INPUT
#define gpr3_7_pdn_pull		PIN_PULL_DOWN

#endif	/*Endif __CODEGEN_GPIOPD_H*/
