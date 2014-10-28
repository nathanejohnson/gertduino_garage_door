#!/usr/bin/env python
import serial

ser = serial.Serial('/dev/ttyAMA0', 9600, timeout=1)
while True:
    inStr = raw_input("Enter a command, 1 to read hall sensor, 2 to toggle garage door")
    try:
        command = int(inStr)
    except ValueError:
        command = -1
    ser.write(str(command))
    print ser.readline()[:-2]
