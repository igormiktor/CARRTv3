#include "BNO055/bno055.h"


#include "pico/binary_info.h"
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/util/queue.h"
#include "hardware/clocks.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/uart.h"


bi_decl( bi_1pin_with_name( CARRTPICO_HEARTBEAT_LED, "On-board LED for blinking" ) );
bi_decl( bi_2pins_with_names( CARRTPICO_SERIAL_LINK_UART_TX_PIN, "uart1 (data) TX", CARRTPICO_SERIAL_LINK_UART_RX_PIN, "uart1 (data) RX" ) );
bi_decl( bi_2pins_with_names( CARRTPICO_I2C_SDA, "i2c0 SDA", CARRTPICO_I2C_SCL, "i2c0 SCL" ) );


int main()
{
    stdio_init_all();


    return 0;
}
