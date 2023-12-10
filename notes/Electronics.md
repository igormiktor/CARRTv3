# Electronics

## Wiring

| Color  | Purpose |
| :---:  | :---    |
| Black  | Ground |
| Red    | Power (5 V) |
| Orange | Power (3.3 V) |
| Blue   | I2C SDA |
| Yellow | I2C SCL |
| Green | PWM |
| White | GPIO (various) |

## Power Voltages

| Point of Measurement | Voltage |
| :--- | :---: |
| Power Boost 1000C Output (at posts) | 5.19 V |
| RPI Zero Output (at RPi Zero) | 5.14 V |
| On 5V Power Rail | 5.08 V |
| On 3.3V Power Rail | 3.25 V |
| Input to Pico (before diode) | 5.08 V |
| Input to Pico (after diode) | 4.3 V |
| RPI Pico Output (3V) | 3.3 V |
| Motor Batteries Output (at posts) | 10.4 V (8 batteries) |
| Motor Hat (at input) | 10.4 V (8 batteries) |
| Motor Batteries Output (at posts) | 7.8 V (6 batteries) |
| Motor Hat (at input) | 7.8 V (6 batteries) |

## Drive Motor Operations

| Operation     | Current           |
| :------------ | :-----------:     |
| Idle          | 0.0007 A          |
| Single motor start  | 0.3 - 0.6 A |
| Single motor run    | 0.2 - 0.3 A |
| All motor start     | 1.2 - 2.5 A |
| All motor run       | 0.8 - 0.9 A |

* Required switching transistor:  TIP120
* Required transistor base resistor:  3.3 KOhms

## Raspberry Pi Zero 2 W Operations

| Operation     | Current    |
| :------------ | :--------: |
| Idle          | 100 mA     |
| Start up      | 200 mA     |
| Computing     | 350 mA     |

* Switched directly by PowerBoost 1000C using a manual switch that grounds the Enable Pin on the PowerBoost.

## Raspberry Pico Operations

| Operation     | Current    |
| :------------ | :--------: |
| Idle          | 18 mA       |
| Start up      | 15 mA      |
| Max possible  | 200 mA     |

* Required switching transistor:  2N2222
* Required transistor base resistor:  2.2 KOhms

