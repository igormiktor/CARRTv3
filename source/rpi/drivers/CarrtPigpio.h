/*
    CarrtPigpio - Interface to allow easy switch between pigpio and pigpiod.

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


#ifndef CarrtPigpio_h
#define CarrtPigpio_h

#ifndef USE_PIGPIOD
    #warning "USE_PIGPIOD is undefined!"
#endif

#if USE_PIGPIOD
    #include <pigpiod_if2.h>
#else
    #include <pigpio.h>
#endif


/* 
    Prefer the pigpio function names, so map these to the pigpiod function names if
    pigpiod is being usedCarrtPigpio.h

    And deal with the pesky Pi id number
*/


#if USE_PIGPIOD

extern int gPigpioId;


extern "C"
{

inline int gpioInitialise( void )
{
    // Always dealing with local pigiod
    gPigpioId = pigpio_start( 0, 0 );
    return gPigpioId;
}

inline void gpioTerminate()
{
    pigpio_stop( gPigpioId );
}


inline int gpioSetMode( unsigned gpio, unsigned mode )
{
    return set_mode( gPigpioId, gpio, mode );
}


inline int gpioGetMode( unsigned gpio )
{
    return get_mode( gPigpioId, gpio );
}


inline int gpioSetPullUpDown( unsigned gpio, unsigned pud ) 
{
    return set_pull_up_down( gPigpioId, gpio, pud );
}


inline int gpioRead( unsigned gpio )
{
    return gpio_read( gPigpioId, gpio );
}


inline int gpioWrite( unsigned gpio, unsigned level )
{
    return gpio_write( gPigpioId, gpio, level );
}


inline int gpioPWM( unsigned user_gpio, unsigned dutycycle )
{
    return set_PWM_dutycycle( gPigpioId, user_gpio, dutycycle );
}


inline int gpioGetPWMdutycycle( unsigned user_gpio )
{
    return get_PWM_dutycycle( gPigpioId, user_gpio );
}


inline int gpioSetPWMrange( unsigned user_gpio, unsigned range )
{
    return set_PWM_range( gPigpioId, user_gpio, range );
}


inline int gpioGetPWMrange( unsigned user_gpio )
{
    return get_PWM_range( gPigpioId, user_gpio );
}


inline int gpioGetPWMrealRange( unsigned user_gpio )
{
    return get_PWM_real_range( gPigpioId, user_gpio );
}


inline int gpioSetPWMfrequency( unsigned user_gpio, unsigned frequency )
{
    return set_PWM_frequency( gPigpioId, user_gpio, frequency );
}


inline int gpioGetPWMfrequency( unsigned user_gpio )
{
    return get_PWM_frequency( gPigpioId, user_gpio );
}


inline int gpioServo( unsigned user_gpio, unsigned pulsewidth )
{
    return set_servo_pulsewidth( gPigpioId, user_gpio, pulsewidth );
}


inline int gpioGetServoPulsewidth( unsigned user_gpio )
{
    return get_servo_pulsewidth( gPigpioId, user_gpio );
}



// int gpioSetAlertFunc(unsigned user_gpio, gpioAlertFunc_t f);                                             // No pigpiod equivalent

// int gpioSetAlertFuncEx( unsigned user_gpio, gpioAlertFuncEx_t f, void *userdata );                       // No pigpiod equivalent

// int gpioSetISRFunc( unsigned gpio, unsigned edge, int timeout, gpioISRFunc_t f );                        // No pigpiod equivalent

// int gpioSetISRFuncEx( unsigned gpio, unsigned edge, int timeout, gpioISRFuncEx_t f, void *userdata );    // No pigpiod equivalent



inline int gpioNotifyOpen( void )
{
    return notify_open( gPigpioId );
}



// int gpioNotifyOpenWithSize(int bufSize );                                                                // No pigpiod equivalent



inline int gpioNotifyBegin( unsigned handle, uint32_t bits )
{
    return notify_begin( gPigpioId, handle, bits );
}


inline int gpioNotifyPause( unsigned handle )
{
    return notify_pause( gPigpioId, handle );
}


inline int gpioNotifyClose( unsigned handle )
{
    return notify_close( gPigpioId, handle );
}


inline int gpioWaveClear( void )
{
    return wave_clear( gPigpioId );
}


inline int gpioWaveAddNew( void )
{
    return wave_add_new( gPigpioId );
}


inline int gpioWaveAddGeneric( unsigned numPulses, gpioPulse_t *pulses )
{
    return wave_add_generic( gPigpioId, numPulses, pulses );
}


inline int gpioWaveAddSerial( unsigned user_gpio, unsigned baud, unsigned data_bits, unsigned stop_bits, unsigned offset, unsigned numBytes, char *str ) 
{
    return wave_add_serial( gPigpioId, user_gpio, baud, data_bits, stop_bits, offset, numBytes, str );
}


inline int gpioWaveCreate( void )
{
    return wave_create( gPigpioId );
}


// int gpioWaveCreatePad( int pctCB, int pctBOOL, int pctTOOL );             // No exact pigpiod equivalent


inline int gpioWaveDelete( unsigned wave_id )
{
    return wave_delete( gPigpioId, wave_id );
}


inline int gpioWaveTxSend( unsigned wave_id, unsigned wave_mode )
{
    return wave_send_using_mode( gPigpioId, wave_id, wave_mode );
}


inline int gpioWaveChain( char *buf, unsigned bufSize )
{
    return wave_chain( gPigpioId, buf, bufSize );
}


inline int gpioWaveTxAt( void )
{
    return wave_tx_at( gPigpioId );
}


inline int gpioWaveTxBusy( void )
{
    return wave_tx_busy( gPigpioId );
}


inline int gpioWaveTxStop( void ) 
{
    return wave_tx_stop( gPigpioId );
}


inline int gpioWaveGetMicros( void )
{
    return wave_get_micros( gPigpioId );
}


inline int gpioWaveGetHighMicros( void )
{
    return wave_get_high_micros( gPigpioId );
}


inline int gpioWaveGetMaxMicros( void )
{
    return wave_get_max_micros( gPigpioId );
}


inline int gpioWaveGetPulses( void )
{
    return wave_get_pulses( gPigpioId );
}


inline int gpioWaveGetHighPulses( void ) 
{
    return wave_get_high_pulses( gPigpioId );
}


inline int gpioWaveGetMaxPulses( void ) 
{
    return wave_get_max_pulses( gPigpioId );
}


inline int gpioWaveGetCbs( void )
{
    return wave_get_cbs( gPigpioId );
}


inline int gpioWaveGetHighCbs( void )
{
    return wave_get_high_cbs( gPigpioId );
}


inline int gpioWaveGetMaxCbs( void )
{
    return wave_get_max_cbs( gPigpioId );
}


inline int gpioSerialReadOpen( unsigned user_gpio, unsigned baud, unsigned data_bits )
{
    return bb_serial_read_open( gPigpioId, user_gpio, baud, data_bits );
}


inline int gpioSerialReadInvert( unsigned user_gpio, unsigned invert )
{
    return bb_serial_invert( gPigpioId, user_gpio, invert );
}


inline int gpioSerialRead( unsigned user_gpio, void *buf, size_t bufSize )
{
    return bb_serial_read( gPigpioId, user_gpio, buf, bufSize );
}


inline int gpioSerialReadClose( unsigned user_gpio ) 
{
    return bb_serial_read_close( gPigpioId, user_gpio );
}


inline int i2cOpen( unsigned i2cBus, unsigned i2cAddr, unsigned i2cFlags )
{
    return i2c_open( gPigpioId, i2cBus, i2cAddr, i2cFlags );
}


inline int i2cClose( unsigned handle ) 
{
    return i2c_close( gPigpioId, handle );
}


inline int i2cWriteQuick( unsigned handle, unsigned bit )
{
    return i2c_write_quick( gPigpioId, handle, bit );
}


inline int i2cWriteByte( unsigned handle, unsigned bVal )
{
    return i2c_write_byte( gPigpioId, handle, bVal );
}


inline int i2cReadByte( unsigned handle )
{
    return i2c_read_byte( gPigpioId, handle );
}


inline int i2cWriteByteData( unsigned handle, unsigned i2cReg, unsigned bVal ) 
{
    return i2c_write_byte_data( gPigpioId, handle, i2cReg, bVal );
}


inline int i2cWriteWordData( unsigned handle, unsigned i2cReg, unsigned wVal )
{
    return i2c_write_word_data( gPigpioId, handle, i2cReg, wVal );
}


inline int i2cReadByteData( unsigned handle, unsigned i2cReg )
{
    return i2c_read_byte_data( gPigpioId, handle, i2cReg );
}


inline int i2cReadWordData( unsigned handle, unsigned i2cReg )
{
    return i2c_read_word_data( gPigpioId, handle, i2cReg );
}


inline int i2cProcessCall( unsigned handle, unsigned i2cReg, unsigned wVal )
{
    return i2c_process_call( gPigpioId, handle, i2cReg, wVal );
}


inline int i2cWriteBlockData( unsigned handle, unsigned i2cReg, char *buf, unsigned count )
{
    return i2c_write_block_data( gPigpioId, handle, i2cReg, buf, count );
}


inline int i2cReadBlockData( unsigned handle, unsigned i2cReg, char *buf )
{
    return i2c_read_block_data( gPigpioId, handle, i2cReg, buf );
}


inline int i2cBlockProcessCall( unsigned handle, unsigned i2cReg, char *buf, unsigned count )
{
    return i2c_block_process_call( gPigpioId, handle, i2cReg, buf, count );
}


inline int i2cReadI2CBlockData( unsigned handle, unsigned i2cReg, char *buf, unsigned count )
{
    return i2c_read_i2c_block_data( gPigpioId, handle, i2cReg, buf, count );
}


inline int i2cWriteI2CBlockData( unsigned handle, unsigned i2cReg, char *buf, unsigned count )
{
    return i2c_write_i2c_block_data( gPigpioId, handle, i2cReg, buf, count );
}


inline int i2cReadDevice( unsigned handle, char *buf, unsigned count )
{
    return i2c_read_device( gPigpioId, handle, buf, count );
}


inline int i2cWriteDevice( unsigned handle, char *buf, unsigned count )
{
    return i2c_write_device( gPigpioId, handle, buf, count );
}


// void i2cSwitchCombined(int setting);                                         // No defined in pigpiod

// int i2cSegments(unsigned handle, pi_i2c_msg_t *segs, unsigned numSegs);      // No defined in pigpiod


inline int i2cZip( unsigned handle, char* inBuf, unsigned inLen, char* outBuf, unsigned outLen )
{
    return i2c_zip( gPigpioId, handle, inBuf, inLen, outBuf, outLen );
}


inline int bbI2COpen( unsigned SDA, unsigned SCL, unsigned baud )
{
    return bb_i2c_open( gPigpioId, SDA, SCL, baud );
}


inline int bbI2CClose( unsigned SDA )
{
    return bb_i2c_close( gPigpioId, SDA );
}


inline int bbI2CZip( unsigned SDA, char *inBuf, unsigned inLen, char *outBuf, unsigned outLen )
{
    return bb_i2c_zip( gPigpioId, SDA, inBuf, inLen, outBuf, outLen );
}


inline int bscXfer( bsc_xfer_t *bscxfer ) 
{
    return bsc_xfer( gPigpioId, bscxfer );
}


inline int bbSPIOpen( unsigned CS, unsigned MISO, unsigned MOSI, unsigned SCLK, unsigned baud, unsigned spiFlags )
{
    return bb_spi_open( gPigpioId, CS, MISO, MOSI, SCLK, baud, spiFlags );
}


inline int bbSPIClose( unsigned CS )
{
    return bb_spi_close( gPigpioId, CS );
}


inline int bbSPIXfer( unsigned CS, char *inBuf, char *outBuf, unsigned count )
{
    return bb_spi_xfer( gPigpioId, CS, inBuf, outBuf, count );
}


inline int spiOpen( unsigned spiChan, unsigned baud, unsigned spiFlags )
{
    return spi_open( gPigpioId, spiChan, baud, spiFlags );
}


inline int spiClose( unsigned handle )
{
    return spi_close( gPigpioId, handle );
}


inline int spiRead( unsigned handle, char *buf, unsigned count )
{
    return spi_read( gPigpioId, handle, buf, count );
}


inline int spiWrite( unsigned handle, char *buf, unsigned count )
{
    return spi_write( gPigpioId, handle, buf, count );
}


inline int spiXfer( unsigned handle, char *txBuf, char *rxBuf, unsigned count )
{
    return spi_xfer( gPigpioId, handle, txBuf, rxBuf, count );
}


inline int serOpen( char *sertty, unsigned baud, unsigned serFlags )
{
    return serial_open( gPigpioId, sertty, baud, serFlags );
}


inline int serClose( unsigned handle )
{
    return serial_close( gPigpioId, handle );
}

inline int serWriteByte( unsigned handle, unsigned bVal )
{
    return serial_write_byte( gPigpioId, handle, bVal );
}


inline int serReadByte( unsigned handle )
{
    return serial_read_byte( gPigpioId, handle );
}

inline int serWrite( unsigned handle, char *buf, unsigned count )
{
    return serial_write( gPigpioId, handle, buf, count );
}


inline int serRead( unsigned handle, char *buf, unsigned count )
{
    return serial_read( gPigpioId, handle, buf, count );
}


inline int serDataAvailable( unsigned handle )
{
    return serial_data_available( gPigpioId, handle );
}


inline int gpioTrigger( unsigned user_gpio, unsigned pulseLen, unsigned level )
{
    return gpio_trigger( gPigpioId, user_gpio, pulseLen, level );
}


inline int gpioSetWatchdog( unsigned user_gpio, unsigned timeout )
{
    return set_watchdog( gPigpioId, user_gpio, timeout );
}


inline int gpioNoiseFilter( unsigned user_gpio, unsigned steady, unsigned active )
{
    return set_noise_filter( gPigpioId, user_gpio, steady, active );
}


inline int gpioGlitchFilter( unsigned user_gpio, unsigned steady )
{
    return set_glitch_filter( gPigpioId, user_gpio, steady );
}


// int gpioSetGetSamplesFunc( gpioGetSamplesFunc_t f, uint32_t bits );                                  // No defined in pigpiod


// inline int gpioSetGetSamplesFuncEx( gpioGetSamplesFuncEx_t f, uint32_t bits, void *userdata );       // No defined in pigpiod


// int gpioSetTimerFunc( unsigned timer, unsigned millis, gpioTimerFunc_t f );                          // No defined in pigpiod


// int gpioSetTimerFuncEx( unsigned timer, unsigned millis, gpioTimerFuncEx_t f, void *userdata );      // No defined in pigpiod


inline pthread_t *gpioStartThread( gpioThreadFunc_t f, void *userdata )
{
    return start_thread( f, userdata );
}


inline void gpioStopThread( pthread_t *pth )
{
   stop_thread( pth );
}


inline int gpioStoreScript( char *script )
{
    return store_script( gPigpioId, script );
}


inline int gpioRunScript( unsigned script_id, unsigned numPar, uint32_t *param )
{
    return run_script( gPigpioId, script_id, numPar, param );
}


inline int gpioUpdateScript( unsigned script_id, unsigned numPar, uint32_t *param )
{
    return update_script( gPigpioId, script_id, numPar, param );
}


inline int gpioScriptStatus( unsigned script_id, uint32_t *param )
{
    return script_status( gPigpioId, script_id, param );
}


inline int gpioStopScript( unsigned script_id )
{
    return stop_script( gPigpioId, script_id );
}


inline int gpioDeleteScript( unsigned script_id )
{
    return delete_script( gPigpioId, script_id );
}


// int gpioSetSignalFunc( unsigned signum, gpioSignalFunc_t f) ;                            // No defined in pigpiod


// int gpioSetSignalFuncEx( unsigned signum, gpioSignalFuncEx_t f, void *userdata );        // No defined in pigpiod


inline uint32_t gpioRead_Bits_0_31( void )
{
    return read_bank_1( gPigpioId );
}


inline uint32_t gpioRead_Bits_32_53( void )
{
    return read_bank_2( gPigpioId );
}


inline int gpioWrite_Bits_0_31_Clear( uint32_t bits )
{
    return clear_bank_1( gPigpioId, bits );
}


inline int gpioWrite_Bits_32_53_Clear( uint32_t bits )
{
    return clear_bank_2( gPigpioId, bits );
}


inline int gpioWrite_Bits_0_31_Set( uint32_t bits )
{
    return set_bank_1( gPigpioId, bits );
}


inline int gpioWrite_Bits_32_53_Set( uint32_t bits )
{
    return set_bank_2( gPigpioId, bits );
}


inline int gpioHardwareClock( unsigned gpio, unsigned clkfreq )
{
    return hardware_clock( gPigpioId, gpio, clkfreq );
}

inline int gpioHardwarePWM( unsigned gpio, unsigned PWMfreq, unsigned PWMduty )
{
    return hardware_PWM( gPigpioId, gpio, PWMfreq, PWMduty );
}

// int gpioTime( unsigned timetype, int *seconds, int *micros );                    // No defined in pigpiod


// int gpioSleep( unsigned timetype, int seconds, int micros );                     // No defined in pigpiod


// uint32_t gpioDelay(uint32_t micros);                                             // No defined in pigpiod


inline uint32_t gpioTick( void )
{ 
    return get_current_tick( gPigpioId );
}


inline unsigned gpioHardwareRevision( void )
{
    return get_hardware_revision( gPigpioId );
}


inline unsigned gpioVersion( void )
{
    return get_pigpio_version( gPigpioId );
}


inline int gpioGetPad( unsigned pad )
{
    return get_pad_strength( gPigpioId, pad );
}


inline int gpioSetPad( unsigned pad, unsigned padStrength )
{
    return set_pad_strength( gPigpioId, pad, padStrength );
}



//  int eventMonitor(unsigned handle, uint32_t bits);                           // Not compatible with pigpiod

// int eventSetFunc(unsigned event, eventFunc_t f);                             // Not compatible with pigpiod

// int eventSetFuncEx(unsigned event, eventFuncEx_t f, void *userdata);         // Not compatible with pigpiod

// int eventTrigger(unsigned event);                                            // Not compatible with pigpiod


inline int shell( char *scriptName, char *scriptString )
{
    return shell_( gPigpioId, scriptName, scriptString );
}



#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcomment"

inline int fileOpen( char *file, unsigned mode )
{
    return file_open( gPigpioId, file, mode );
}

#pragma GCC diagnostic pop


inline int fileClose( unsigned handle )
{
    return file_close( gPigpioId, handle );
}


inline int fileWrite( unsigned handle, char *buf, unsigned count )
{
    return file_write( gPigpioId, handle, buf, count );
}


inline int fileRead( unsigned handle, char *buf, unsigned count )
{
    return file_read( gPigpioId, handle, buf, count );
}


inline int fileSeek( unsigned handle, int32_t seekOffset, int seekFrom ) 
{
    return file_seek( gPigpioId, handle, seekOffset, seekFrom );
}


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcomment"

inline int fileList( char *fpat, char *buf, unsigned count )
{
    return file_list( gPigpioId, fpat, buf, count );
}

#pragma GCC diagnostic pop


// int gpioCfgBufferSize( unsigned cfgMillis );                                                     // No defined in pigpiod

// int gpioCfgClock( unsigned cfgMicros, unsigned cfgPeripheral, unsigned cfgSource ;               // Not defined in pigpiod

// int gpioCfgDMAchannel(unsigned DMAchannel);                                                      // Not defined in pigpiod

// int gpioCfgDMAchannels(unsigned primaryChannel, unsigned secondaryChannel);                      // Not defined in pigpiod

// int gpioCfgPermissions(uint64_t updateMask);                                                     // Not defined in pigpiod

// int gpioCfgSocketPort(unsigned port);                                                            // Not defined in pigpiod

// int gpioCfgInterfaces(unsigned ifFlags);                                                         // Not defined in pigpiod

// int gpioCfgMemAlloc(unsigned memAllocMode);                                                      // Not defined in pigpiod

// int gpioCfgNetAddr(int numSockAddr, uint32_t *sockAddr);                                         // Not defined in pigpiod

// uint32_t gpioCfgGetInternals(void);                                                              // Not defined in pigpiod

// int gpioCfgSetInternals(uint32_t cfgVal);                                                        // Not defined in pigpiod

// int gpioCustom1(unsigned arg1, unsigned arg2, char *argx, unsigned argc);                        // Not defined in pigpiod

// int gpioCustom2(unsigned arg1, char *argx, unsigned argc, char *retBuf, unsigned retMax);        // Not defined in pigpiod


// int rawWaveAddSPI(rawSPI_t *spi, unsigned offset, unsigned spiSS, char *buf, unsigned spiTxBits,     // Not defined in pigpiod
//    unsigned spiBitFirst, unsigned spiBitLast, unsigned spiBits);

// int rawWaveAddGeneric(unsigned numPulses, rawWave_t *pulses);                                    // Not defined in pigpiod 

// unsigned rawWaveCB(void);                                                                        // Not defined in pigpiod

// rawCbs_t *rawWaveCBAdr(int cbNum);                                                               // Not defined in pigpiod

// uint32_t rawWaveGetOOL(int pos);                                                                 // Not defined in pigpiod

// void rawWaveSetOOL(int pos, uint32_t lVal);                                                      // Not defined in pigpiod

// uint32_t rawWaveGetOut(int pos);                                                                 // Not defined in pigpiod

// void rawWaveSetOut(int pos, uint32_t lVal);                                                      // Not defined in pigpiod

// uint32_t rawWaveGetIn(int pos);                                                                  // Not defined in pigpiod

// void rawWaveSetIn(int pos, uint32_t lVal);                                                       // Not defined in pigpiod

// rawWaveInfo_t rawWaveInfo(int wave_id);                                                          // Not defined in pigpiod

// int getBitInBytes(int bitPos, char *buf, int numBits);                                           // Not defined in pigpiod     

// void putBitInBytes(int bitPos, char *buf, int bit);                                              // Not defined in pigpiod


// double time_time( void )                                         // Name and signature unchanged

// void time_sleep( double seconds );                               // Name and signature unchanged


// void rawDumpWave(void);                                                                          // Not defined in pigpiod

// void rawDumpScript(unsigned script_id);                                                          // Not defined in pigpiod


}

#endif  // #if USE_PIGPIOD

#endif  // #ifndef CarrtPigpio_h