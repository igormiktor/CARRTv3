# Resistor Calculations

## PowerBoost 1000C LBO

LBO drops to 0V when PowerBoost detects battery low voltage (<3.2V).  
However, LBO is pulled up to Vin (up to 5.25V), 
so we need to create a resistor divider to ensure the output 
is always under 3.3V.

* Input V = 5.25 V
* R1 = 1.5K Ohm
* Output V = 3.12 V and 1.4 mA
* R2 = 2.2K Ohm
* Ground

Note that when the input voltage drops to 3.2V, the output voltage is 1.9V (and at 0.86 mA).
This is sufficient voltage to drive RPi to read the gpio pin as a digital "1".

## Encoders

Encoders on the wheels are 5V devices, need to convert them to 3.3V.  
We use the same resistor divider bridge

* Input V = 5.0 V
* R1 = 1.5K Ohm
* Output V = 2.98 V and 1.4 mA
* R2 = 2.2K Ohm
* Ground

Note:  we need two of these (two encoders).

## Pico power control (on/off from RPi Zero)

Use a 2N2222 transistor on the ground for the Pico to allow the RPi0 to turn it 
on and off

* 2N2222 collector <- Pico GND
* 2N2222 base <- 10 KOhm resistor <- RPi0 GPIO17 (pin 11)
* 2N2222 emitter -> system GND 

Setting RPi0 GPIO17 (pin 11) LOW turns off the Pico; setting RPi0 GPIO17 HIGH turns on the Pico.
