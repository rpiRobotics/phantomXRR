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

    def getJointPosition(self, servoNumber):
        # map from joint number to servo number used for finding
        # joint angle by querying the correct servo
        jointToServo = {1:'1', 2:'2', 3:'4', 4:'6', 5:'8'}
        with self._lock:
            msg=struct.pack("cc",'r',jointToServo[int(servoNumber)])
            self._serial.write(msg)
            raw = self._serial.readline()
            # chop off the joint index
            raw = raw[1:-2]
            return int(raw)
            
    # posArray is a list of size 5 to set joints
    def setJointPositions(self, posArray):
        if -1 in posArray:
            print "Tried to command a -1"
            return
            
        with self._lock:
            commandString ="1"+str(posArray[0])+"b2"+str(posArray[1])+"b4"+str(posArray[2])+"b6"+str(posArray[3])+"b8"+str(posArray[4])+"b"
            self._serial.write(commandString)

    # posArray is a list of size 5 to set joints
    def setJointPosition(self, servoNum, pos):
        with self._lock:
            commandString =str(servoNum)+str(pos)+"b"
            self._serial.write(commandString)
            

def main():    

    port = 10001       
    t1 = RR.LocalTransport()
    t1.StartServerAsNodeName("phantomXRR")
    RRN.RegisterTransport(t1)

    t2 = RR.TcpTransport()
    t2.EnableNodeAnnounce()
    t2.StartServer(port)
    RRN.RegisterTransport(t2)
    
    
    myPhantom = PhantomXInterface('/dev/ttyUSB0')

    with open('phantomXRR.robodef', 'r') as f:
        service_def = f.read()
    
    RRN.RegisterServiceType(service_def)
    RRN.RegisterService("phantomXController", "phantomXRR.PhantomXInterface", myPhantom)
    print "Conect string: tcp://localhost:" + str(port) + "/phantomXRR/phantomXController"
    raw_input("Press any key to end")

    RRN.Shutdown()
    
    # test code
    # jointAngles = []
    # jointAngles.append(myPhantom.getJointPosition(1))
    # jointAngles.append(myPhantom.getJointPosition(2))
    # jointAngles.append(myPhantom.getJointPosition(3))
    # jointAngles.append(myPhantom.getJointPosition(4))
    # jointAngles.append(myPhantom.getJointPosition(5))
    # myPhantom.setJointPositions(jointAngles)
    


if __name__ == '__main__':
    main()
