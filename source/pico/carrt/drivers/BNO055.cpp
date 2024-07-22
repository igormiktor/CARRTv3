/*
    BNO055.cpp - BNO055 functions for CARRT Pico.

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


#include "BNO055.h"

#include "I2C.h"
#include "shared/CarrtError.h"


#include "hardware/timer.h"


extern "C"
{
    // This file does not have guards for C++
    #include "BNO055/bno055.h"
}





#if 0
struct bno055_t
{
    u8 chip_id; /**< chip_id of bno055 */
    u16 sw_rev_id; /**< software revision id of bno055 */
    u8 page_id; /**< page_id of bno055 */
    u8 accel_rev_id; /**< accel revision id of bno055 */
    u8 mag_rev_id; /**< mag revision id of bno055 */
    u8 gyro_rev_id; /**< gyro revision id of bno055 */
    u8 bl_rev_id; /**< boot loader revision id of bno055 */
    u8 dev_addr; /**< i2c device address of bno055 */
    BNO055_WR_FUNC_PTR; /**< bus write function pointer */
    BNO055_RD_FUNC_PTR; /**<bus read function pointer */
    void (*delay_msec)(BNO055_MDELAY_DATA_TYPE); /**< delay function pointer */
};
#endif


namespace BNO055
{
    struct bno055_t     sBno055;

    void delayMsec( unsigned int msec );
};







void BNO055::delayMsec( unsigned int msec )
{
    busy_wait_ms( msec );
}




void BNO055::init()
{
    sBno055.dev_addr = BNO055_I2C_ADDR1;    
    sBno055.bus_write = I2C::send;
    sBno055.bus_read = I2C::receive;
    sBno055.delay_msec = BNO055::delayMsec;

    int err = bno055_init( &sBno055 );

    // Set the power mode as NORMAL 
    err += bno055_set_power_mode( BNO055_POWER_MODE_NORMAL );

    // Set mode of the BNO055 to NDOF (9 Degs of Freedom, Fused)
    err += bno055_set_operation_mode( BNO055_OPERATION_MODE_NDOF );

    if ( err )
    {
        throw CarrtError( makePicoErrorId( PicoError::kPicoI2cBNO055Error, 1, err ), "CARRT Pico BNO055 init failed" );
    }

    // Need to wait for the BNO055 to reach calibration, but make that a separate function call
}
    
 

