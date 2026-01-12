/*
   CarrtPicoBinaryInfo.cpp - Binary meta info for the CARRT Pico executable

    Copyright (c) 2026 Igor Mikolic-Torreira.  All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <pico/binary_info.h>

#include "CarrtPicoDefines.h"

// Binary metadata for Pico executable

bi_decl( bi_1pin_with_name( PICO_DEFAULT_LED_PIN,
                            "On-board LED used for blinking and signaling" ) );

bi_decl( bi_2pins_with_names( CARRTPICO_SERIAL_LINK_UART_TX_PIN,
                              "uart1 (data) TX",
                              CARRTPICO_SERIAL_LINK_UART_RX_PIN,
                              "uart1 (data) RX" ) );

bi_decl( bi_2pins_with_names( CARRTPICO_I2C_SDA, "i2c0 SDA", CARRTPICO_I2C_SCL,
                              "i2c0 SCL" ) );
