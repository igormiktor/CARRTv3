#include <stdio.h>
#include <math.h>

#include "BNO055/bno055.h"
#include "bno055_support.h"

#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/uart.h"


#define MY_I2C_SPEED        ( 400*1000 )
#define MY_I2C_SDA          8
#define MY_I2C_SCL          9



int main()
{
    stdio_init_all();
    
    // I2C Initialisation
    i2c_init( MY_I2C_PORT, MY_I2C_SPEED );
    
    gpio_set_function( MY_I2C_SDA, GPIO_FUNC_I2C ) ;
    gpio_set_function( MY_I2C_SCL, GPIO_FUNC_I2C );
    gpio_pull_up( MY_I2C_SDA );
    gpio_pull_up( MY_I2C_SCL );

    puts( "BNO055 Test");

    sleep_ms( 3000 );

    bno055_data_readout_template( 1 );

    // Remap axis the way we want them
    bno055_set_operation_mode( BNO055_OPERATION_MODE_CONFIG );
    bno055_set_axis_remap_value( BNO055_REMAP_X_Y );
//    bno055_set_remap_x_sign( 1 );
    bno055_set_remap_z_sign( 1 );
    bno055_set_operation_mode( BNO055_OPERATION_MODE_NDOF );

    unsigned char calib_mag = 0;
    unsigned char calib_accel = 0;
    unsigned char calib_gyro = 0;
    unsigned char calib_sys = 0;

    while ( calib_mag + calib_accel + calib_gyro < 9 )
    {
        sleep_ms( 3000 );
        bno055_get_mag_calib_stat( &calib_mag );
        bno055_get_accel_calib_stat( &calib_accel );
        bno055_get_gyro_calib_stat( &calib_gyro );
        bno055_get_sys_calib_stat( &calib_sys );

        printf( "Calib status (M, A, G, S): %u, %u, %u, %u \n", 
                calib_mag, calib_accel, calib_gyro, calib_sys ); 
    }

    puts( "\nAchieved full calibration!\n" );

    for ( int i = 0; i < 10000; ++i )
    {
        putchar( '\n' );
        /* variable used to read the euler h data output as degree or radians */
        struct bno055_euler_float_t euler_hpr;
        bno055_convert_float_euler_hpr_deg( &euler_hpr );
    
        struct bno055_linear_accel_float_t linear_accel_xyz;
        bno055_convert_float_linear_accel_xyz_msq( &linear_accel_xyz );        

        struct bno055_gravity_float_t gravity_xyz;
        bno055_convert_float_gravity_xyz_msq( &gravity_xyz );


        /*
        imu::Quaternion q = bno.getQuat();
        q.normalize();
        float temp = q.x();  q.x() = -q.y();  q.y() = temp;
        q.z() = -q.z();
        imu::Vector<3> euler = q.toEuler();
        Serial.print(F("Orientation: "));
        Serial.print(-180/M_PI * euler.x());  // heading, nose-right is positive, z-axis points up
        Serial.print(F(" "));
        Serial.print(-180/M_PI * euler.y());  // roll, rightwing-up is positive, y-axis points forward
        Serial.print(F(" "));
        Serial.print(-180/M_PI * euler.z());  // pitch, nose-down is positive, x-axis points right
        Serial.println(F(""));
        */
#if ENABLE_QUATERNIONS
        struct bno055_quaternion_t quaternion_wxyz;
        bno055_read_quaternion_wxyz( &quaternion_wxyz );
        // convert to float
        const float scale = (1.0 / (1 << 14));
        float w = scale*quaternion_wxyz.w;
        float x = scale*quaternion_wxyz.x;
        float y = scale*quaternion_wxyz.y;
        float z = scale*quaternion_wxyz.z;
        // Normalize
        float sqw = w*w;
        float sqx = x*x;
        float sqy = y*y;
        float sqz = z*z;
        float mag = sqrt( sqw + sqx + sqy + sqz );
        w /= mag;
        x /= mag;
        y /= mag;
        z /= mag;
        //Flip axes???
        float tmp = x;
        x = y;
        y = tmp;
        z = z;
        // Convert to Euler
        sqw = w*w;
        sqx = x*x;
        sqy = y*y;
        sqz = z*z;
        float h = (-180.0 / M_PI) * atan2( 2.0 * (x * y + z * w), (sqx - sqy - sqz + sqw) );
        float p = (-180.0 / M_PI) * asin( -2.0 * (x * z - y * w) / (sqx + sqy + sqz + sqw) );
        float r = (-180.0 / M_PI) * atan2( 2.0 * (y * z + x * w), (-sqx - sqy + sqz + sqw) );
#endif

        printf( "Accel x = %f, y = %f, z = %f \n", linear_accel_xyz.x, linear_accel_xyz.y, linear_accel_xyz.z );
        printf( "Gravity x = %f, y = %f, z = %f \n", gravity_xyz.x, gravity_xyz.y, gravity_xyz.z );
        printf( "Euler h = %f, p = %f, r = %f \n", euler_hpr.h, euler_hpr.p, euler_hpr.r );
#if ENABLE_QUATERNIONS
        printf( "Q-Euler h = %f, p = %f, r = %f \n", h, p, r );
#endif

        bno055_get_mag_calib_stat( &calib_mag );
        bno055_get_accel_calib_stat( &calib_accel );
        bno055_get_gyro_calib_stat( &calib_gyro );
        bno055_get_sys_calib_stat( &calib_sys );
        printf( "Calib status (M, A, G, S): %u, %u, %u, %u \n", 
                calib_mag, calib_accel, calib_gyro, calib_sys ); 

        sleep_ms( 5000 );
    }

    /* De-initialize */
    bno055_set_power_mode( BNO055_POWER_MODE_SUSPEND );  
    return 0;
}
