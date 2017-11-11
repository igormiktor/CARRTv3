/*
    I2c.h - An I2C master library
    For Raspberry Pi (secifically, Pi Zero W).

    Copyright (c) 2017 Igor Mikolic-Torreira.  All right reserved.

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


/*!
 * \file
 *
 * \brief This file provides functions that interface to the TWI (two-wire serial interface) hardware
 * of the Raspberry Pi, providing a high-level interface to
 * I2C protocol communications.  Include this file if you want your application will operate in Master mode
 * as defined in the I2C protocol.
 *
 */





#ifndef I2c_h
#define I2c_h


#include <cstdint>



/*!
 * \brief This namespace bundles the I2C-protocol-based interface to the TWI hardware.  It provides logical cohesion
 * for functions implement the Master portions of the I2C protocol and prevents namespace collisions.
 *
 * These interfaces are buffered for both input and output and operate using interrupts associated
 * with the TWI hardware.  This means the asynchronous transmit functions return immediately after queuing data
 * in the output buffer for transmission and the transmission happens asynchronously, using
 * dedicated TWI hardware. Similarly, data is received asynchronously and placed into the input buffer.
 *
 * These functions are designed around the normal operating modes of the I2C protocol.  From a Master device point of view,
 * I2C communications consist of sending a designated device a message to do something, and then either:
 *
 * - doing nothing because no further action required on the Master's part (e.g., telling the designated device to shutdown)
 * - transmitting additional data needed by the designated device (e.g., telling the designated device to store some data)
 * - receiving data from the designated device (e.g., telling the designated device to report the current temperature or to
 * read back some data from its memory)
 *
 * For very simple devices, the receipt of the message itself can suffice to tell it to do something.  More commonly,
 * the instruction to the designated device consists of a single byte that passes a "register address" on the device.  It is
 * call a register address because it often corresponds directly to a memory register on the device.  But
 * it is best to think of it as an instruction code to the designated device (e.g., 0x01 = report the temperature;
 * 0x02 = set the units to either F or C (depending on additional data sent by the Master); 0x03 = report the humidity; etc.)
 *
 * The functions defined by this module conform directly to the above I2C paradigm.  The functions come in both synchronous and
 * asynchronous versions.  The synchronous versions simply call the asynchronous versions and block internally until the
 * asynchronous operations are complete.
 *
 * Note also that even "read" operations always begin (from the Master's point of view) with a "send" to the designated device
 * the Master wants to read data from.  For this reason all operations (both read and write) utilize the transmit buffer.
 */

namespace I2c
{

    /*!
    * \brief This enum lists I2C bus speed configurations.
    *
    * \hideinitializer
    */
    enum I2cBusSpeed
    {
        kI2cBusSlow                 = 0,                    //!< I2C slow (standard) mode: 100 KHz  \hideinitializer
        kI2cBusFast                 = 1                     //!< I2C fast mode: 400 KHz  \hideinitializer
    };


    /*!
     * \brief Configures the TWI hardware for I2C communications in Master mode.  You must call this function before conducting any
     * I2C communications using the functions in this module.
     *
     * This function enables the TWI related interrupts and enables the built-in hardware pullups.
     *
     * \arg \c speed the speed mode for the I2C protocol.  The options are slow (100 KHz) or fast (400 KHz); the
     * default is fast (kI2cBusFast).
     */
    void start( std::uint8_t speed = kI2cBusFast );


    /*!
     * \brief Terminates the I2C communications using the TWI hardware, and disables the TWI interrupts.
     *
     * After calling this function, you need to call start() again if you want to resume I2C communications.
     */
    void stop();


    /*!
     * \brief Transmit a single register address (a one-byte message) synchronously. This function blocks until
     * the communications exchange is complete or encounters an error. Error codes are returned (0 means no error).
     *
     * \arg \c address the I2C address of the destination device for this message.
     * \arg \c registerAddress in device-centric terms, the register address on the destination device; think of it as
     * a one-byte instruction to the destination device telling it to do something (e.g., turn off or on).
     *
     * \returns an error code which if positive corresponds to I2cSendErrorCodes, or if negative the absolute value
     * corresponds to I2cStatusCodes (0 means no error).
     */
    int write( std::uint8_t address, std::uint8_t registerAddress );


    /*!
     * \brief Transmit a single register address and corresponding single byte of data synchronously. This function blocks until
     * the communications exchange is complete or encounters an error. Error codes are returned (0 means no error).
     *
     * \arg \c address the I2C address of the destination device for this message.
     * \arg \c registerAddress in device-centric terms, the register address on the destination device; think of it as
     * a one-byte instruction to the destination device telling it to do something (e.g., set the volume level).
     * \arg \c data a single byte of data serving as a parameter to the register address (e.g., the volume level to set).
     *
     * \returns an error code which if positive corresponds to I2cSendErrorCodes, or if negative the absolute value
     * corresponds to I2cStatusCodes (0 means no error).
     */
    int write( std::uint8_t address, std::uint8_t registerAddress, std::uint8_t data );


    int write( std::uint8_t address, std::uint8_t registerAddress, std::uint16_t data );


    /*!
     * \brief Transmit a single register address and corresponding null-terminated string of data synchronously.
     * This function blocks until
     * the communications exchange is complete or encounters an error. Error codes are returned (0 means no error).
     *
     * \arg \c address the I2C address of the destination device for this message
     * \arg \c registerAddress in device-centric terms, the register address on the destination device; think of it as
     * a one-byte instruction to the destination device telling it to do something (e.g., an address in a memory device).
     * \arg \c data a null-terminated string of data serving as a parameter to the register address (e.g., a string to store
     * sequentially starting at the registerAddress).
     *
     * \returns an error code which if positive corresponds to I2cSendErrorCodes, or if negative the absolute value
     * corresponds to I2cStatusCodes (0 means no error).
     */
    int write( std::uint8_t address, std::uint8_t registerAddress, const char* data );


    /*!
     * \brief Transmit a single register address and corresponding buffer of data synchronously.
     * This function blocks until
     * the communications exchange is complete or encounters an error. Error codes are returned (0 means no error).
     *
     *
     * \arg \c address the I2C address of the destination device for this message
     * \arg \c registerAddress in device-centric terms, the register address on the destination device; think of it as
     * a one-byte instruction to the destination device telling it to do something (e.g., an address in a memory device).
     * \arg \c data a buffer of data serving as a parameter to the register address (e.g., the data to store
     * sequentially starting at the registerAddress).
     * \arg \c numberBytes the number of bytes from the buffer to transmit.
     *
     * \returns an error code which if positive corresponds to I2cSendErrorCodes, or if negative the absolute value
     * corresponds to I2cStatusCodes (0 means no error).
     */
    int write( std::uint8_t address, std::uint8_t registerAddress, std::uint8_t* data, std::uint8_t numberBytes );



    int read( std::uint8_t address, std::uint8_t registerAddress, std::uint8_t* destination );

    int read( std::uint8_t address, std::uint8_t registerAddress, std::uint16_t* destination );


    /*!
     * \brief Request to read data from a specific register on a device and receive that data synchronously.
     * This function blocks until
     * the communications exchange is complete or encounters an error. Error codes are returned (0 means no error).
     *
     * \arg \c address the I2C address of the destination device you want to read from.
     * \arg \c registerAddress in device-centric terms, the register address on the destination device; think of it as
     * a one-byte instruction to the destination device telling it what you want to read (e.g., temperature or the
     * starting address of a block of memory).
     * \arg \c numberBytes the number of bytes you expect to read.
     * \arg \c destination a pointer to a buffer in which the received data will be stored; the buffer should be
     * at least \c numberBytes large.
     *
     * \returns an error code which if positive corresponds to I2cSendErrorCodes, or if negative the absolute value
     * corresponds to I2cStatusCodes (0 means no error).
     */
    int read( std::uint8_t address, std::uint8_t registerAddress, std::uint8_t numberBytes, std::uint8_t* destination );


};


#endif
