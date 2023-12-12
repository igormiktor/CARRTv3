# Raspberry Pi Zero 2 W Configuration

## Goal

Goal is to minize the amperage that the RPi Zero 2 W draws.  Power comes from a LiPo battery through an Adafruit
Powerboost 1000C that has a 2A internal switch (~2.5A peak limiting). The RPi Zero then powers the RP2040 (5V feed) and 
all the secondary chips (3.3V feed).  Motors are powered by a separate power circuit.

## Power Demands

The main power consumers are:

| Device      | Recommended Power <br /> Supply Capacity   | Typical Bare-Board <br /> Current Draw | Max Board <br /> Current Draw | 
|:-----------:|:-----------------:|:------------------:|:------------:|
| Raspberry Pi Zero W   | 1.2 A | 150 mA | ~0.8 A |
| Raspberry Pi Zero 2 W | 2.0 A | 350 mA | ~1.3 A |
| Raspberry Pico        | 0.5 A | 35 mA  | ~100 mA | 

## Minimizing Power Demand

To minimize the RPi Zero power consumption, we make the following configuration changes.

### Turn off blue tooth
To turn off blue tooth (which also makes the best UART available for serial communications) **_add_** the following 
line to `/boot/config.txt` under section `[all]`:

```bash
dtoverlay=disable-bt
```

### Turn off HMDI.
To turn off HMDI first **_comment out_** the following line in `/boot/config.txt`:

```bash
dtoverlay=vc4-kms-v3d
```
and then **_add_** the following line to `/etc/rc.local`:

```bash
/usr/bin/tvservice -o
```

## Provide a Linux shutdown button

To provide a button to trigger a controlled Linux shutdown, I configured GPIO 16 to trigger a controlled
shutdown by adding the following line to `/boot/config.txt` under section `[all]`: 

```bash
dtoverlay=gpio-shutdown,gpio_pin=16,active_low=1,gpio_pull=up
```

And then make sure that in file `/etc/systemd/logind.conf` the line for HandlePowerKey appears as follows:

```bash
HandlePowerKey=poweroff
```

If it is not, the best solution is to create the directory `/etc/systemd/logind.conf.d` and then in that directory
create a file (such as `handlePowerKey.conf` or `poweroff.conf`) containing:

```bash
[Login]
HandlePowerKey=poweroff
```

Note that the default GPIO for poweroff (GPIO 3) is needed for I2C, so it can't be used for the shutdown function.
