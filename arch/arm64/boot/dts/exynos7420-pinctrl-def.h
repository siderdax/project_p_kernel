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
#ifndef __PINCTRL_DEF_H
#define __PINCTRL_DEF_H

#define PIN_CON_INPUT		0
#define PIN_CON_OUTPUT		1
#define PIN_CON_INT			0xf

#define PIN_PULL_NONE		0
#define PIN_PULL_DOWN		1
#define PIN_PULL_RESERVE 	2
#define PIN_PULL_UP			3

#define PIN_PDN_OUT0		0
#define PIN_PDN_OUT1		1
#define PIN_PDN_INPUT		2
#define PIN_PDN_PREV		3

#define PIN_IN(_pin, _pull)			\
	_pin {						\
		samsung,pins = #_pin;			\
		samsung,pin-function = <0>;		\
		samsung,pin-pud = <PIN_PULL_ ##_pull>;	\
	}

#define PIN_OUT(_pin)				\
	_pin {						\
		samsung,pins = #_pin;			\
		samsung,pin-function = <1>;		\
		samsung,pin-pud = <0>;			\
	}

#define PIN_OUT_SET(_pin, _val)			\
	_pin {						\
		samsung,pins = #_pin;			\
		samsung,pin-function = <1>;		\
		samsung,pin-pud = <0>;			\
		samsung,pin-val	= <_val>;		\
	}

#define PIN_ALIVE_SLP(_pin, _mode, _pull, _val)				\
	_pin {							\
		samsung,pins = #_pin;			\
		samsung,pin-function = <_mode>;		\
		samsung,pin-pud = <_pull>;			\
		samsung,pin-val	= <_val>;		\
	}

#define PIN_SLP(_pin, _mode, _pull)				\
	_pin {							\
		samsung,pins = #_pin;				\
		samsung,pin-con-pdn = <_mode>;	\
		samsung,pin-pud-pdn = <_pull>;	\
	}

#endif	/*Endif __PINCTRL_DEF_H*/