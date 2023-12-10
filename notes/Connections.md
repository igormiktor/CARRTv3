# Wiring Connections

## Rapsberry Pi Zero 2 W

| To | Function          | Pin | | Pin | Function | To |
| :---: |  :---: | :---: | :---: | :---: | :---: | :---: |
| Motor Board  | 3V3     |  1 |  |  2 | 5V | PowerBoost (in) |
| I2C Bus Zero | I2C SDA |  3 |   |  4 | 5V | Motor Board |
| I2C Bus Zero | I2C SCL |  5 |  |  6 | GND | Common Gnd |
|              |         |  7 |  |  8 | UART TX | Pico UART RX |
| PowerBoost   | Gnd     |  9 |  | 10 | UART RX | Pico UART TX |
| Pico On/Off  | GPIO17  | 11 |  | 12 |  |  |
|              |         | 13 |  | 14 |  |  |
|              |         | 15 |  | 16 |  |  |
|              |         | 17 |  | 18 |  |  |
|              |         | 19 |  | 20 |  |  |
|              |         | 21 |  | 22 |  |  |
|              |         | 23 |  | 24 |  |  |
|              |         | 25 |  | 26 |  |  |
|              |         | 27 |  | 28 |  |  |
|              |         | 29 |  | 30 |  |  |
|              |         | 31 |  | 32 |  |  |
|              |         | 33 |  | 34 |  |  |
|              |         | 35 |  | 36 | GPIO16 | Shutdown Button |
|              |         | 37 |  | 38 |  |  |
| Motor Board  |   Gnd   | 39 |  | 40 |  |  |


## Rapsberry Pico

| To    | Function        | Pin | | Pin | Function | To |
| :---: |  :---: | :---: | :---: | :---: | :---: | :---: |
|              |          |  1 |  | 40 | VBUS   |            |
|              |          |  2 |  | 39 | VSYS   | 5V (in)    |
|              |          |  3 |  | 38 | Gnd    | Common Gnd |
| I2C BUS Pico | I2C0 SDA |  4 |  | 37 |        |            |
| I2C BUS Pico | I2C1 SCL |  5 |  | 36 | 3V3    | BNO055 |
| Zero UART RX | UART1 TX |  6 |  | 35 |        |        |
| Zero UART TX | UART1 RX |  7 |  | 34 |        |        |
|              |          |  8 |  | 33 | Gnd    | BNO055 |
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
