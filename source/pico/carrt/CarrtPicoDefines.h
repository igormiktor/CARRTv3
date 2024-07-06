/*
    CarrtPicoDefines.h - System defines for CARRT-Pico

    Copyright (c) 2024 Igor Mikolic-Torreira.  All right reserved.

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






#ifndef CarrtPicoDefines_h
#define CarrtPicoDefines_h

#include "pico.h"


// **************************************************************

// Define the signaling LED

#ifndef SIGNALING_LED
#define SIGNALING_LED               PICO_DEFAULT_LED_PIN
#endif  // SIGNALING_LED

#ifndef CARRTPICO_ONBOARD_LED    
#define CARRTPICO_ONBOARD_LED       PICO_DEFAULT_LED_PIN
#endif  // CARRTPICO_ONBOARD_LED

// **************************************************************

// I2C defines to Pico's I2C network
// Uses I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.

#ifndef CARRTPICO_I2C_PORT
#define CARRTPICO_I2C_PORT       i2c0
#endif  // CARRTPICO_I2C_PORT

#ifndef CARRTPICO_I2C_SPEED
#define CARRTPICO_I2C_SPEED       (400*1000)
#endif  // CARRTPICO_I2C_SPEED

#ifndef CARRTPICO_I2C_SDA
#define CARRTPICO_I2C_SDA        8
#endif  // CARRTPICO_I2C_SDA

#ifndef CARRTPICO_I2C_SCL
#define CARRTPICO_I2C_SCL        9
#endif  // CARRTPICO_I2C_SCL





// **************************************************************

// UART defines for the serial-link between RPi0 and Pico

#ifndef CARRTPICO_SERIAL_LINK_UART
#define CARRTPICO_SERIAL_LINK_UART                uart1
#endif  // CARRTPICO_SERIAL_LINK_UART

#ifndef CARRTPICO_SERIAL_LINK_UART_BAUD_RATE
#define CARRTPICO_SERIAL_LINK_UART_BAUD_RATE      115200
#endif // CARRTPICO_SERIAL_LINK_UART_BAUD_RATE

#ifndef CARRTPICO_SERIAL_LINK_UART_TX_PIN
#define CARRTPICO_SERIAL_LINK_UART_TX_PIN         4
#endif  // CARRTPICO_SERIAL_LINK_UART_TX_PIN

#ifndef CARRTPICO_SERIAL_LINK_UART_RX_PIN
#define CARRTPICO_SERIAL_LINK_UART_RX_PIN         5    
#endif  // CARRTPICO_SERIAL_LINK_UART_RX_PIN



#endif  // CarrtPicoDefines_h

