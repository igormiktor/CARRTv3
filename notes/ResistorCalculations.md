# Resistor Calculations

## PowerBoost 1000C LBO

LBO drops to 0V when PowerBoost detects battery low voltage (<3.2V).  
However, LBO is pulled up to Vin (up to 5.25V), 
so we need to create a resistor divider to ensure the output 
is always under 3.3V.

* Input V = 5.25 V
* R1 = 330 Ohm
* Output V = 3.30 V
* R2 = 560 Ohm
* Ground

Note that when the input voltage drops to 3.2V, the output voltage is 2.0V.
This is sufficient voltage to drive RPi to read the gpio pin as a digital "1".

## Encoders

Encoders on the wheels are 5V devices, need to convert them to 3.3V.  
We use the same resistor divider bridge

* Input V = 5.0 V
* R1 = 330 Ohm
* Output V = 3.15 V
* R2 = 560 Ohm
* Ground