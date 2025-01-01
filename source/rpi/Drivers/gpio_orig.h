
#ifndef PIGPIO_H
#define PIGPIO_H

#include <stddef.h>
#include <stdint.h>
#include <pthread.h>

#define PIGPIO_VERSION 79



#define PI_INPFIFO "/dev/pigpio"
#define PI_OUTFIFO "/dev/pigout"
#define PI_ERRFIFO "/dev/pigerr"

#define PI_ENVPORT "PIGPIO_PORT"
#define PI_ENVADDR "PIGPIO_ADDR"

#define PI_LOCKFILE "/var/run/pigpio.pid"

#define PI_I2C_COMBINED "/sys/module/i2c_bcm2708/parameters/combined"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct
{
   uint16_t func;
   uint16_t size;
} gpioHeader_t;

typedef struct
{
   size_t size;
   void *ptr;
   uint32_t data;
} gpioExtent_t;

typedef struct
{
   uint32_t tick;
   uint32_t level;
} gpioSample_t;

typedef struct
{
   uint16_t seqno;
   uint16_t flags;
   uint32_t tick;
   uint32_t level;
} gpioReport_t;

typedef struct
{
   uint32_t gpioOn;
   uint32_t gpioOff;
   uint32_t usDelay;
} gpioPulse_t;

#define WAVE_FLAG_READ  1
#define WAVE_FLAG_TICK  2

typedef struct
{
   uint32_t gpioOn;
   uint32_t gpioOff;
   uint32_t usDelay;
   uint32_t flags;
} rawWave_t;


typedef struct
{
   uint16_t botCB;
   uint16_t topCB;
   uint16_t botOOL;
   uint16_t topOOL;
   uint16_t deleted;
   uint16_t numCB;
   uint16_t numBOOL;
   uint16_t numTOOL;
} rawWaveInfo_t;

typedef struct
{
   int clk;
   int mosi;
   int miso;
   int ss_pol;
   int ss_us;
   int clk_pol;
   int clk_pha;
   int clk_us;
} rawSPI_t;

typedef struct {
   uint32_t info;
   uint32_t src;
   uint32_t dst;
   uint32_t length;
   uint32_t stride;
   uint32_t next;
   uint32_t pad[2];
} rawCbs_t;

typedef struct
{
   uint16_t addr;
   uint16_t flags;
   uint16_t len;
   uint8_t  *buf;
} pi_i2c_msg_t;


#define BSC_FIFO_SIZE 512

typedef struct
{
   uint32_t control;
   int rxCnt;
   char rxBuf[BSC_FIFO_SIZE];
   int txCnt;
   char txBuf[BSC_FIFO_SIZE];
} bsc_xfer_t;


typedef void (*gpioAlertFunc_t)    (int      gpio,
                                    int      level,
                                    uint32_t tick);

typedef void (*gpioAlertFuncEx_t)  (int      gpio,
                                    int      level,
                                    uint32_t tick,
                                    void    *userdata);

typedef void (*eventFunc_t)        (int      event,
                                    uint32_t tick);

typedef void (*eventFuncEx_t)      (int      event,
                                    uint32_t tick,
                                    void    *userdata);

typedef void (*gpioISRFunc_t)      (int      gpio,
                                    int      level,
                                    uint32_t tick);

typedef void (*gpioISRFuncEx_t)    (int      gpio,
                                    int      level,
                                    uint32_t tick,
                                    void    *userdata);

typedef void (*gpioTimerFunc_t)    (void);

typedef void (*gpioTimerFuncEx_t)  (void *userdata);

typedef void (*gpioSignalFunc_t)   (int signum);

typedef void (*gpioSignalFuncEx_t) (int    signum,
                                    void  *userdata);

typedef void (*gpioGetSamplesFunc_t)   (const gpioSample_t *samples,
                                        int                 numSamples);

typedef void (*gpioGetSamplesFuncEx_t) (const gpioSample_t *samples,
                                        int                 numSamples,
                                        void               *userdata);

typedef void *(gpioThreadFunc_t) (void *);



#define PI_MIN_GPIO       0
#define PI_MAX_GPIO      53


#define PI_MAX_USER_GPIO 31


#define PI_OFF   0
#define PI_ON    1

#define PI_CLEAR 0
#define PI_SET   1

#define PI_LOW   0
#define PI_HIGH  1


#define PI_TIMEOUT 2


#define PI_INPUT  0
#define PI_OUTPUT 1
#define PI_ALT0   4
#define PI_ALT1   5
#define PI_ALT2   6
#define PI_ALT3   7
#define PI_ALT4   3
#define PI_ALT5   2


#define PI_PUD_OFF  0
#define PI_PUD_DOWN 1
#define PI_PUD_UP   2


#define PI_DEFAULT_DUTYCYCLE_RANGE   255


#define PI_MIN_DUTYCYCLE_RANGE        25
#define PI_MAX_DUTYCYCLE_RANGE     40000


#define PI_SERVO_OFF 0
#define PI_MIN_SERVO_PULSEWIDTH 500
#define PI_MAX_SERVO_PULSEWIDTH 2500


#define PI_HW_PWM_MIN_FREQ 1
#define PI_HW_PWM_MAX_FREQ      125000000
#define PI_HW_PWM_MAX_FREQ_2711 187500000
#define PI_HW_PWM_RANGE 1000000


#define PI_HW_CLK_MIN_FREQ       4689
#define PI_HW_CLK_MIN_FREQ_2711 13184
#define PI_HW_CLK_MAX_FREQ      250000000
#define PI_HW_CLK_MAX_FREQ_2711 375000000

#define PI_NOTIFY_SLOTS  32

#define PI_NTFY_FLAGS_EVENT    (1 <<7)
#define PI_NTFY_FLAGS_ALIVE    (1 <<6)
#define PI_NTFY_FLAGS_WDOG     (1 <<5)
#define PI_NTFY_FLAGS_BIT(x) (((x)<<0)&31)

#define PI_WAVE_BLOCKS     4
#define PI_WAVE_MAX_PULSES (PI_WAVE_BLOCKS * 3000)
#define PI_WAVE_MAX_CHARS  (PI_WAVE_BLOCKS *  300)

#define PI_BB_I2C_MIN_BAUD     50
#define PI_BB_I2C_MAX_BAUD 500000

#define PI_BB_SPI_MIN_BAUD     50
#define PI_BB_SPI_MAX_BAUD 250000

#define PI_BB_SER_MIN_BAUD     50
#define PI_BB_SER_MAX_BAUD 250000

#define PI_BB_SER_NORMAL 0
#define PI_BB_SER_INVERT 1

#define PI_WAVE_MIN_BAUD      50
#define PI_WAVE_MAX_BAUD 1000000

#define PI_SPI_MIN_BAUD     32000
#define PI_SPI_MAX_BAUD 125000000

#define PI_MIN_WAVE_DATABITS 1
#define PI_MAX_WAVE_DATABITS 32

#define PI_MIN_WAVE_HALFSTOPBITS 2
#define PI_MAX_WAVE_HALFSTOPBITS 8

#define PI_WAVE_MAX_MICROS (30 * 60 * 1000000)

#define PI_MAX_WAVES 250

#define PI_MAX_WAVE_CYCLES 65535
#define PI_MAX_WAVE_DELAY  65535

#define PI_WAVE_COUNT_PAGES 10


#define PI_WAVE_MODE_ONE_SHOT      0
#define PI_WAVE_MODE_REPEAT        1
#define PI_WAVE_MODE_ONE_SHOT_SYNC 2
#define PI_WAVE_MODE_REPEAT_SYNC   3


#define PI_WAVE_NOT_FOUND  9998
#define PI_NO_TX_WAVE      9999


#define PI_FILE_SLOTS 16
#define PI_I2C_SLOTS  512
#define PI_SPI_SLOTS  32
#define PI_SER_SLOTS  16

#define PI_MAX_I2C_ADDR 0x7F

#define PI_NUM_AUX_SPI_CHANNEL 3
#define PI_NUM_STD_SPI_CHANNEL 2

#define PI_MAX_I2C_DEVICE_COUNT (1<<16)
#define PI_MAX_SPI_DEVICE_COUNT (1<<16)


#define  PI_I2C_RDRW_IOCTL_MAX_MSGS 42


#define PI_I2C_M_WR           0x0000
#define PI_I2C_M_RD           0x0001
#define PI_I2C_M_TEN          0x0010
#define PI_I2C_M_RECV_LEN     0x0400
#define PI_I2C_M_NO_RD_ACK    0x0800
#define PI_I2C_M_IGNORE_NAK   0x1000
#define PI_I2C_M_REV_DIR_ADDR 0x2000
#define PI_I2C_M_NOSTART      0x4000


#define PI_I2C_END          0
#define PI_I2C_ESC          1
#define PI_I2C_START        2
#define PI_I2C_COMBINED_ON  2
#define PI_I2C_STOP         3
#define PI_I2C_COMBINED_OFF 3
#define PI_I2C_ADDR         4
#define PI_I2C_FLAGS        5
#define PI_I2C_READ         6
#define PI_I2C_WRITE        7


#define PI_SPI_FLAGS_BITLEN(x) ((x&63)<<16)
#define PI_SPI_FLAGS_RX_LSB(x)  ((x&1)<<15)
#define PI_SPI_FLAGS_TX_LSB(x)  ((x&1)<<14)
#define PI_SPI_FLAGS_3WREN(x)  ((x&15)<<10)
#define PI_SPI_FLAGS_3WIRE(x)   ((x&1)<<9)
#define PI_SPI_FLAGS_AUX_SPI(x) ((x&1)<<8)
#define PI_SPI_FLAGS_RESVD(x)   ((x&7)<<5)
#define PI_SPI_FLAGS_CSPOLS(x)  ((x&7)<<2)
#define PI_SPI_FLAGS_MODE(x)    ((x&3))


#define BSC_DR         0
#define BSC_RSR        1
#define BSC_SLV        2
#define BSC_CR         3
#define BSC_FR         4
#define BSC_IFLS       5
#define BSC_IMSC       6
#define BSC_RIS        7
#define BSC_MIS        8
#define BSC_ICR        9
#define BSC_DMACR     10
#define BSC_TDR       11
#define BSC_GPUSTAT   12
#define BSC_HCTRL     13
#define BSC_DEBUG_I2C 14
#define BSC_DEBUG_SPI 15

#define BSC_CR_TESTFIFO 2048
#define BSC_CR_RXE  512
#define BSC_CR_TXE  256
#define BSC_CR_BRK  128
#define BSC_CR_CPOL  16
#define BSC_CR_CPHA   8
#define BSC_CR_I2C    4
#define BSC_CR_SPI    2
#define BSC_CR_EN     1

#define BSC_FR_RXBUSY 32
#define BSC_FR_TXFE   16
#define BSC_FR_RXFF    8
#define BSC_FR_TXFF    4
#define BSC_FR_RXFE    2
#define BSC_FR_TXBUSY  1


#define BSC_SDA      18
#define BSC_MOSI     20
#define BSC_SCL_SCLK 19
#define BSC_MISO     18
#define BSC_CE_N     21

#define BSC_SDA_2711      10
#define BSC_MOSI_2711      9
#define BSC_SCL_SCLK_2711 11
#define BSC_MISO_2711     10
#define BSC_CE_N_2711      8


#define PI_MAX_BUSY_DELAY 100


#define PI_MIN_WDOG_TIMEOUT 0
#define PI_MAX_WDOG_TIMEOUT 60000


#define PI_MIN_TIMER 0
#define PI_MAX_TIMER 9


#define PI_MIN_MS 10
#define PI_MAX_MS 60000

#define PI_MAX_SCRIPTS       32

#define PI_MAX_SCRIPT_TAGS   50
#define PI_MAX_SCRIPT_VARS  150
#define PI_MAX_SCRIPT_PARAMS 10


#define PI_SCRIPT_INITING 0
#define PI_SCRIPT_HALTED  1
#define PI_SCRIPT_RUNNING 2
#define PI_SCRIPT_WAITING 3
#define PI_SCRIPT_FAILED  4


#define PI_MIN_SIGNUM 0
#define PI_MAX_SIGNUM 63


#define PI_TIME_RELATIVE 0
#define PI_TIME_ABSOLUTE 1

#define PI_MAX_MICS_DELAY 1000000
#define PI_MAX_MILS_DELAY 60000


#define PI_BUF_MILLIS_MIN 100
#define PI_BUF_MILLIS_MAX 10000



#define PI_CLOCK_PWM 0
#define PI_CLOCK_PCM 1


#define PI_MIN_DMA_CHANNEL 0
#define PI_MAX_DMA_CHANNEL 15


#define PI_MIN_SOCKET_PORT 1024
#define PI_MAX_SOCKET_PORT 32000



#define PI_DISABLE_FIFO_IF   1
#define PI_DISABLE_SOCK_IF   2
#define PI_LOCALHOST_SOCK_IF 4
#define PI_DISABLE_ALERT     8


#define PI_MEM_ALLOC_AUTO    0
#define PI_MEM_ALLOC_PAGEMAP 1
#define PI_MEM_ALLOC_MAILBOX 2


#define PI_MAX_STEADY  300000
#define PI_MAX_ACTIVE 1000000


#define PI_CFG_DBG_LEVEL         0
#define PI_CFG_ALERT_FREQ        4
#define PI_CFG_RT_PRIORITY       (1<<8)
#define PI_CFG_STATS             (1<<9)
#define PI_CFG_NOSIGHANDLER      (1<<10)

#define PI_CFG_ILLEGAL_VAL       (1<<11)



#define RISING_EDGE  0
#define FALLING_EDGE 1
#define EITHER_EDGE  2



#define PI_MAX_PAD 2

#define PI_MIN_PAD_STRENGTH 1
#define PI_MAX_PAD_STRENGTH 16


#define PI_FILE_NONE   0
#define PI_FILE_MIN    1
#define PI_FILE_READ   1
#define PI_FILE_WRITE  2
#define PI_FILE_RW     3
#define PI_FILE_APPEND 4
#define PI_FILE_CREATE 8
#define PI_FILE_TRUNC  16
#define PI_FILE_MAX    31

#define PI_FROM_START   0
#define PI_FROM_CURRENT 1
#define PI_FROM_END     2


#define MAX_CONNECT_ADDRESSES 256


#define PI_MAX_EVENT 31


#define PI_EVENT_BSC 31

int gpioInitialise(void);


void gpioTerminate(void);


int gpioSetMode(unsigned gpio, unsigned mode);


int gpioGetMode(unsigned gpio);


int gpioSetPullUpDown(unsigned gpio, unsigned pud);


int gpioRead (unsigned gpio);


int gpioWrite(unsigned gpio, unsigned level);


int gpioPWM(unsigned user_gpio, unsigned dutycycle);


int gpioGetPWMdutycycle(unsigned user_gpio);


int gpioSetPWMrange(unsigned user_gpio, unsigned range);


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

int i2cOpen(unsigned i2cBus, unsigned i2cAddr, unsigned i2cFlags);


int i2cClose(unsigned handle);


int i2cWriteQuick(unsigned handle, unsigned bit);


int i2cWriteByte(unsigned handle, unsigned bVal);


int i2cReadByte(unsigned handle);


int i2cWriteByteData(unsigned handle, unsigned i2cReg, unsigned bVal);


int i2cWriteWordData(unsigned handle, unsigned i2cReg, unsigned wVal);


int i2cReadByteData(unsigned handle, unsigned i2cReg);


int i2cReadWordData(unsigned handle, unsigned i2cReg);


int i2cProcessCall(unsigned handle, unsigned i2cReg, unsigned wVal);


int i2cWriteBlockData(
unsigned handle, unsigned i2cReg, char *buf, unsigned count);


int i2cReadBlockData(unsigned handle, unsigned i2cReg, char *buf);


int i2cBlockProcessCall(
unsigned handle, unsigned i2cReg, char *buf, unsigned count);


int i2cReadI2CBlockData(
unsigned handle, unsigned i2cReg, char *buf, unsigned count);


int i2cWriteI2CBlockData(
unsigned handle, unsigned i2cReg, char *buf, unsigned count);

int i2cReadDevice(unsigned handle, char *buf, unsigned count);


int i2cWriteDevice(unsigned handle, char *buf, unsigned count);

void i2cSwitchCombined(int setting);

int i2cSegments(unsigned handle, pi_i2c_msg_t *segs, unsigned numSegs);

int i2cZip(
   unsigned handle,
   char    *inBuf,
   unsigned inLen,
   char    *outBuf,
   unsigned outLen);

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


#ifdef __cplusplus
}
#endif



#define PI_CMD_MODES  0
#define PI_CMD_MODEG  1
#define PI_CMD_PUD    2
#define PI_CMD_READ   3
#define PI_CMD_WRITE  4
#define PI_CMD_PWM    5
#define PI_CMD_PRS    6
#define PI_CMD_PFS    7
#define PI_CMD_SERVO  8
#define PI_CMD_WDOG   9
#define PI_CMD_BR1   10
#define PI_CMD_BR2   11
#define PI_CMD_BC1   12
#define PI_CMD_BC2   13
#define PI_CMD_BS1   14
#define PI_CMD_BS2   15
#define PI_CMD_TICK  16
#define PI_CMD_HWVER 17
#define PI_CMD_NO    18
#define PI_CMD_NB    19
#define PI_CMD_NP    20
#define PI_CMD_NC    21
#define PI_CMD_PRG   22
#define PI_CMD_PFG   23
#define PI_CMD_PRRG  24
#define PI_CMD_HELP  25
#define PI_CMD_PIGPV 26
#define PI_CMD_WVCLR 27
#define PI_CMD_WVAG  28
#define PI_CMD_WVAS  29
#define PI_CMD_WVGO  30
#define PI_CMD_WVGOR 31
#define PI_CMD_WVBSY 32
#define PI_CMD_WVHLT 33
#define PI_CMD_WVSM  34
#define PI_CMD_WVSP  35
#define PI_CMD_WVSC  36
#define PI_CMD_TRIG  37
#define PI_CMD_PROC  38
#define PI_CMD_PROCD 39
#define PI_CMD_PROCR 40
#define PI_CMD_PROCS 41
#define PI_CMD_SLRO  42
#define PI_CMD_SLR   43
#define PI_CMD_SLRC  44
#define PI_CMD_PROCP 45
#define PI_CMD_MICS  46
#define PI_CMD_MILS  47
#define PI_CMD_PARSE 48
#define PI_CMD_WVCRE 49
#define PI_CMD_WVDEL 50
#define PI_CMD_WVTX  51
#define PI_CMD_WVTXR 52
#define PI_CMD_WVNEW 53

#define PI_CMD_I2CO  54
#define PI_CMD_I2CC  55
#define PI_CMD_I2CRD 56
#define PI_CMD_I2CWD 57
#define PI_CMD_I2CWQ 58
#define PI_CMD_I2CRS 59
#define PI_CMD_I2CWS 60
#define PI_CMD_I2CRB 61
#define PI_CMD_I2CWB 62
#define PI_CMD_I2CRW 63
#define PI_CMD_I2CWW 64
#define PI_CMD_I2CRK 65
#define PI_CMD_I2CWK 66
#define PI_CMD_I2CRI 67
#define PI_CMD_I2CWI 68
#define PI_CMD_I2CPC 69
#define PI_CMD_I2CPK 70

#define PI_CMD_SPIO  71
#define PI_CMD_SPIC  72
#define PI_CMD_SPIR  73
#define PI_CMD_SPIW  74
#define PI_CMD_SPIX  75

#define PI_CMD_SERO  76
#define PI_CMD_SERC  77
#define PI_CMD_SERRB 78
#define PI_CMD_SERWB 79
#define PI_CMD_SERR  80
#define PI_CMD_SERW  81
#define PI_CMD_SERDA 82

#define PI_CMD_GDC   83
#define PI_CMD_GPW   84

#define PI_CMD_HC    85
#define PI_CMD_HP    86

#define PI_CMD_CF1   87
#define PI_CMD_CF2   88

#define PI_CMD_BI2CC 89
#define PI_CMD_BI2CO 90
#define PI_CMD_BI2CZ 91

#define PI_CMD_I2CZ  92

#define PI_CMD_WVCHA 93

#define PI_CMD_SLRI  94

#define PI_CMD_CGI   95
#define PI_CMD_CSI   96

#define PI_CMD_FG    97
#define PI_CMD_FN    98

#define PI_CMD_NOIB  99

#define PI_CMD_WVTXM 100
#define PI_CMD_WVTAT 101

#define PI_CMD_PADS  102
#define PI_CMD_PADG  103

#define PI_CMD_FO    104
#define PI_CMD_FC    105
#define PI_CMD_FR    106
#define PI_CMD_FW    107
#define PI_CMD_FS    108
#define PI_CMD_FL    109

#define PI_CMD_SHELL 110

#define PI_CMD_BSPIC 111
#define PI_CMD_BSPIO 112
#define PI_CMD_BSPIX 113

#define PI_CMD_BSCX  114

#define PI_CMD_EVM   115
#define PI_CMD_EVT   116

#define PI_CMD_PROCU 117
#define PI_CMD_WVCAP 118




#define PI_CMD_SCRIPT 800

#define PI_CMD_ADD   800
#define PI_CMD_AND   801
#define PI_CMD_CALL  802
#define PI_CMD_CMDR  803
#define PI_CMD_CMDW  804
#define PI_CMD_CMP   805
#define PI_CMD_DCR   806
#define PI_CMD_DCRA  807
#define PI_CMD_DIV   808
#define PI_CMD_HALT  809
#define PI_CMD_INR   810
#define PI_CMD_INRA  811
#define PI_CMD_JM    812
#define PI_CMD_JMP   813
#define PI_CMD_JNZ   814
#define PI_CMD_JP    815
#define PI_CMD_JZ    816
#define PI_CMD_TAG   817
#define PI_CMD_LD    818
#define PI_CMD_LDA   819
#define PI_CMD_LDAB  820
#define PI_CMD_MLT   821
#define PI_CMD_MOD   822
#define PI_CMD_NOP   823
#define PI_CMD_OR    824
#define PI_CMD_POP   825
#define PI_CMD_POPA  826
#define PI_CMD_PUSH  827
#define PI_CMD_PUSHA 828
#define PI_CMD_RET   829
#define PI_CMD_RL    830
#define PI_CMD_RLA   831
#define PI_CMD_RR    832
#define PI_CMD_RRA   833
#define PI_CMD_STA   834
#define PI_CMD_STAB  835
#define PI_CMD_SUB   836
#define PI_CMD_SYS   837
#define PI_CMD_WAIT  838
#define PI_CMD_X     839
#define PI_CMD_XA    840
#define PI_CMD_XOR   841
#define PI_CMD_EVTWT 842


#define PI_INIT_FAILED       -1
#define PI_BAD_USER_GPIO     -2
#define PI_BAD_GPIO          -3
#define PI_BAD_MODE          -4
#define PI_BAD_LEVEL         -5
#define PI_BAD_PUD           -6
#define PI_BAD_PULSEWIDTH    -7
#define PI_BAD_DUTYCYCLE     -8
#define PI_BAD_TIMER         -9
#define PI_BAD_MS           -10
#define PI_BAD_TIMETYPE     -11
#define PI_BAD_SECONDS      -12
#define PI_BAD_MICROS       -13
#define PI_TIMER_FAILED     -14
#define PI_BAD_WDOG_TIMEOUT -15
#define PI_NO_ALERT_FUNC    -16
#define PI_BAD_CLK_PERIPH   -17
#define PI_BAD_CLK_SOURCE   -18
#define PI_BAD_CLK_MICROS   -19
#define PI_BAD_BUF_MILLIS   -20
#define PI_BAD_DUTYRANGE    -21
#define PI_BAD_DUTY_RANGE   -21
#define PI_BAD_SIGNUM       -22
#define PI_BAD_PATHNAME     -23
#define PI_NO_HANDLE        -24
#define PI_BAD_HANDLE       -25
#define PI_BAD_IF_FLAGS     -26
#define PI_BAD_CHANNEL      -27
#define PI_BAD_PRIM_CHANNEL -27
#define PI_BAD_SOCKET_PORT  -28
#define PI_BAD_FIFO_COMMAND -29
#define PI_BAD_SECO_CHANNEL -30
#define PI_NOT_INITIALISED  -31
#define PI_INITIALISED      -32
#define PI_BAD_WAVE_MODE    -33
#define PI_BAD_CFG_INTERNAL -34
#define PI_BAD_WAVE_BAUD    -35
#define PI_TOO_MANY_PULSES  -36
#define PI_TOO_MANY_CHARS   -37
#define PI_NOT_SERIAL_GPIO  -38
#define PI_BAD_SERIAL_STRUC -39
#define PI_BAD_SERIAL_BUF   -40
#define PI_NOT_PERMITTED    -41
#define PI_SOME_PERMITTED   -42
#define PI_BAD_WVSC_COMMND  -43
#define PI_BAD_WVSM_COMMND  -44
#define PI_BAD_WVSP_COMMND  -45
#define PI_BAD_PULSELEN     -46
#define PI_BAD_SCRIPT       -47
#define PI_BAD_SCRIPT_ID    -48
#define PI_BAD_SER_OFFSET   -49
#define PI_GPIO_IN_USE      -50
#define PI_BAD_SERIAL_COUNT -51
#define PI_BAD_PARAM_NUM    -52
#define PI_DUP_TAG          -53
#define PI_TOO_MANY_TAGS    -54
#define PI_BAD_SCRIPT_CMD   -55
#define PI_BAD_VAR_NUM      -56
#define PI_NO_SCRIPT_ROOM   -57
#define PI_NO_MEMORY        -58
#define PI_SOCK_READ_FAILED -59
#define PI_SOCK_WRIT_FAILED -60
#define PI_TOO_MANY_PARAM   -61
#define PI_NOT_HALTED       -62
#define PI_SCRIPT_NOT_READY -62
#define PI_BAD_TAG          -63
#define PI_BAD_MICS_DELAY   -64
#define PI_BAD_MILS_DELAY   -65
#define PI_BAD_WAVE_ID      -66
#define PI_TOO_MANY_CBS     -67
#define PI_TOO_MANY_OOL     -68
#define PI_EMPTY_WAVEFORM   -69
#define PI_NO_WAVEFORM_ID   -70
#define PI_I2C_OPEN_FAILED  -71
#define PI_SER_OPEN_FAILED  -72
#define PI_SPI_OPEN_FAILED  -73
#define PI_BAD_I2C_BUS      -74
#define PI_BAD_I2C_ADDR     -75
#define PI_BAD_SPI_CHANNEL  -76
#define PI_BAD_FLAGS        -77
#define PI_BAD_SPI_SPEED    -78
#define PI_BAD_SER_DEVICE   -79
#define PI_BAD_SER_SPEED    -80
#define PI_BAD_PARAM        -81
#define PI_I2C_WRITE_FAILED -82
#define PI_I2C_READ_FAILED  -83
#define PI_BAD_SPI_COUNT    -84
#define PI_SER_WRITE_FAILED -85
#define PI_SER_READ_FAILED  -86
#define PI_SER_READ_NO_DATA -87
#define PI_UNKNOWN_COMMAND  -88
#define PI_SPI_XFER_FAILED  -89
#define PI_BAD_POINTER      -90
#define PI_NO_AUX_SPI       -91
#define PI_NOT_PWM_GPIO     -92
#define PI_NOT_SERVO_GPIO   -93
#define PI_NOT_HCLK_GPIO    -94
#define PI_NOT_HPWM_GPIO    -95
#define PI_BAD_HPWM_FREQ    -96
#define PI_BAD_HPWM_DUTY    -97
#define PI_BAD_HCLK_FREQ    -98
#define PI_BAD_HCLK_PASS    -99
#define PI_HPWM_ILLEGAL    -100
#define PI_BAD_DATABITS    -101
#define PI_BAD_STOPBITS    -102
#define PI_MSG_TOOBIG      -103
#define PI_BAD_MALLOC_MODE -104
#define PI_TOO_MANY_SEGS   -105
#define PI_BAD_I2C_SEG     -106
#define PI_BAD_SMBUS_CMD   -107
#define PI_NOT_I2C_GPIO    -108
#define PI_BAD_I2C_WLEN    -109
#define PI_BAD_I2C_RLEN    -110
#define PI_BAD_I2C_CMD     -111
#define PI_BAD_I2C_BAUD    -112
#define PI_CHAIN_LOOP_CNT  -113
#define PI_BAD_CHAIN_LOOP  -114
#define PI_CHAIN_COUNTER   -115
#define PI_BAD_CHAIN_CMD   -116
#define PI_BAD_CHAIN_DELAY -117
#define PI_CHAIN_NESTING   -118
#define PI_CHAIN_TOO_BIG   -119
#define PI_DEPRECATED      -120
#define PI_BAD_SER_INVERT  -121
#define PI_BAD_EDGE        -122
#define PI_BAD_ISR_INIT    -123
#define PI_BAD_FOREVER     -124
#define PI_BAD_FILTER      -125
#define PI_BAD_PAD         -126
#define PI_BAD_STRENGTH    -127
#define PI_FIL_OPEN_FAILED -128
#define PI_BAD_FILE_MODE   -129
#define PI_BAD_FILE_FLAG   -130
#define PI_BAD_FILE_READ   -131
#define PI_BAD_FILE_WRITE  -132
#define PI_FILE_NOT_ROPEN  -133
#define PI_FILE_NOT_WOPEN  -134
#define PI_BAD_FILE_SEEK   -135
#define PI_NO_FILE_MATCH   -136
#define PI_NO_FILE_ACCESS  -137
#define PI_FILE_IS_A_DIR   -138
#define PI_BAD_SHELL_STATUS -139
#define PI_BAD_SCRIPT_NAME -140
#define PI_BAD_SPI_BAUD    -141
#define PI_NOT_SPI_GPIO    -142
#define PI_BAD_EVENT_ID    -143
#define PI_CMD_INTERRUPTED -144
#define PI_NOT_ON_BCM2711  -145
#define PI_ONLY_ON_BCM2711 -146

#define PI_PIGIF_ERR_0    -2000
#define PI_PIGIF_ERR_99   -2099

#define PI_CUSTOM_ERR_0   -3000
#define PI_CUSTOM_ERR_999 -3999



#define PI_DEFAULT_BUFFER_MILLIS           120
#define PI_DEFAULT_CLK_MICROS              5
#define PI_DEFAULT_CLK_PERIPHERAL          PI_CLOCK_PCM
#define PI_DEFAULT_IF_FLAGS                0
#define PI_DEFAULT_FOREGROUND              0
#define PI_DEFAULT_DMA_CHANNEL             14
#define PI_DEFAULT_DMA_PRIMARY_CHANNEL     14
#define PI_DEFAULT_DMA_SECONDARY_CHANNEL   6
#define PI_DEFAULT_DMA_PRIMARY_CH_2711     7
#define PI_DEFAULT_DMA_SECONDARY_CH_2711   6
#define PI_DEFAULT_DMA_NOT_SET             15
#define PI_DEFAULT_SOCKET_PORT             8888
#define PI_DEFAULT_SOCKET_PORT_STR         "8888"
#define PI_DEFAULT_SOCKET_ADDR_STR         "localhost"
#define PI_DEFAULT_UPDATE_MASK_UNKNOWN     0x0000000FFFFFFCLL
#define PI_DEFAULT_UPDATE_MASK_B1          0x03E7CF93
#define PI_DEFAULT_UPDATE_MASK_A_B2        0xFBC7CF9C
#define PI_DEFAULT_UPDATE_MASK_APLUS_BPLUS 0x0080480FFFFFFCLL
#define PI_DEFAULT_UPDATE_MASK_ZERO        0x0080000FFFFFFCLL
#define PI_DEFAULT_UPDATE_MASK_PI2B        0x0080480FFFFFFCLL
#define PI_DEFAULT_UPDATE_MASK_PI3B        0x0000000FFFFFFCLL
#define PI_DEFAULT_UPDATE_MASK_PI4B        0x0000000FFFFFFCLL
#define PI_DEFAULT_UPDATE_MASK_COMPUTE     0x00FFFFFFFFFFFFLL
#define PI_DEFAULT_MEM_ALLOC_MODE          PI_MEM_ALLOC_AUTO

#define PI_DEFAULT_CFG_INTERNALS           0


#endif