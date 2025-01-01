/*
    CarrtPigpio - Interface to allow easy switch between pigpio and pigpiod.

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


#ifndef USE_PIGPIO
#error "USE_PIGPIOD is undefined!"
#endif

#if USE_PIGPIOD
#include <pigpiod_if2.h>
#else
#include <pigpio.h>
#endif


/* 
    Prefer the pigio function names, so map these to the pigiod function names if
    pigpiod is being used

    And deal with the pesky Pi id number
*/


#if USE_PIGPIOD

extern int gGpioId;


inline int gpioInitialise( void )
{
    // Always dealing with local pigiod
    gGpioId = pigpio_start( 0, 0 );
    return gGpioId;
}

inline void gpioTerminate()
{
    pigpio_stop( gGpioId );
}


int gpioSetMode( unsigned gpio, unsigned mode )
{
    return set_mode( gGpioId, gpio, mode );
}


int gpioGetMode( unsigned gpio )
{
    return get_mode( gGpioId, gpio );
}


int gpioSetPullUpDown( unsigned gpio, unsigned pud ) 
{
    return set_pull_up_down( gGpioId, gpio, pud );
}


int gpioRead( unsigned gpio )
{
    return gpio_read( gGpioId, gpio );
}


int gpioWrite( unsigned gpio, unsigned level )
{
    return gpio_write( gGpioId, gpio, level );
}


int gpioPWM( unsigned user_gpio, unsigned dutycycle )
{
    return set_PWM_dutycycle( gGpioId, user_gpio, dutycycle );
}


int gpioGetPWMdutycycle( unsigned user_gpio )
{
    return get_PWM_dutycycle( gGpioId, user_gpio );
}


int gpioSetPWMrange( unsigned user_gpio, unsigned range )
{
    return set_PWM_range( gGpioId, user_gpio, range );
}


int gpioGetPWMrange(unsigned user_gpio);


int gpioGetPWMrealRange(unsigned user_gpio);


int gpioSetPWMfrequency(unsigned user_gpio, unsigned frequency);


int gpioGetPWMfrequency(unsigned user_gpio);


int gpioServo(unsigned user_gpio, unsigned pulsewidth);


int gpioGetServoPulsewidth(unsigned user_gpio);


int gpioSetAlertFunc(unsigned user_gpio, gpioAlertFunc_t f);


int gpioSetAlertFuncEx(
   unsigned user_gpio, gpioAlertFuncEx_t f, void *userdata);


int gpioSetISRFunc(
   unsigned gpio, unsigned edge, int timeout, gpioISRFunc_t f);


int gpioSetISRFuncEx(
   unsigned gpio,
   unsigned edge,
   int timeout,
   gpioISRFuncEx_t f,
   void *userdata);

#endif

