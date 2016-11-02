# This is a Robot Raconteur Server for the Phantom Reactor Arm from Trossen
# It has methods to read and set joint angles.  Run this script in a terminal
# and then run the associated Matlab client.  A simple client would look like the following
# in Matlab:
# 
#robot = RobotRaconteur.Connect('tcp://localhost:10001/phantomXRR/phantomXController');
#robot.getJointPosition(int16(0))
#robot.setJointPosition(int16(0),int16(100))
#robot.getJointPosition(int16(0))

# ATTENTION: You will need to change the port for serial connection from COM6 to whatever
# port your robot arm is actually on.  Use device manager to find the port
# change it in the line: myPhantom = PhantomXInterface('COM6')
# Make sure the .robodef file is in the same folder as this script!

import serial
import struct
import time
import RobotRaconteur as RR
import thread
import threading
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
        self._serial=serial.Serial(serialPort,baudrate=9600,timeout=.2)
        # save joint readings so if a reading fails
        self.joint_value = [500,500,500,500,500] 

    def getJointPosition(self, servoNum):
        # map from joint number to servo number used for finding
        # joint angle by querying the correct servo
        try:
            jointToServo = {1:'1', 2:'2', 3:'4', 4:'6', 5:'8'}
            with self._lock:
                msg=struct.pack("cc",'r',jointToServo[int(servoNum)])
                self._serial.write(msg)
                raw = self._serial.readline()
                # chop off the joint index
                raw = raw[1:-2]
    
                # if for some reason we got a bad value, return the last good one
                if raw == '':
                    return self.joint_value[servoNum-1]
    
                if int(raw) < 0:
                    return self.joint_value[servoNum-1]
                        
    
                jointReading = int(raw)
                self.joint_value[servoNum-1] = jointReading
                return jointReading
        except:
            print "Could not read joint positions"
            return self.joint_value
            
    # posArray is a list of size 5 to set joints
    def setJointPositions(self, posArray):
        try:
            if -1 in posArray:
                print "Tried to command a -1"
                return
                
            with self._lock:
                commandString ="1"+str(posArray[0])+"b2"+str(posArray[1])+"b4"+str(posArray[2])+"b6"+str(posArray[3])+"b8"+str(posArray[4])+"b"
                self._serial.write(commandString)
        except:
            print "Could not set joint positions"

    # posArray is a list of size 5 to set joints
    def setJointPosition(self, servoNum, pos):
        try:
            with self._lock:
                jointToServo = {1:'1', 2:'2', 3:'4', 4:'6', 5:'8'}
                commandString = jointToServo[int(servoNum)]+str(pos)+"b"
                self._serial.write(commandString)
        except:
            print "Could not set joint position"
            

def main():    

    port = 10001       
    t1 = RR.LocalTransport()
    t1.StartServerAsNodeName("phantomXRR")
    RRN.RegisterTransport(t1)

    t2 = RR.TcpTransport()
    t2.EnableNodeAnnounce()
    t2.StartServer(port)
    RRN.RegisterTransport(t2)
    
    
    myPhantom = PhantomXInterface('COM11')

    with open('phantomXRR.robodef', 'r') as f:
        service_def = f.read()
    
    RRN.RegisterServiceType(service_def)
    RRN.RegisterService("phantomXController", "phantomXRR.PhantomXInterface", myPhantom)
    print "Conect string: tcp://localhost:" + str(port) + "/phantomXRR/phantomXController"
    raw_input("Press any key to end")

    RRN.Shutdown()


if __name__ == '__main__':
    main()
