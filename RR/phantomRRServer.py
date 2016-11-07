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
        self._serial=serial.Serial(serialPort,baudrate=38400,timeout=.2)
        # save joint readings so if a reading fails
        self.joint_value = [500,500,500,500,500] 

    def getJointPositions(self):
        # map from joint number to servo number used for finding
        # joint angle by querying the correct servo
        try:
            with self._lock:
                msg=struct.pack("<h",-9999)
                self._serial.write(msg)
                raw = [0,0,0,0,0]
                
                for i in range(5):
                    raw[i] = self._serial.readline()
                    # chop off the joint index
                    raw[i] = raw[i][1:-2]
                    # if for some reason we got a bad value, return the last good one
                    if raw[i] == '':
                        raw[i] = self.joint_value[i]
                    if int(raw[i]) < 0:
                        raw[i] = self.joint_value[i]
                    self.joint_value[i] = int(raw[i])
                return self.joint_value
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
                #commandString ="1"+str(posArray[0])+"b2"+str(posArray[1])+"b4"+str(posArray[2])+"b6"+str(posArray[3])+"b8"+str(posArray[4])+"b"
                pos_sum = 0
                for i in range(5):
                    pos_sum += posArray[i]
                    print posArray[i]
                print pos_sum
                data = struct.pack("<hhhhhhhh",-1,pos_sum,posArray[0],posArray[1],posArray[2],posArray[3],posArray[4],-99)
                self._serial.write(data)
        except:
            print "Could not set joint positions"
    
    '''
    # posArray is a list of size 5 to set joints
    def setJointPosition(self, servoNum, pos):
        try:
            with self._lock:
                jointToServo = {1:'1', 2:'2', 3:'4', 4:'6', 5:'8'}
                commandString = jointToServo[int(servoNum)]+str(pos)+"b"
                self._serial.write(commandString)
        except:
            print "Could not set joint position"
            
    '''
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


if __name__ == '__main__':
    main()
