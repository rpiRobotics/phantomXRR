import serial
import struct
import time

# -1 is read start
# 0xAA55 is read end
_serial = serial.Serial('COM11', baudrate = 38400, timeout=1)
print "Serial set"
#raw = _serial.readline()
#print "after readline"

#data = struct.pack("<hhhhhhhh",-1,2300,500,300,500,500,500,-99)
data = struct.pack("<h",-9999)
#print "after struct"
#_serial.write(data)
#print "after write"
#time.sleep(2)

for i in range(20):
        _serial.write(data)
        raw = _serial.readline()
        print "Getting new message"
        print raw
