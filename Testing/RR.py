import serial
import struct
import time

# -1 is read start
# 0xAA55 is read end
#_serial = serial.Serial('/dev/tty.usbserial-12KDFD04', baudrate = 38400, timeout=1)
_serial = serial.Serial('/dev/tty.usbserial-12TBYLF', baudrate = 38400)
print "Serial set"
raw = _serial.readline()
print 0
print raw
print 1
#print "after readline"
pos1 = 451
pos2 = 520
pos3 = 427
pos4 = 500
pos5 = 500



pos_sum = pos1+pos2+pos3+pos4+pos5

data = struct.pack("<chhhhhh",'s',pos_sum,pos1,pos2,pos3,pos4,pos5)
#data = struct.pack("<c",'r')

'''
_serial.write(data)	
raw = _serial.readline()
print raw
raw = _serial.readline()
print raw
raw = _serial.readline()
print raw
'''
data = struct.pack("<chhhhhh",'s',pos_sum,pos1,pos2,pos3,pos4,pos5)
_serial.write(data)

for i in range(12):		
	raw = _serial.readline()
	print raw
	'''
	_serial.write(data)
	raw = _serial.readline()
	print raw
	_serial.write(data)
	raw = _serial.readline()
	print raw
	_serial.write(data)
	raw = _serial.readline()
	print raw
	'''
	#pos1 -= 30
	#pos_sum = pos1+pos2+pos3+pos4+pos5
