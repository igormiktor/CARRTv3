/*
    CarrtPicoDefines.h - System defines for CARRT-Pico

    Copyright (c) 2025 Igor Mikolic-Torreira.  All right reserved.

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

// Enable stdio over UART for debugging (or not)

#ifndef USE_CARRTPICO_STDIO
#define USE_CARRTPICO_STDIO         1       // Default to use if unless explictly turned off
#endif  // USE_CARRTPICO_STDIO

#ifdef  DEBUGPICO
#undef  USE_CARRTPICO_STDIO
#define USE_CARRTPICO_STDIO         1       // Force on if compiling DEBUG version
#endif  // USE_CARRTPICO_STDIO (redefine)




// **************************************************************

// Define the on-board LED
#ifndef CARRTPICO_ONBOARD_LED    
#define CARRTPICO_ONBOARD_LED           PICO_DEFAULT_LED_PIN
#endif  // CARRTPICO_ONBOARD_LED

// Define the signaling LED
#ifndef CARRTPICO_HEARTBEAT_LED
#define CARRTPICO_HEARTBEAT_LED         CARRTPICO_ONBOARD_LED
#endif  // CARRTPICO_HEARTBEAT_LED




// **************************************************************

// Define the GPIO pin for the left encoder
#ifndef CARRTPICO_ENCODER_LEFT_GPIO
#define CARRTPICO_ENCODER_LEFT_GPIO     20          // GPIO20, Pin 26
#endif  // CARRTPICO_ENCODER_LEFT_GPIO

// Define the GPIO pin for the right encoder
#ifndef CARRTPICO_ENCODER_RIGHT_GPIO
#define CARRTPICO_ENCODER_RIGHT_GPIO    19          // GPIO19, Pin 25
#endif  // CARRTPICO_ENCODER_RIGHT_GPIO







// **************************************************************

// I2C defines to Pico's I2C network
// Uses I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.

#ifndef CARRTPICO_I2C_PORT
#define CARRTPICO_I2C_PORT              i2c0
#endif  // CARRTPICO_I2C_PORT

#ifndef CARRTPICO_I2C_SPEED
#define CARRTPICO_I2C_SPEED             (400*1000)
#endif  // CARRTPICO_I2C_SPEED

#ifndef CARRTPICO_I2C_SDA
#define CARRTPICO_I2C_SDA               8
#endif  // CARRTPICO_I2C_SDA

#ifndef CARRTPICO_I2C_SCL
#define CARRTPICO_I2C_SCL               9
#endif  // CARRTPICO_I2C_SCL







// **************************************************************

// BNO055 related defines (I2C network address, etc)

#ifndef CARRT_BNO0555_I2C_ADDR
#define CARRT_BNO0555_I2C_ADDR          0x28
#endif  // CARRT_BNO0555_I2C_ADDR






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




// **************************************************************

// Flag value to confirm successful launch of core1

#define CORE1_SUCCESS      1234
#define CORE1_FAILURE      21

#define SIZE_OF_CORE0_TO_CORE1_QUEUE            8


// **************************************************************

// Debounce time for button presses

#ifndef PICO_GPIO_DEBOUNCE_TIME
#define PICO_GPIO_DEBOUNCE_TIME                 10      // milliseconds (seems to work well)
#endif  // PICO_GPIO_DEBOUNCE_TIME




#endif  // CarrtPicoDefines_h

