#include <ax12.h>
#include <BioloidController.h>
//#include <math.h>

BioloidController bioloid = BioloidController(1000000);
const int SERVOCOUNT = 8;
uint16_t desired_loc[]= {512, 512, 512, 512, 512, 512, 512, 512};

PROGMEM prog_uint16_t Center[] = {8, 512, 512, 512, 512, 512, 512, 512, 512};

//===========================================================================
int servo_num = 0;
int cur_pos = 0;
boolean bad_pos = false;

String instring; 
boolean run = true;
//===========================================================================
/*
void go_to(){
    Serial.println("enter go to");
    PROGMEM prog_uint16_t Home[] = {8, desired_loc[0], desired_loc[1], desired_loc[2], desired_loc[3], desired_loc[4], desired_loc[5], desired_loc[6], desired_loc[7]};
    bioloid.loadPose(Home); 
    bioloid.readPose(); 
    bioloid.interpolateSetup(1000); // setup for interpolation from current->next over 1/2 a second
    while(bioloid.interpolating > 0){  // do this while we have not reached our new pose
        bioloid.interpolateStep();     // move servos, if necessary. 
        delay(3);
    }
    Serial.println("reach the end of go to");
}
*/

void go_to(){
  //Serial.println("enter go to");
  for (int i = 0; i < 8;i++) {
    if (i != 6) {
      cur_pos = ax12GetRegister(i+1,36,2);
      if ( (cur_pos < desired_loc[i]-5) || (cur_pos > desired_loc[i]+5) ) {
        if (i == 1) {
          SetPosition(i+1,desired_loc[i]);
          SetPosition(i+2,1024-desired_loc[i]);
        }
        else if(i == 2) {
          SetPosition(i+1,desired_loc[i]);
          SetPosition(i,1024-desired_loc[i]);
        }
        else if(i == 3) {
          SetPosition(i+1,desired_loc[i]);
          SetPosition(i+2,1024-desired_loc[i]);
        }
        else if(i == 4) {
          SetPosition(i+1,desired_loc[i]);
          SetPosition(i,1024-desired_loc[i]);
        }
        else {
          SetPosition(i+1,desired_loc[i]);
        }
      }
    }  
  }
  //Serial.println("reach the end of go to");
}

void MoveCenter(){
    delay(100);                    // recommended pause
    bioloid.loadPose(Center);   // load the pose from FLASH, into the nextPose buffer
    bioloid.readPose();            // read in current servo positions to the curPose buffer

    bioloid.interpolateSetup(1000); // setup for interpolation from current->next over 1/2 a second
    while(bioloid.interpolating > 0){  // do this while we have not reached our new pose
        bioloid.interpolateStep();     // move servos, if necessary. 
        delay(3);
    }
}

//============================================================================
//============================================================================

void setup()
{
  pinMode(0,OUTPUT);
  Serial.begin(9600); //start serial communications at 38400bps
  Serial.setTimeout(2);
  //Serial.println("Before MoveCenter()");
  //MoveCenter();
  //Serial.println("After MoveCenter()");
  //randomSeed(analogRead(0));
  for (int i = 0; i < 8;i++) {
    desired_loc[i] = ax12GetRegister(i,36,2);
  }
}
//============================================================================
// user input sample
// 1500b2300b3400b
// move all the servos at the same time
// r1 r2
// return position

void loop() 
{
  
  if (Serial.available() > 0) 
  {
    //Serial.println("Serial.available()");
    instring = Serial.readString();
    
    //Serial.println("before go to");
    //go_to();
    
    //Serial.println("Before for loop");
    for (int i = 0; i < instring.length();i++)
    {
      if (instring[i] == 'r')
      {
        servo_num = int(instring[i+1])-48;
        Serial.print(instring[i+1]);
        Serial.println(GetPosition(servo_num));
        //Serial.println(ax12GetRegister(servo_num,36,2));
        i++;
      }
      else if ((instring[i] == '1' )
                || (instring[i] == '2' )
                || (instring[i] == '3' )
                || (instring[i] == '4' )
                || (instring[i] == '5' )
                || (instring[i] == '6' )
                || (instring[i] == '8' )
                )
      { 
        servo_num = int(instring[i])-48;
        String desire_pos = "";
        i++;
        
        while (instring[i] != 'b')
        {
          desire_pos += instring[i];
          i++;
        }
        
        int temp = desire_pos.toInt();
        //Serial.print("desire_pos: ");
        //Serial.println(temp);
        
        //cur_pos = ax12GetRegister(servo_num,36,2);
        
        if (servo_num == 2 )
        {
          desired_loc[2-1]= temp;
          desired_loc[3-1]= 1024-temp;
        }
        else if (servo_num == 3 )
        { 
          desired_loc[3-1]= temp;
          desired_loc[2-1]= 1024-temp;
        }
        else if (servo_num == 4 )
        {
          desired_loc[4-1]= temp;
          desired_loc[5-1]= 1024-temp;
        }
        else if (servo_num == 5 )
        {  
          desired_loc[5-1]= temp;
          desired_loc[4-1]= 1024-temp;
        }
        else
        {
          desired_loc[servo_num-1] = temp;
        }
      }
    }
    //Serial.println("before go to");
    go_to();
    //Serial.println("Done!!!");
    Serial.flush();
  }
}  
