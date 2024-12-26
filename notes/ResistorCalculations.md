# Resistor Calculations

## PowerBoost 1000C LBO

LBO drops to 0V when PowerBoost detects battery low voltage (<3.2V).  
However, LBO is pulled up to Vin (up to 5.25V), 
so we need to create a resistor divider to ensure the output 
is always under 3.3V.

* Input V = 5.25 V   <-- PowerBoost LBO Pin
* R1 = 1.5K Ohm
* Output V = 3.12 V and 1.4 mA  --> RPi Zero Pin xxx
* R2 = 2.2K Ohm
* Ground

Alternate:

* Input V = 5.25 V              <-- Encoder wire 1 (Green)
* R1 = 3.3K Ohm
* Output V = 3.19 V and 0.6 mA  --> Pico Pin xxx or yyy
* R2 = 5.1K Ohm
* Ground   

## Encoders

Encoders on the wheels are 5V devices, need to convert them to 3.3V.  
We use the same resistor divider bridge

* Input V = 5.0 V              <-- Encoder wire 1 (Green)
* R1 = 1.5K Ohm
* Output V = 2.98 V and 1.4 mA  --> Pico Pin xxx or yyy
* R2 = 2.2K Ohm
* Ground                       

Alternate:

* Input V = 5.0 V              <-- Encoder wire 1 (Green)
* R1 = 3.3K Ohm
* Output V = 3.04 V and 0.6 mA  --> Pico Pin xxx or yyy
* R2 = 5.1K Ohm
* Ground   


Note:  we need two of these (two encoders).
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