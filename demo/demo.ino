#include <ax12.h>

//===========================================================================
String instring; 
boolean run = true;
//===========================================================================
void setup()
{
  delay(100);
  Serial.begin(38400); //start serial communications at 38400bps
  delay(100);
  Serial.setTimeout(10);
  SetPosition(1,512);
  Serial.println("Set up done");
  delay(1000);
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
  /*
  Serial.print("servo ");
  Serial.print(1);
  Serial.print("'s postion: ");
  Serial.println(GetPosition(1));
  */
  if (Serial.available() > 0) 
  {
    //Serial.println("get Serial");
    // read the incoming byte:
    instring = Serial.readString();
    int servo_num = 0;
    for (int i = 0; i < instring.length();i++)
    {
      if (instring[i] == 'r')
      {
        servo_num = int(instring[i+1])-48;
        //Serial.print("servo ");
        Serial.print(instring[i+1]);
        //Serial.print("'s postion: ");
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
        //Serial.print("get servo num: ");
        Serial.println(instring[i]);
        servo_num = int(instring[i])-48;
        String desire_pos = "";
        i++;
        while (instring[i] != 'b')
        {
          desire_pos += instring[i];
          i++;
        }
        //Serial.print("desire pos: ");
        //Serial.println(desire_pos);
        int temp = desire_pos.toInt();
        //Serial.print("temp: ");
        //Serial.println(temp);
        if (instring[i] == '2' )
        {
          SetPosition(2,temp);
          SetPosition(3,1024-temp);
        }
        else if (instring[i] == '3' )
        {
          SetPosition(3,temp);
          SetPosition(2,1024-temp);
        }
        else if (instring[i] == '4' )
        {
          SetPosition(4,temp);
          SetPosition(5,1024-temp);
        }
        else if (instring[i] == '5' )
        {
          SetPosition(5,temp);
          SetPosition(4,1024-temp);
        }
        else
        {
          SetPosition(servo_num,temp);
        }
      }
    } 
  }
}  
