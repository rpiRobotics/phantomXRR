import serial
import struct
import time
import thread
import threading
import traceback
import socket
import time
import sys
import signal
import argparse
import random
import copy

class PhantomXInterface(object):
    def __init__(self,  serialPort):
        self._lock=threading.RLock()
        self._serial=serial.Serial(serialPort,baudrate=38400,timeout=.1)
        # save joint readings so if a reading fails
        self.joint_value = [500,500,500,500,500] 

    def getJointPositions(self):
        # map from joint number to servo number used for finding
        # joint angle by querying the correct servo
        #print "before send"
        msg=struct.pack("<c",'r')
        self._serial.write(msg)
        return 1
         
    # posArray is a list of size 5 to set joints
    def setJointPositions(self, posArray):
        try:
            if -1 in posArray:
                print "Tried to command a -1"
                return
                
            if 1==1:
                
                pos_sum = 0
                for i in range(5):
                    pos_sum += posArray[i]

                data = struct.pack("<chhhhhh",'s',pos_sum,posArray[0],posArray[1],posArray[2],posArray[3],posArray[4])
                self._serial.write(data)

        except:
            print "Could not set joint positions"

def add_random(base, lower_b, upper_b):
    new_angles = copy.deepcopy(base)
    for i in range(len(new_angles)):
        new_angles[i]+=random.randint(lower_b,upper_b)
    return new_angles
            
def main():
    myPhantom = PhantomXInterface('/dev/tty.usbserial-12KDFD04')
    print "after set up"
    base_pose = [500,500,500,500,500]
    #raw = myPhantom._serial.readline()
    #print raw
    print "before for loop"
    for i in range(1000):
        myPhantom.getJointPositions()
        '''
        #print "reading positions---------------------"
        #serial.available()
        raw = myPhantom._serial.readline()
        print raw
        #state
        raw = myPhantom._serial.readline()
        print raw
        #read state
        raw = myPhantom._serial.readline()
        print raw
        #five joint positions
        #1
        raw = myPhantom._serial.readline()
        print raw
        #2
        raw = myPhantom._serial.readline()
        print raw
        #3
        raw = myPhantom._serial.readline()
        print raw
        #4
        raw = myPhantom._serial.readline()
        print raw
        #5
        raw = myPhantom._serial.readline()
        print raw
        #print "before set------------------------"
        '''
        new_pose = add_random(base_pose,-10,10)
        #print new_pose
        myPhantom.setJointPositions(new_pose)
        '''
        #serial.available()
        raw = myPhantom._serial.readline()
        print raw
        #state
        raw = myPhantom._serial.readline()
        print raw
        #set state
        raw = myPhantom._serial.readline()
        print raw
        #before go to
        raw = myPhantom._serial.readline()
        print raw        
        '''
        time.sleep(0.1)


if __name__ == '__main__':
    main()
