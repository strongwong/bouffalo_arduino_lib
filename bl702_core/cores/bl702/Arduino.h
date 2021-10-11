/*
###############################################################################
# Copyright (c) 2018, PulseRain Technology LLC 
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License (LGPL) as 
# published by the Free Software Foundation, either version 3 of the License,
# or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful, but 
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
# or FITNESS FOR A PARTICULAR PURPOSE.  
# See the GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
###############################################################################
*/

#ifndef _ARDUINO_H
#define _ARDUINO_H

#include "common_type.h"

/* bouffalo lab api */
#include "misc.h"
#include "gcc.h"

#include "bl702_config.h"
#include "bflb_platform.h"
#include "drv_device.h"
#include "drv_list.h"

// #include "hal_adc.h"
// #include "hal_cam.h"
// #include "hal_clock.h"
// #include "hal_dac.h"
// #include "hal_dma.h"
// #include "hal_emac.h"
// #include "hal_flash.h"
#include "hal_drv/inc/hal_gpio.h"
// #include "hal_i2c.h"
// #include "hal_i2s.h"
// #include "hal_keyscan.h"
// #include "hal_mjpeg.h"
// #include "hal_mtimer.h"
// #include "hal_power.h"
// #include "hal_pwm.h"
// #include "hal_sec_aes.h"
// #include "hal_sec_dsa.h"
// #include "hal_sec_ecdsa.h"
// #include "hal_sec_hash.h"
// #include "hal_spi.h"
// #include "hal_timer.h"
// #include "hal_uart.h"
// #include "hal_usb.h"

//============================================================================================
// Constant definition
//============================================================================================

constexpr uint8_t INPUT  = 0;
constexpr uint8_t OUTPUT = 1;

constexpr uint8_t HIGH   = 1;
constexpr uint8_t LOW    = 0;

constexpr uint8_t CHANGE  = 2;
constexpr uint8_t RISING  = 3;
constexpr uint8_t FALLING = 4;

#ifndef false
#define false 0
#endif

#ifndef true
#define true 1
#endif

void setup(void);
void loop(void);


#endif
