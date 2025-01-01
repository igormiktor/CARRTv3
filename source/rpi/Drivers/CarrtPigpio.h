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
    pigpiod is being used

    And deal with the pesky Pi id number
*/


#if USE_PIGPIOD

extern int gGpioId;


extern "C"
{

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


inline int gpioSetMode( unsigned gpio, unsigned mode )
{
    return set_mode( gGpioId, gpio, mode );
}


inline int gpioGetMode( unsigned gpio )
{
    return get_mode( gGpioId, gpio );
}


inline int gpioSetPullUpDown( unsigned gpio, unsigned pud ) 
{
    return set_pull_up_down( gGpioId, gpio, pud );
}


inline int gpioRead( unsigned gpio )
{
    return gpio_read( gGpioId, gpio );
}


inline int gpioWrite( unsigned gpio, unsigned level )
{
    return gpio_write( gGpioId, gpio, level );
}


inline int gpioPWM( unsigned user_gpio, unsigned dutycycle )
{
    return set_PWM_dutycycle( gGpioId, user_gpio, dutycycle );
}


inline int gpioGetPWMdutycycle( unsigned user_gpio )
{
    return get_PWM_dutycycle( gGpioId, user_gpio );
}


inline int gpioSetPWMrange( unsigned user_gpio, unsigned range )
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


int gpioSetAlertFuncEx( unsigned user_gpio, gpioAlertFuncEx_t f, void *userdata );


int gpioSetISRFunc(
   unsigned gpio, unsigned edge, int timeout, gpioISRFunc_t f);


int gpioSetISRFuncEx(
   unsigned gpio,
   unsigned edge,
   int timeout,
   gpioISRFuncEx_t f,
   void *userdata);


int gpioNotifyOpen(void);


int gpioNotifyOpenWithSize(int bufSize);


int gpioNotifyBegin(unsigned handle, uint32_t bits);


int gpioNotifyPause(unsigned handle);


int gpioNotifyClose(unsigned handle);


int gpioWaveClear(void);


int gpioWaveAddNew(void);


int gpioWaveAddGeneric(unsigned numPulses, gpioPulse_t *pulses);


int gpioWaveAddSerial
   (unsigned user_gpio,
    unsigned baud,
    unsigned data_bits,
    unsigned stop_bits,
    unsigned offset,
    unsigned numBytes,
    char     *str);


int gpioWaveCreate(void);


int gpioWaveCreatePad(int pctCB, int pctBOOL, int pctTOOL);

int gpioWaveDelete(unsigned wave_id);


int gpioWaveTxSend(unsigned wave_id, unsigned wave_mode);


int gpioWaveChain(char *buf, unsigned bufSize);


int gpioWaveTxAt(void);


int gpioWaveTxBusy(void);


int gpioWaveTxStop(void);


int gpioWaveGetMicros(void);


int gpioWaveGetHighMicros(void);


int gpioWaveGetMaxMicros(void);


int gpioWaveGetPulses(void);


int gpioWaveGetHighPulses(void);


int gpioWaveGetMaxPulses(void);


int gpioWaveGetCbs(void);


int gpioWaveGetHighCbs(void);


int gpioWaveGetMaxCbs(void);


int gpioSerialReadOpen(unsigned user_gpio, unsigned baud, unsigned data_bits);

int gpioSerialReadInvert(unsigned user_gpio, unsigned invert);


int gpioSerialRead(unsigned user_gpio, void *buf, size_t bufSize);


int gpioSerialReadClose(unsigned user_gpio);


inline int i2cOpen( unsigned i2cBus, unsigned i2cAddr, unsigned i2cFlags )
{
    return i2c_open( gGpioId, i2cBus, i2cAddr, i2cFlags );
}


inline int i2cClose( unsigned handle ) 
{
    return i2c_close( gGpioId, handle );
}


inline int i2cWriteQuick( unsigned handle, unsigned bit )
{
    return i2c_write_quick( gGpioId, handle, bit );
}


inline int i2cWriteByte( unsigned handle, unsigned bVal )
{
    return i2c_write_byte( gGpioId, handle, bVal );
}


inline int i2cReadByte( unsigned handle )
{
    return i2c_read_byte( gGpioId, handle );
}


inline int i2cWriteByteData( unsigned handle, unsigned i2cReg, unsigned bVal ) 
{
    return i2c_write_byte_data( gGpioId, handle, i2cReg, bVal );
}


inline int i2cWriteWordData( unsigned handle, unsigned i2cReg, unsigned wVal )
{
    return i2c_write_word_data( gGpioId, handle, i2cReg, wVal );
}


inline int i2cReadByteData( unsigned handle, unsigned i2cReg )
{
    return i2c_read_byte_data( gGpioId, handle, i2cReg );
}


inline int i2cReadWordData( unsigned handle, unsigned i2cReg )
{
    return i2c_read_word_data( gGpioId, handle, i2cReg );
}


inline int i2cProcessCall( unsigned handle, unsigned i2cReg, unsigned wVal )
{
    return i2c_process_call( gGpioId, handle, i2cReg, wVal );
}


inline int i2cWriteBlockData( unsigned handle, unsigned i2cReg, char *buf, unsigned count )
{
    return i2c_write_block_data( gGpioId, handle, i2cReg, buf, count );
}


inline int i2cReadBlockData( unsigned handle, unsigned i2cReg, char *buf )
{
    return i2c_read_block_data( gGpioId, handle, i2cReg, buf );
}


inline int i2cBlockProcessCall( unsigned handle, unsigned i2cReg, char *buf, unsigned count )
{
    return i2c_block_process_call( gGpioId, handle, i2cReg, buf, count );
}


inline int i2cReadI2CBlockData( unsigned handle, unsigned i2cReg, char *buf, unsigned count )
{
    return i2c_read_i2c_block_data( gGpioId, handle, i2cReg, buf, count );
}


inline int i2cWriteI2CBlockData( unsigned handle, unsigned i2cReg, char *buf, unsigned count )
{
    return i2c_write_i2c_block_data( gGpioId, handle, i2cReg, buf, count );
}


inline int i2cReadDevice( unsigned handle, char *buf, unsigned count )
{
    return i2c_read_device( gGpioId, handle, buf, count );
}


inline int i2cWriteDevice( unsigned handle, char *buf, unsigned count )
{
    return i2c_write_device( gGpioId, handle, buf, count );
}


/* void i2cSwitchCombined(int setting);  // Undefined in pigpiod */

/* int i2cSegments(unsigned handle, pi_i2c_msg_t *segs, unsigned numSegs); // Undefined in pigpiod */


inline int i2cZip( unsigned handle, char* inBuf, unsigned inLen, char* outBuf, unsigned outLen )
{
    return i2c_zip( gGpioId, handle, inBuf, inLen, outBuf, outLen );
}


int bbI2COpen(unsigned SDA, unsigned SCL, unsigned baud);

int bbI2CClose(unsigned SDA);

int bbI2CZip(
   unsigned SDA,
   char    *inBuf,
   unsigned inLen,
   char    *outBuf,
   unsigned outLen);

int bscXfer(bsc_xfer_t *bsc_xfer);

int bbSPIOpen(
   unsigned CS, unsigned MISO, unsigned MOSI, unsigned SCLK,
   unsigned baud, unsigned spiFlags);

int bbSPIClose(unsigned CS);

int bbSPIXfer(
   unsigned CS,
   char    *inBuf,
   char    *outBuf,
   unsigned count);

int spiOpen(unsigned spiChan, unsigned baud, unsigned spiFlags);

int spiClose(unsigned handle);


int spiRead(unsigned handle, char *buf, unsigned count);


int spiWrite(unsigned handle, char *buf, unsigned count);

int spiXfer(unsigned handle, char *txBuf, char *rxBuf, unsigned count);


int serOpen(char *sertty, unsigned baud, unsigned serFlags);


int serClose(unsigned handle);

int serWriteByte(unsigned handle, unsigned bVal);

int serReadByte(unsigned handle);

int serWrite(unsigned handle, char *buf, unsigned count);


int serRead(unsigned handle, char *buf, unsigned count);


int serDataAvailable(unsigned handle);


int gpioTrigger(unsigned user_gpio, unsigned pulseLen, unsigned level);


int gpioSetWatchdog(unsigned user_gpio, unsigned timeout);


int gpioNoiseFilter(unsigned user_gpio, unsigned steady, unsigned active);


int gpioGlitchFilter(unsigned user_gpio, unsigned steady);


int gpioSetGetSamplesFunc(gpioGetSamplesFunc_t f, uint32_t bits);


int gpioSetGetSamplesFuncEx(
   gpioGetSamplesFuncEx_t f, uint32_t bits, void *userdata);


int gpioSetTimerFunc(unsigned timer, unsigned millis, gpioTimerFunc_t f);


int gpioSetTimerFuncEx(
   unsigned timer, unsigned millis, gpioTimerFuncEx_t f, void *userdata);


pthread_t *gpioStartThread(gpioThreadFunc_t f, void *userdata);


void gpioStopThread(pthread_t *pth);


int gpioStoreScript(char *script);


int gpioRunScript(unsigned script_id, unsigned numPar, uint32_t *param);



int gpioUpdateScript(unsigned script_id, unsigned numPar, uint32_t *param);


int gpioScriptStatus(unsigned script_id, uint32_t *param);


int gpioStopScript(unsigned script_id);


int gpioDeleteScript(unsigned script_id);


int gpioSetSignalFunc(unsigned signum, gpioSignalFunc_t f);


int gpioSetSignalFuncEx(
   unsigned signum, gpioSignalFuncEx_t f, void *userdata);


uint32_t gpioRead_Bits_0_31(void);


uint32_t gpioRead_Bits_32_53(void);


int gpioWrite_Bits_0_31_Clear(uint32_t bits);


int gpioWrite_Bits_32_53_Clear(uint32_t bits);


int gpioWrite_Bits_0_31_Set(uint32_t bits);


int gpioWrite_Bits_32_53_Set(uint32_t bits);

int gpioHardwareClock(unsigned gpio, unsigned clkfreq);

int gpioHardwarePWM(unsigned gpio, unsigned PWMfreq, unsigned PWMduty);

int gpioTime(unsigned timetype, int *seconds, int *micros);


int gpioSleep(unsigned timetype, int seconds, int micros);


uint32_t gpioDelay(uint32_t micros);


uint32_t gpioTick(void);


unsigned gpioHardwareRevision(void);


unsigned gpioVersion(void);


int gpioGetPad(unsigned pad);


int gpioSetPad(unsigned pad, unsigned padStrength);

int eventMonitor(unsigned handle, uint32_t bits);

int eventSetFunc(unsigned event, eventFunc_t f);

int eventSetFuncEx(unsigned event, eventFuncEx_t f, void *userdata);

int eventTrigger(unsigned event);


int shell(char *scriptName, char *scriptString);

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wcomment"

int fileOpen(char *file, unsigned mode);

#pragma GCC diagnostic pop

int fileClose(unsigned handle);


int fileWrite(unsigned handle, char *buf, unsigned count);


int fileRead(unsigned handle, char *buf, unsigned count);


int fileSeek(unsigned handle, int32_t seekOffset, int seekFrom);

#pragma GCC diagnostic push

#pragma GCC diagnostic ignored "-Wcomment"

int fileList(char *fpat,  char *buf, unsigned count);

#pragma GCC diagnostic pop


int gpioCfgBufferSize(unsigned cfgMillis);


int gpioCfgClock(
   unsigned cfgMicros, unsigned cfgPeripheral, unsigned cfgSource);


int gpioCfgDMAchannel(unsigned DMAchannel);


int gpioCfgDMAchannels(unsigned primaryChannel, unsigned secondaryChannel);


int gpioCfgPermissions(uint64_t updateMask);


int gpioCfgSocketPort(unsigned port);


int gpioCfgInterfaces(unsigned ifFlags);


int gpioCfgMemAlloc(unsigned memAllocMode);


int gpioCfgNetAddr(int numSockAddr, uint32_t *sockAddr);


uint32_t gpioCfgGetInternals(void);

int gpioCfgSetInternals(uint32_t cfgVal);


int gpioCustom1(unsigned arg1, unsigned arg2, char *argx, unsigned argc);


int gpioCustom2(unsigned arg1, char *argx, unsigned argc,
                char *retBuf, unsigned retMax);


int rawWaveAddSPI(
   rawSPI_t *spi,
   unsigned offset,
   unsigned spiSS,
   char *buf,
   unsigned spiTxBits,
   unsigned spiBitFirst,
   unsigned spiBitLast,
   unsigned spiBits);

int rawWaveAddGeneric(unsigned numPulses, rawWave_t *pulses);

unsigned rawWaveCB(void);

rawCbs_t *rawWaveCBAdr(int cbNum);

uint32_t rawWaveGetOOL(int pos);


void rawWaveSetOOL(int pos, uint32_t lVal);

uint32_t rawWaveGetOut(int pos);


void rawWaveSetOut(int pos, uint32_t lVal);

uint32_t rawWaveGetIn(int pos);


void rawWaveSetIn(int pos, uint32_t lVal);

rawWaveInfo_t rawWaveInfo(int wave_id);

int getBitInBytes(int bitPos, char *buf, int numBits);

void putBitInBytes(int bitPos, char *buf, int bit);

double time_time(void);


void time_sleep(double seconds);


void rawDumpWave(void);


void rawDumpScript(unsigned script_id);


}

#endif

