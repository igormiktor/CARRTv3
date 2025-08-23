# Wiring Connections

## Rapsberry Zero

| To | Function          | Pin | | Pin | Function | To |
| :---: |  :---: | :---: | :---: | :---: | :---:  | :---: |
| Other 3V3 ICs            | 3V3     |  1 |  |  2 | 5V (in)   | PowerBoost 5V out |
| I2C Bus RPi0             | I2C SDA |  3 |  |  4 |           |  |
| I2C Bus RPi0             | I2C SCL |  5 |  |  6 |           | |
|                          |         |  7 |  |  8 | UART TX   | Pico UART RX |
|                          |         |  9 |  | 10 | UART RX   | Pico UART TX |
| Pico On/Off 2N2222 base  | GPIO17  | 11 |  | 12 |  |  |
|                          |         | 13 |  | 14 | GND       | TXB0104 GND |
|                          |         | 15 |  | 16 | GPIO23    | Monitor PowerBoost LPO for battery low |
|  TXB0104 OE Pin          | 3V3     | 17 |  | 18 |  |  |
|                          |         | 19 |  | 20 |  |  |
|                          |         | 21 |  | 22 |  |  |
|                          |         | 23 |  | 24 |  |  |
|                          |         | 25 |  | 26 |  |  |
|                          |         | 27 |  | 28 |  |  |
|                          |         | 29 |  | 30 |  |  |
|                          |         | 31 |  | 32 | PWM0      | Buzzer |
|                          |         | 33 |  | 34 | GND       | Common Gnd |
|                          |         | 35 |  | 36 | GPIO16    | Shutdown Button |
|                          |         | 37 |  | 38 |  |  |
|                          |         | 39 |  | 40 |  |  |


## Rapsberry Pico

| To    | Function        | Pin | | Pin | Function | To |
| :---: |  :---: | :---: | :---: | :---: | :---: | :---: |
|  Debug RX (Black/Yellow) | UART0 TX |  1 |  | 40 | VBUS      |            |
|  Debug TX (White/Orange) | UART0 RX |  2 |  | 39 | VSYS      | 5V Motor Board via 1N4001 diode |
|  Debug Gnd (Red/Black)   | Gnd      |  3 |  | 38 | Gnd       | BNO055  |
| I2C Bus Pico             | I2C1 SDA |  4 |  | 37 |           |            |
| I2C Bus Pico             | I2C1 SCL |  5 |  | 36 | 3V3       | BNO055 |
| RPi0 UART RX             | UART1 TX |  6 |  | 35 |           |        |
| RPi0 UART TX             | UART1 RX |  7 |  | 34 |           |        |
|                          |          |  8 |  | 33 |  |  |
|                          |          |  9 |  | 32 | ADC1      | ADC to PowerBoost Batt Pin (via V-Divider) |
|                          |          | 10 |  | 31 | ADC0      | ADC to 9V Batt+ (via V-Divider) |
|                          |          | 11 |  | 30 |  |  |
|                          |          | 12 |  | 29 |  |  |
|                          |          | 13 |  | 28 |  |  |
|                          |          | 14 |  | 27 |  |  |
|                          |          | 15 |  | 26 |  |  |
|                          |          | 16 |  | 25 |  |  |
|                          |          | 17 |  | 24 |  |  |
|                          |          | 18 |  | 23 | Gnd       | Gnd via 2N2222 controlled by Zero GPIO17 |
| Left Encoder             | GPIO14   | 19 |  | 22 |  |  |
| Right Encoder            | GPIO15   | 20 |  | 21 |  |  |

### Rapsberry Pico SWD (Debug) Pins
(In order from Pin 20 side to Pin 21 side)
| To    | Function        | Pin |
| :---: |  :---: | :---: | 
| Debug CLK (Orange) | SWCLK |  SWD1 | 
| Debug Gnd (Black)  | GND   |  SWD2 | 
| Debug IO (Yellow)  | SWIO  |  SWD3 |  


## Adafruit PowerBoost 1000C

| Pin   | Function                                    | To |
| :---: | :---:                                       | :---: |
| USB   | USB 5V (charge in)                          |  |
| Bat   | Battery Input (3.0-4.2V)                    | Pico pin 32 (ADC1) via V-Divider |
| Vs    | Max(USB, LiPo)                              |  |
| EN    | Enable (Ground to disable)                  | On/Off Switch |
| GND   | Gound                                       |  |
| LBO   | Low Battery Signal (toggles from Bat to 0V) | (not 3.3V safe!) RPi0 pin 16 (GPIO23) via TXB0104 |
| GND   | Ground                                      | Common Gnd |
| 5Vo   | Output 5V (5.2-5.0 V)                       | Common IC 5V rail  |
|


## BNO055

| Pin   | Function                                    | To |
| :---: | :---:                                       | :---: |
| Vin   | Power in (3.3-5.0V)                         | Pico 3V3 |
| 3vo   | 3.3V output                                 |  |
| GND   | Ground                                      | Pico Gnd |
| SDA   | I2C data                                    | Pico I2C1 SDA |
| SCL   | I2C clock                                   | Pico I2C1 SCL |
| RST   | HW reset (set low, then high)               |  |
| PS0   | Mode change pin 0                           |  |
| PS1   | Mode change pin 1                           |  |
| INT   | HW interrupt (goes high 3V)                 |  |
| ADR   | I2C address (default = 0x29; high = 0x28)   |  |
