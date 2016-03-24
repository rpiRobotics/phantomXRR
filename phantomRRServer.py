import serial
import struct
import time
import RobotRaconteur as RR
import thread
import threading
import numpy
import traceback
import socket
import time
import sys
import signal
import argparse

RRN = RR.RobotRaconteurNode.s

# catch control c to allow for use of the same port
def signal_handler(signal, frame):
        print('Ctrl+C shutdown!')
        RRN.Shutdown()
        sys.exit(0)


class PhantomXInterface(object):
    def __init__(self,  serialPort):
        self._lock=threading.RLock()
        self._serial=serial.Serial(serialPort,baudrate=38400)

    def getServoPosition(self, servoNumber):
        with self._lock:
            msg=struct.pack("cc",'r',str(servoNumber))
            self._serial.write(msg)
            raw = self._serial.read(35)
            print raw
            
    # posArray is a list of size 5 to set joints
    def setServoPositions(self, posArray):
        with self._lock:
            commandString ="1"+str(posArray[0])+"b2"+str(posArray[1])+"b4"+str(posArray[2])+"b6"+str(posArray[3])+"b8"+str(posArray[4])
            msg=stuct.pack("s", commandString)
            self._serial.write(msg)
            

def main():
    myPhantom = PhantomXInterface('/dev/ttyUSB0')
    myPhantom.getServoPosition(2)
    


if __name__ == '__main__':
    main()
