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

[Version 2](https://github.com/igormiktor/CARRTv2) was based on an AVR Mega2560 microcontroller, which proved to have too little memory to maintain a suitably accurate and large enough map of its environment (and it was quite slow at calculating paths).  Version 2 was also unable to track its own position and orientation with sufficient accuracy, making usable map updates effectively impossible.

To overcome these problems, Version 3 uses a Raspberry Pi Zero 2 W as its primary processor, paired with a Raspberry Pico to handle real-time tasks.  The Pi Zero has sufficient RAM to maintain a map of its environment with suitable resolution.  Version 3 also has encoders on its wheels and it uses a BNO055 9-DOF sensor with on-board sensor fusion, so that Euler angles and quaternions can be directly read from it.  The combination of wheel-encoders and direct access to orientation information are estimated to provide suffiently accurate orientation and position information. 

## The Hardware and Software

The hardware consists of a four-wheeled chassis carrying a Raspberry Pi Zero 2 W as its primary processor, paired with a Raspberry Pico to handle real-time tasks.  These are complemented by various other supporting IC subsystems (e.g., a motor driver board, a board with an LCD and keypad, a PWM/Servo-driver board, 3V3<->5V level converters, a battery regulator, etc.)  Navigation is enabled by an BNO055 9-DOF orientation sensor and wheel-encoders.  A Garmin Lidar Lite sensor mounted on a servo allows CARRT to sense obstacles (both furniture and walls) and navigate around them.

The hardware and how it is wired together is described in the [`schematics`](https://github.com/igormiktor/CARRTv3/tree/main/schematics) sub-directory by a [KiCad](https://www.kicad.org/) project file, as well as a PDF of the schematics. Photos of CARRT appear in the [`images`](https://github.com/igormiktor/CARRTv3/tree/main/images) sub-directory.

The software is found in the [`source`](https://github.com/igormiktor/CARRTv3/tree/main/source) directory, with subdirectories for the Pi Zero source code ([`source/rpi`](https://github.com/igormiktor/CARRTv3/tree/main/source/rpi)) and the Pico source code ([`source/pico`](https://github.com/igormiktor/CARRTv3/tree/main/source/pico)).  Code common to both is in [`source/shared`](https://github.com/igormiktor/CARRTv3/tree/main/source/shared).   

The Pi Zero uses I2C to control and monitor the motor driver, the LCD & Keypad, the servo, and the Lidar.  The Pico uses real-time interrupts to track the wheel-encoders and uses a separate I2C to control and monitor the BNO055; the Pico also uses an analog input pin to monitor the level of the LiPo battery that powers the IC circuits.  The Pi Zero and the Pico communicate over a serial link connecting their UARTs.

## Power

CARRTv3 is powered from two sources. A LiPo battery connected to a PowerBoost 1000 module provides 5V power to drive all of the IC-related electronics.  The Pi Zero provides a 3V3 power feed for those subcomponents that need 3V3 power.  A six-pack of AA batteries provides 9V power to the four motors that drive the wheels and to the servo that points the Lidar sensor.

## Specs

* Main processor
    * PI Zero 2 W board
    * Broadcom BCM2710A1, quad-core 64-bit SoC (Arm Cortex-A53 @ 1GHz)
    * 512MB LPDDR2
    * 2.4GHz IEEE 802.11b/g/n wireless LAN, Bluetooth 4.2, BLE, onboard antenna
    * 2 x SPI, 1 x I2C, 1 x UART, no ADC
* Secondary microcontroller
    * Pico RP2040 microcontroller
    * Dual-core Arm Cortex M0+ processor, running up to 133 MHz
    * 264KB of SRAM
    * 2MB of on-board flash memory
    * 2 × SPI, 2 × I2C, 2 × UART, 3 × 12-bit ADC
    * Accurate clock and timer on-chip
    * Temperature sensor
* Power buses
    * First power bus supplied by LiPo battery
        * PowerBoost 1000 generates 5V from the LiPo
        * Pi Zero in turn generates 3V3 power for all devices that need it
        * Both 5V and 3V3 available as required (CARRTv3 is primarily a 3V3 system)
    * Second power bus supplied by a six-pack of AA batteries
        * Provides 9V power to the motors 
        * 4 driving motors (one per wheel)
        * 1 servo motor (points the Lidar across a 180 degree arc) 
* Motor controllers
    * Adafruit Raspberry Pi DC & Stepper Motor Bonnet
    * Uses 4 H-Bridges to power 4 DC motors
    * Each H-Bridge is a TB6612 providing 1.2A per bridge
    * Controlled by the Pi Zero via I2C
* LCD display with keypad
    * 2 x 16 character color LCD
    * 5 button keypad
    * LCD controlled by an HD44780U
    * MCP23017 drives the HD44780U and the keypad
    * MCP23017 connected via I2C to Pi Zero
    * Controlled by Pi Zero via I2C
* PowerBoost 1000
    * Converts LiPo battery to 5V
    * Also serves LiPo batter charger (via USB power connection)
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
    * 16 independent PWM channels
    * Used to control the servo
    * Controlled by Pi Zero via I2C
    * Servo powered by 9V bus
* TXB0104 Bi-Directional Level Shifter
    * Performs bidirectional level shifting between 5V and 3V3 
    * Auto-detects the direction
    * Not suitable for I2C level-shifting
* BSS138 Bi-Directional Logic Level Shifter
    * 4-channel I2C-safe bidirectional logic level converter 
    * 4 BSS138 FETs with 10K pullups
    * Logic-shift only, no loads possible on high side
* Wheel-encoders 
    * On two front wheels
    * Encoders are 5V subsystems
    * Read by Pico after level-shifting by TXB0104
* Hiwonder HPS-2018MG servo
    * Working voltage 6.8 - 10.4V
    * Rotation range 0 - 180 degress 
    * Servo accuracy 0.3 degrees
    * Pulse period 20ms
    * Duty cycle 0.5ms - 2.5ms
    * Controlled by Pi Zero via I2C via PCA9685
* Garmin Lidar Lite v3
    * Mounted on the HPS-2018MG servo
    * Resolution 1cm
    * Accuracy +/- 2.5cm
    * Range 5cm to 40m
    * Laser wavelength 905nm
    * Lidar is a 5V subsystem
    * Controlled by PI Zero via I2C (level-shifted by BSS138)
* PS1240 piezoelectric buzzer
    * For success beeps and error tones

