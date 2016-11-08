# robot-arm
file path: robot-arm/dynaPoseSerial/demo/demo.ino

able to control the robot arm through serial communicaiton

example:

  input: "1400b2300b4500b"

  output: move servo 1 from current position to position 400
  
          move servo 2 from current position to position 300 
          
          move servo 4 from current position to position 500
      
  input: "r1"

  output: current position of servo 1
