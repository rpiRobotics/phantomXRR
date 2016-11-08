#include <ax12.h>



class Reactor 
{
public:

  int desired_joint_positions[7];
  int current_joint_positions[7];
  
  
  void get_joint_positions(){
    for(int i=1; i<9; i++){
      if(i==7)
        i=8;
      current_joint_positions[i-1] = GetPosition(i);
    }
  }
  
  void go_to_neutral(){
    for(int i=1; i<9; i++){
      if(i==7)
        i=8;
      SetPosition(i,512);
    }
    
  }
  
  // update all servos
  void update(){
    get_joint_positions();
    boolean done= false;
    while(!done){
      done = true;
      for(int i=1; i<9; i++){
         if(i == 7)
           i=8;
         if(abs(GetPosition(i) -  desired_joint_positions[i-1]) > 5){
           done = false;
           if(GetPosition(i) > desired_joint_positions[i-1])
             current_joint_positions[i-1]--;
           else
             current_joint_positions[i-1]++;
           
           SetPosition(i,current_joint_positions[i-1]);
         }
      }
      delay(10);
    }
  }
  
};


//===========================================================================
String instring; 
//===========================================================================
void setup()
{
  Serial.begin(38400); //start serial communications at 38400bps
  Serial.setTimeout(2);
  Reactor my_reactor;
  
  for(int i = 0; i<8; i++)
    my_reactor.desired_joint_positions[i] = 512;
    
  delay(3);
  my_reactor.go_to_neutral();
  delay(3);
  my_reactor.update();
}

//============================================================================
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
    // read the incoming byte:
    instring = Serial.readString();
    int servo_num = 0;
    for (int i = 0; i < instring.length();i++)
    {
      if (instring[i] == 'r')
      {
        servo_num = int(instring[i+1])-48;
        Serial.print(instring[i+1]);
        Serial.println(GetPosition(servo_num));
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
        if (instring[i] == '2' )
        {
          SetPosition(2,temp);
          SetPosition(3,1024-temp);
        }
        else if (instring[i] == '3' )
        {
          SetPosition(2,temp);
          SetPosition(3,1024-temp);
        }
        else if (instring[i] == '4' )
        {
          SetPosition(4,temp);
          SetPosition(5,1024-temp);
        }
        else if (instring[i] == '5' )
        {
          SetPosition(4,temp);
          SetPosition(5,1024-temp);
        }
        else
        {
          SetPosition(servo_num,temp);
        }
      }
    } 
  }
}  

