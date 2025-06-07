# The evolution of CARRTv3 

A series of photos capturing the evolution of CARRTv3 from the very beginning of prototyping.

## Prototyping

A very early prototype combined a Raspberry Pi Zero with an Adafruit Pro Trinket (3V3) for realtime tasks. It was powered by 12V from a pack of 8 x AA cells.  This 12V feed powered both all motors and also feed a UBEC to generate 5.2V output to power the ICs.  The prototype also included an LCD with keypad. a 9DOF BNO055 sensor, and a motor-driver board.  The two photos below show the configuration without (left) and with (right) the drive motors connected.  For prototypoing, the various boards and components are velcroed onto a transparent plastic board.

[<img src="2019-06-02%20Carrt3%20Prototyping.jpg" height="320" alt="Early prototype: RPi Zero, Adafruit Trinket, 9DOF sensor, LCD + keypad, motor board" />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2019-06-02%20Carrt3%20Prototyping.jpg")
[<img src="2019-08-24%20Carrt3%20Prototyping.jpg" height="320" alt="Early prototype: RPi Zero, Adafruit Trinket, 9DOF sensor, LCD + keypad, motor board, and motors" />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2019-08-24%20Carrt3%20Prototyping.jpg)

The power source was not sufficently stable to reliably power the RPi Zero (and other ICs).  The boards themselves also took up too much space (cm<sup>2</sup>) to all fit when mounted on the vehicle chassis.  So while useful for initial testing and exploration, the configuration was updated as follows:

* An AdaFruit PowerBoost 1000C, powered by a 3.7v 2,500mAh LiPo battery replaces the 8 x AA cells and UPEC combination to power the RPi Zero (and other ICs).
    * A set of 6 x AA batteries (producing 9V) is retained on a separate power bus to power motors of various kinds.
* An RPi Pico replaces the Adafruit Pro Trinket (3V3).  The Pico provides a much more capable microcontroller, having 2 cores, more memory, and running at 10x the clock speed of the Trinket (133MHz vs 12MHz).
* A much smaller motor board replaces the original motor board.  The original was an RPi "shield" (same footprint as a fullsize RPi). The new one is a much smaller RPI "bonnet", with the much smaller footprint of an RPI Zero.
* A PCA9685 PWM board provides 16 x 12 bit PWM outputs to drive servo motors.

## Organizing Electronics

To facilitate attachment of printed circuit boards (PCBs) and the wiring between these, I created a 3D-printed plastic "riser".  The various PCBs are attached to the riser, allowing wiring to be run under or over the riser, keeping the wiring a bit neater.  The riser (with attached PCBs) is then attached to the chassis.

Not all the PCBs could fit on the riser.  As a result the motor driver PCB (an Adafruit "bonnet") is actually attached underneath the top plate of the chassis (in the area below the breadboards).  A 3D-printed holder for the LiPo battery is also attached underneath the top plate (located roughly under the PowerBoost and the RPi Zero).

Finally, a small breadboard is also installed on the lower plate of the chassis to distribute 9V power to all motors.  That small breadboard also carries an on-off switch for the 9V power circuit and an indicator LED that lights when 9V power is on.  The 9V battery pack (6 x AA) simply sits in the well between the four drive motors.

The following images show (L-to-R):
* The plastic riser with PCBs attached (but not wired).
* The plastic riser adjacent to the assembled chassis.
* The dissassembled (partially wired) chassis showing (L-to-R)
    * The bottom plate, with 4 motors and 9V breadboard.
    * The top plate (inverted), showing the motor driver PCB, and the LiPo battery holder.
    * The riser with PCBs (partially wired for testing).

[<img src="2024-05-25%20Electronics%20Deck.jpg" width="320" alt="Riser Deck, with electronics" />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2024-05-25%20Electronics%20Deck.jpg")
[<img src="2024-05-25%20Chassis%2BElectronics%20Deck.jpg" width="320" alt="Riser deck adjacent to the assembled chassis" />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2024-05-25%20Chassis%2BElectronics%20Deck.jpg")
[<img src="2024-05-26%20L-to-R%20Chassis%20bottom%2C%20underneath%20Chassis%20top%2C%20Electronics%20deck.jpg" width="320" alt="Dissassembled (partially wired) chassis" />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2024-05-26%20L-to-R%20Chassis%20bottom%2C%20underneath%20Chassis%20top%2C%20Electronics%20deck.jpg")


This layer-cake assembly (and especially the riser carrying the PCBs witht he primary electronics) simplifies assembly and disassembly, and facilitated "test as you assemble.  The following images show how it all comes together (L-to-R):
* The electronics-carrying riser mounted on top of the vehicle chassis.
* The battery-holder mounted under the chassis top plate.
* The battery-holder and motor driver PCB mounted under the chassis top plate.


[<img src="2024-05-25%20Chassis%20with%20Electronics%20Deck.jpg" width="320" alt="Electronics-carrying riser mounted on top of the vehicle chassis" />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2024-05-25%20Chassis%20with%20Electronics%20Deck.jpg")
[<img src="2024-05-25%20Battery%20under%20Chassis%20top.jpg" width="320" alt="Riser Deck, with electronics" />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2024-05-25%20Battery%20under%20Chassis%20top.jpg")
[<img src="2024-05-25%20Chassis%20%2B%20battery%20holder%20(L)%20%2B%20motor%20driver%20(R).jpg" width="320" alt="Riser Deck, with electronics" />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2024-05-25%20Chassis%20%2B%20battery%20holder%20(L)%20%2B%20motor%20driver%20(R).jpg")


## Wiring and Power-up

Wiring and power-up testing started early and continued throughout assembly.  This is depicted in the following images (L-to-R):
* A minimal assembly and wiring under power (electronics on riser only)
* All layers powered, LCD and drive motors running

[<img src="2024-05-26%20Minimal%20Assembly%20Powered.jpg" width="480" alt="Minimal assembly and wiring under power " />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2024-05-26%20Minimal%20Assembly%20Powered.jpg")
[<img src="2024-06-03%20Motors%20running%20under%20electronic%20control.jpg" width="480" alt="All layers powered, LCD and drive motors running" />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2024-06-03%20Motors%20running%20under%20electronic%20control.jpg")


Final wiring was accomplished with hook-up wire (instead of breadboarding wire).  As a general rule, longer wire reaches run under the riser; shorter ones run over the riser and PCBs.  The color code for the hook-up wires is:
| Color  | Purpose |
| :---:  | :---    |
| Black  | Ground |
| Red    | Power (5V) |
| Orange | Power (3V3) |
| Blue   | I2C SDA (data) |
| Yellow | I2C SCL (clock) |
| Grey | Serial TX-RX |
| Dark Blue | Serial RX-TX |
| Green | PWM and GPIO (various) |
| White | GPIO (various) |

Images below show a nearly fully wired CARRTv3. The only wires missing are those connecting the motor driver PCB and those connecting the Garmin Lidar (which not yet installed).  These images show views from the front and rear; from the left and right sides; and final a top view.

[<img src="2025-04-08%20CARRTv3%20Front%20View.jpg" width="480" alt="Front view" />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2025-04-08%20CARRTv3%20Front%20View.jpg")
[<img src="2025-04-08%20CARRTv3%20Rear%20View.jpg" width="480" alt="Rear view" />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2025-04-08%20CARRTv3%20Rear%20View.jpg")

[<img src="2025-04-08%20CARRTv3%20Left%20View.jpg" width="480" alt="Left view" />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2025-04-08%20CARRTv3%20Left%20View.jpg")
[<img src="2025-04-08%20CARRTv3%20Right%20View.jpg" width="480" alt="Right view" />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2025-04-08%20CARRTv3%20Right%20View.jpg")

[<img src="2025-04-08%20CARRTv3%20Top%20View.jpg" width="480" alt="Top view" />]("https://github.com/igormiktor/CARRTv3/blob/main/images/2025-04-08%20CARRTv3%20Top%20View.jpg")
