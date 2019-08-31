#!/usr/bin/env python2

# Test serial comms with Trinket

import serial

try:
    import RPi.GPIO as gpio

    port = "/dev/serial0"

    trinket = serial.Serial( port, 115200, timeout = 10 )
    print "Opened port", port, "for testing:"

    gpio.setmode( gpio.BCM )


    # Turn Trinket on
    gpio.setup( 27, gpio.OUT )
    gpio.output( 27, gpio.HIGH )
    banner = str( trinket.readline() )
    print "Rcvd:", banner
    if banner == "Trinket started\n":
        testStrings = [ "Hello World!", "Test 1", "Test 2\n", "Test\n 3" ]
        for testStr in testStrings:
            bytesSent = trinket.write( testStr )
            print "Sent:", testStr, "bytes:", bytesSent
            rcvdStr = str( trinket.readline() )
            print "Rcvd:", rcvdStr, "bytes:", len( rcvdStr )


except RuntimeError:
    print( "Error importing RPi.GPIO!" )

except IOError:
    print "Failed at", port, "\n"

finally:
    gpio.output( 27, gpio.LOW )
    gpio.cleanup()
    trinket.close()
