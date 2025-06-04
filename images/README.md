# The evolution of CARRTv3 

A series of photos capturing the evolution of CARRTv3 from the very beginning of prototyping.

## Prototyping ##

A very early prototype combined a Raspberry Pi Zero with an Adafruit Pro Trinket (3V3) for realtime tasks. It was powered by 12V from a pack of 8 x AA cells.  This 12V feed powered both all motors and also feed a UBEC to generate 5.2V output to power the ICs.  The prototype also included an LCD with keypad. a 9DOF BNO055 sensor, and a motor-driver board.  The two photos below show the configuration without (left) and with (right) the drive motors connected.

[<img src="2019-06-02%20Carrt3%20Prototyping.jpg" height="320" alt="Early prototype: RPi Zero, Adafruit Trinket, 9DOF sensor, LCD + keypad, motor board" />]("2019-06-02%20Carrt3%20Prototyping.jpg")  [<img src="2019-08-24%20Carrt3%20Prototyping.jpg" height="320" alt="Early prototype: RPi Zero, Adafruit Trinket, 9DOF sensor, LCD + keypad, motor board, and motors" />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2019-08-24%20Carrt3%20Prototyping.jpg)

The power source was not sufficently stable to reliably power the RPi Zero (and other ICs).  The boards themselves also took up too much space (cm^2^) to mount on the vehicle chassis.  So while useful for initial testing and exploration, the configuration was updated as follows:

* An AdaFruit PowerBoost 1000C, powered by a 3.7v 2,500mAh LiPo battery replaces the 8 x AA cells and UPEC combination to power the RPi Zero (and other ICs).
    * A set of 6 x AA batteries (producing 9V) is retained on a separate power bus to power motors of various kinds.
* An RPi Pico replaces the Adafruit Pro Trinket (3V3).  The Pico provides a much more capable microcontroller, having 2 cores, more memory, and running at 10x the clock speed of the Trinket (133MHz vs 12MHz).
* A much smaller motor board replaces the original motor board.  The original was an RPi "shield" (same footprint as a fullsize RPi). The new one is a much smaller RPI "bonnet", with the much smaller footprint of an RPI Zero.
* A PCA9685 PWM board provides 16 x 12 bit PWM outputs to drive servo motors