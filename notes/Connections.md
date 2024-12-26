# Wiring Connections

## Rapsberry Zero

| To | Function          | Pin | | Pin | Function | To |
| :---: |  :---: | :---: | :---: | :---: | :---:  | :---: |
| Motor Board  | 3V3     |  1 |  |  2 | 5V        | PowerBoost (in) |
| I2C Bus Zero | I2C SDA |  3 |  |  4 | 5V        | Motor Board |
| I2C Bus Zero | I2C SCL |  5 |  |  6 | GND       | Common Gnd |
|              |         |  7 |  |  8 | UART TX   | Pico UART RX |
| PowerBoost   | Gnd     |  9 |  | 10 | UART RX   | Pico UART TX |
| Pico On/Off 2N2222 base | GPIO17  | 11 |  | 12 |  |  |
|              |         | 13 |  | 14 |  |  |
|              |         | 15 |  | 16 | GPIO23 | Monitor PowerBoost LPO for battery low |
|              |         | 17 |  | 18 |  |  |
|              |         | 19 |  | 20 |  |  |
|              |         | 21 |  | 22 |  |  |
|              |         | 23 |  | 24 |  |  |
|              |         | 25 |  | 26 |  |  |
|              |         | 27 |  | 28 |  |  |
|              |         | 29 |  | 30 |  |  |
|              |         | 31 |  | 32 |  |  |
|              |         | 33 |  | 34 |  |  |
|              |         | 35 |  | 36 | GPIO16    | Shutdown Button |
|              |         | 37 |  | 38 |  |  |
| Motor Board  |   Gnd   | 39 |  | 40 |  |  |


## Rapsberry Pico

| To    | Function        | Pin | | Pin | Function | To |
| :---: |  :---: | :---: | :---: | :---: | :---: | :---: |
|              |          |  1 |  | 40 | VBUS      |            |
|              |          |  2 |  | 39 | VSYS      | 5V Motor Board via 1N4001 diode |
|              |          |  3 |  | 38 | Gnd       | Gnd via 2N2222 controlled by Zero GPIO17 |
| I2C BUS Pico | I2C1 SDA |  4 |  | 37 |           |            |
| I2C BUS Pico | I2C1 SCL |  5 |  | 36 | 3V3       | BNO055 |
| Zero UART RX | UART1 TX |  6 |  | 35 |           |        |
| Zero UART TX | UART1 RX |  7 |  | 34 |           |        |
|              |          |  8 |  | 33 | Gnd       | BNO055 |
|              |          |  9 |  | 32 |  |  |
|              |          | 10 |  | 31 |  |  |
|              |          | 11 |  | 30 |  |  |
|              |          | 12 |  | 29 |  |  |
|              |          | 13 |  | 28 |  |  |
|              |          | 14 |  | 27 |  |  |
|              |          | 15 |  | 26 |  |  |
|              |          | 16 |  | 25 |  |  |
|              |          | 17 |  | 24 |  |  |
|              |          | 18 |  | 23 |  |  |
|              |          | 19 |  | 22 |  |  |
|              |          | 20 |  | 21 |  |  |



## Adafruit PowerBoost 1000C

| Pin   | Function                                    | To |
| :---: | :---:                                       | :---: |
| USB   | USB 5V (charge in)                          |  |
| Bat   | Battery Input (3.0-4.2V)                    |  |
| Vs    | Max(USB, LiPo)                              |  |
| EN    | Enable (Ground to disable)                  | On/Off Switch |
| GND   | Gound                                       |  |
| LBO   | Low Battery Signal (toggles from Bat to OV) | (not 3.3V safe!) |
| GND   | Ground                                      | RPi Zero Gnd |
| 5Vo   | Output 5V (5.2-5.0 V)                       | RPi Zero 5V  |
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
