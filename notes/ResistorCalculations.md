# Resistor Calculations

## Motor Battery Level Monitoring

Need to monitor motor battery voltage (nominally 6 x 1.5V = 9V).  
We need to create a resistor divider to ensure the output is not over 3.3V.

### Range of motor battery voltages (6 x AA batteries)
| Type    | Full charge | Functionally Dead |
| :---:   |  :---:      | :---: | 
| Akaline | 9.9V        | 7.5V |
| NiMH    | 8.7V        | 7.2V |
| Lithium | 10.8V       | 7.8V |


* Input Full V = 8.7 - 10.8 V
* Input Dead V = 7.2 - 7.8 V
* R1 = 180K Ohm
* Output Full V = 2.72 - 3.38 V --> Pico Pin GPIO27
* Output Dead V = 2.35 - 2.44 V --> Pico Pin GPIO27
* R2 = 82K Ohm
* Ground 
* Current through bridge ~0.05 mA  

### Range of measured voltages (at Pico Pin GPIO27)

| Type    | Full charge | Functionally Dead |
| :---:   |  :---:      | :---: | 
! Akaline | 3.10V       | 2.35V |
| NiMH    | 2.72V       | 2.25V |
| Lithium | 3.38V       | 2.44V |


## PowerBoost 1000C LBO (Low Battery) Monitoring

LBO drops to 0V when PowerBoost detects battery low voltage (<3.2V).  
However, LBO is pulled up to Vin (up to 5.25V), 
so we need to create a resistor divider to ensure the output 
is always under 3.3V.

* Input V = 5.25 V   <-- PowerBoost LBO Pin
* R1 = 39K Ohm
* Output V = 3.3 V and 0.05 mA  --> Pico Pin GPIO26
* R2 = 68K Ohm
* Ground   

## Encoders

Encoders on the wheels are 5V devices, need to convert them to 3.3V.  
We use the TXB0104 to the the 5V -> 3V3 conversion.

We need two of these conversions (one pair of TXB0104 pins per encoder).

Encoder Pins:

* Pin 1 --> Signal pin (Green)
* Pin 2 <-- +5V power pin (Red)
* Pin 3 --> Gnd (Black)


## Pico power control (on/off from RPi Zero)

Use a 2N2222 transistor on the ground for the Pico to allow the RPi0 to turn it 
on and off

* 2N2222 collector <-- Pico GND
* 2N2222 base <-- 10 KOhm resistor <-- RPi0 GPIO17 (pin 11)
* 2N2222 emitter --> system GND 

Setting RPi0 GPIO17 (pin 11) LOW turns off the Pico; setting RPi0 GPIO17 HIGH turns on the Pico.




## Sources

Resistor calculator: [http://howardtechnical.com/voltage-divider-calculator/](http://howardtechnical.com/voltage-divider-calculator/)