# CARRTv3
Cute Autonomous Robot Rover Thing (version 3 -- Raspberry Pi Zero 2 W and Raspberry Pico based)

## Goal 

Goal of this project is to create a small robotic vehicle (CARRT) that can do the following:

- Start in one room of the house;
- Receive a goal position, specified relative to its starting location (e.g., 8 meters North, 4.5 meters West);
- Autonomously make its way to the new position, mapping doors and walls with its Lidar sensor until it finds a path to the goal;
- Build a map as it goes, enabling effective back-tracking whenever it encounters a dead-end.

This is both a hardware and a software project.

The name "CARRT" is thanks to my daughter.

## Version 3: What's New?

[Version 2](https://github.com/igormiktor/CARRTv2) was based on an AVR Mega2560 microcontroller, which proved to have too little memory maintain a suitably accurate and large enough map of its environment and also calculate paths.  Version 2 also suffered from inability to track its own position and orientation with sufficient accuracy.

To overcome these problems, Version 3 uses a Raspberry Pi Zero 2 W as its primary processor, paired with a Raspberry Pico to handle real-time tasks.  The Pi Zero has more than sufficient RAM to maintain a suitable resolution map of its environment.  Version 3 also has encouders on its wheels and uses a BNO055 9-DOF sensor with on-board sensor fusion using an integrated 32-bit Cortex M0+ microcontroller, so that Euler angles and quaternions can be directly read from the BNO055.  The combination of wheel-encoders and direct access to orientation information are expected to provide suffiently accurate orientation and position information. 

## The Hardware and Software

The hardware consists of a four-wheeled chassis carrying a Raspberry Pi Zero 2 W as its primary process, paired with a Raspberry Pico to handle real-time tasks.  These are complemented by various other supporting IC subsystems (e.g., a motor driver board, a board with an LCD and keypad, a PWM/Servo-driver board, 3V3<->5V level converters, a battery regulator, etc.)  Navigation is enabled by an BNO055 9-DOF orientation sensor and wheel-encoders.  A Garmin Lidar Lite sensor mounted on a servo allows CARRT to sense obstacles (both furniture and walls) and navigate around them.

The hardware and how it is wired together is described in the `schematics` sub-directory by a [KiCad](https://www.kicad.org/) project file, as well as a PDF of the schematics. Photos of CARRT appear in the `images` sub-directory.

The software is found in the `source` directory, with subdirectories for the RPi Zero source code (`source/rpi`) and the Pico source code (`source/pico`).  Code common to both is in `source/shared`.   The RPi Zero uses I2C to control and monitor the motor driver, the LCD+Keypad, the servo, the Lidar.  The Pico uses a separate I2C to control and monitor the BNO055 and the wheel-encoders; it also uses an analog input to monitor the level of the LiPo battery that powers the IC circuits.

## Power

CARRTv3 is powered from two sources. A LiPo battery connected to PowerBoost 1000 module provides 5V power that drives all of the IC-related electronics.  The RPi Zero provides a 3V3 power feed for those subcomponents that need 3V3 power.  The four wheel motors and the servo are driven by a 9V circuit feed by a six-pack of AA batteries

## Specs

* Main processor
    * PI Zero 2 W board
    * Broadcom BCM2710A1, quad-core 64-bit SoC (Arm Cortex-A53 @ 1GHz)
    * 512MB LPDDR2
    * 2.4GHz IEEE 802.11b/g/n wireless LAN, Bluetooth 4.2, BLE, onboard antenna
* Secondary microcontroller
    * Pico RP2040 microcontroller
    * Dual-core Arm Cortex M0+ processor, running up to 133 MHz
    * 264KB of SRAM
    * 2MB of on-board flash memory
    * 2 × SPI, 2 × I2C, 2 × UART, 3 × 12-bit ADC, 16 × controllable PWM channels
    * Accurate clock and timer on-chip
    * Temperature sensor
* Power buses
    * Two power buses
    * First power bus supplied by LiPo battery
        * PowerBoost 1000 generates 5V from the LiPo
        * Pi Zero in turn generates 3V3 power for all devices that need it
        * Both 5V and 3V3 available as required (CARRTv3 is primarily a 3V3 system)
    * Second power bus supplies unregulated power to motors and servos
        * 9V power supplied by a six-pack of AA batteries
* Motor controllers
    * Adafruit Raspberry Pi DC & Stepper Motor Bonnet 
    * Uses 4 H-Bridges to power 4 DC motors
    * Each H-Bridge is a TB6612 chipset providing 1.2A per bridge
    * Controlled by the Pi Zero via I2C
* 2 x 16 character color LCD display with keypad
    * LCD controlled by an HD44780U
    * MCP23017 drives the HD44780U and the keypad
    * Controlled by Pi Zero via I2C
* PowerBoost 1000
    * Converts LiPo battery to 5V
    * Also LiPo batter charger
    * Provides "power off" control via grounding of an "enable" pin 
    * Battery level pin read by analog Pico pin (level shifted by resistor bridge to 3V3)
* BNO055 9-DOF Orientation Sensor
    * 3 sensors in one device
        * Triaxial 16bit gyroscope
        * Triaxial 14bit accelerometer
        * Geomagnetic sensor
    * 32-bit Cortex M0+ microcontroller runs sensor fusion software,
    * Outputs Euler angles, quaternions, rotation vector, linear acceleration, gravity, heading 
    * Controlled by Pico via I2C
* PCA9685 16-channel PWM/Servo driver
    * Used to drive the servo
    * Controlled by Pi Zero via I2C
    * Servos powered by 9V bus
* TXB0104 Bi-Directional Level Shifter
    * Performs bidirectional level shifting between 5V and 3V3 
    * Auto-detects the direction
    * Not suitable for I2C level-shifting
* BSS138 Bi-Directional Logic Level Shifter
    * 4-channel I2C-safe bi-directional logic level converter 
    * Open-drain-with-10K-pullups outputs
    * 4 BSS138 FETs with 10K pullups
    * Logic-shift only, no loads on high side
* Wheel-encoders 
    * On two front wheels
    * This is a 5V subsystem
    * Read by Pico after level-shifting by TXB0104
* HPS-2018MG servo
    * Controlled by Pi Zero via I2C
* Garmin Lidar Lite v3
    * Mounted on the HPS-2018MG servo
    * This is a 5V subsystem
    * Controlled by PI Zero via I2C (level-shifted by BSS138)
* PS1240 piezoelectric buzzer
    * For success beeps and error tones

