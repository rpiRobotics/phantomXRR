#include <ax12.h>

#define READ_COMMAND 114
#define SET_COMMAND 115

//---------------------------------------------------------------
union Data {
   char bufc[12];
   unsigned short int bufi[6];
};

uint16_t desired_loc[]= {512, 512, 512, 512, 512};
uint16_t current_loc[]= {512, 512, 512, 512, 512};

int cur_pos = 0;
boolean correctmsg = false;
char state = 0;


//---------------------------------------------------------------
void go_to(){
  //Joint 1
  //Serial.println("Move Joint 1");
  cur_pos = GetPosition(1);
  //Serial.println(cur_pos);
  //Serial.println(desired_loc[0]);
  if ( (cur_pos < desired_loc[0]-5) || (cur_pos > desired_loc[0]+5) ) {
    SetPosition(1,desired_loc[0]);
  }
  //Joint 4
  //Serial.println("Move Joint 4");
  cur_pos = GetPosition(6);
  //Serial.println(cur_pos);
  //Serial.println(desired_loc[3]);
  if ( (cur_pos < desired_loc[3]-5) || (cur_pos > desired_loc[3]+5) ) {
    SetPosition(6,desired_loc[3]);
  }
  //Joint 5
  //Serial.println("Move Joint 5");
  cur_pos = GetPosition(8);
  //Serial.println(cur_pos);
  //Serial.println(desired_loc[4]);
  if ( (cur_pos < desired_loc[4]-5) || (cur_pos > desired_loc[4]+5) ) {
    SetPosition(8,desired_loc[4]);
  }
  //Joint 2
  //Serial.println("Move Joint 2");
  cur_pos = GetPosition(2);
  //Serial.println(cur_pos);
  //Serial.println(desired_loc[1]);
  if ( (cur_pos < desired_loc[1]-5) || (cur_pos > desired_loc[1]+5) ) {
    SetPosition(2,desired_loc[1]); 
    SetPosition(3,1024-desired_loc[1]);
  }
  //Joint 3
  //Serial.println("Move Joint 3");
  cur_pos = GetPosition(4);
  //Serial.println(cur_pos);
  //Serial.println(desired_loc[2]);
  if ( (cur_pos < desired_loc[2]-5) || (cur_pos > desired_loc[2]+5) ) {
    SetPosition(4,desired_loc[2]);
    SetPosition(5,1024-desired_loc[2]);
  } 
}

//---------------------------------------------------------------
void setup(){
  Serial.begin(38400); //start serial communications at 38400bps
  Serial.setTimeout(2); 
  //Serial.println("setup done");
}

//---------------------------------------------------------------
void loop(){
  delay(1);
  union Data data;
  switch(state){
    //-----------------------------------------------------------------
    
    //GetPositions
    case READ_COMMAND:
      // DO READ STUFF  
      //Serial.println("current_loc init");
      current_loc[0] = GetPosition(1);
      current_loc[1] = GetPosition(2);
      current_loc[2] = GetPosition(4);
      current_loc[3] = GetPosition(6);
      current_loc[4] = GetPosition(8);
        
      for (int i=0; i<5; ++i) {
        Serial.print(i+1);
        Serial.println(current_loc[i]);
      }
        
      state = 0; // GO BACK TO DEFAULT STATE
      break;
      
      
    //-------------------------------------------------------------
    //SetPositions  
    case SET_COMMAND:{
      //Serial.println("set state");
      //DO SET STUFF
      
      // CHECK TO MAKE SURE WE HAVE THE PROPER NUMBER OF BYTES
      delay(10); // LET THINGS ACCUMULATE
      if(Serial.available() < 12){
        //Serial.println("serial < 12");
        state = 0;
        break;  
      }
      
      //Serial.print("Serial: ");
      //Serial.println(Serial.available());
      int bytesread = 0;
      
      for (int i=0;i<12;++i) {
        data.bufc[i] = Serial.read();
      }
      
      //bytesread = Serial.readBytes(data.bufc, 13);
      //Serial.print("bytesread: ");
      //Serial.println(bytesread);
      //Serial.println();
      //calculate sum
      int sum = 0;
      
      for (int i=1; i<6; ++i) {
        sum = sum + data.bufi[i];
      }
      //check sum
      if (data.bufi[0] == sum) {
        correctmsg = true;
        //Serial.println("sum checked");
      }
      
      //Serial.print("correctmsg: ");
      //Serial.println(correctmsg);
      
      if (correctmsg) {
        for (int i=1; i<6; i++) {
          desired_loc[i-1] = data.bufi[i];
        }
        //Serial.println("before go_to");
        go_to();
      }
      
      correctmsg = false;
      state = 0; // GO BACK TO DEFAULT STATE
      break;
    }
    
    //--------------------------------------------------------------
    //Default 
    default:{
       if(Serial.available() > 0){
        //Serial.print("Serial: ");
        //Serial.println(Serial.available());
        state = Serial.read();
      }      
    }  
  }
}  
