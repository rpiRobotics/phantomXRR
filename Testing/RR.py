import serial
import struct
import time

# -1 is read start
# 0xAA55 is read end
_serial = serial.Serial('COM11', baudrate = 38400, timeout=1)
print "Serial set"
#raw = _serial.readline()
#print "after readline"

data = struct.pack("<chhhhhh",'s',2300,500,300,500,500,500)
#data = struct.pack("<c",'r')

for i in range(20):
        _serial.write(data)
        raw = _serial.readline()
        print raw