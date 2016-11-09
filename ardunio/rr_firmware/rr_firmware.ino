#include <ax12.h>

#define NO_COMMAND 0
#define READ_COMMAND -9999
#define SET_COMMAND -1

int state = 1 // state starts in NO_COMMAND

//---------------------------------------------------------------
union Data {
   char bufc[16];
   unsigned short int bufi[8];
};

uint16_t desired_loc[]= {512, 512, 512, 512, 512};
uint16_t current_loc[]= {512, 512, 512, 512, 512};

int cur_pos = 0;
boolean correctmsg = true;

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
  //Serial.println("SetUp Done");
  
  boolean IN_NO_COMMAND_STATE
}

//---------------------------------------------------------------
void loop(){
  //----------------
  //SetPositions
  switch(state):
    case NO_COMMAND:  
      if(Serial.available() > 0){
        //READ FIRST BYTE
        state = Serial.read() // CONVERT ME CAUSE IM READING ONLY ONE BYTE WHEN I SHOULD BE READING 2     
      }
      break;
      
    case READ_COMMAND:
      // DO READ STUFF  
      break;
      
    case SET_COMMAND:
      // DO SET STUFF 
       
    default:
      state = NO_COMMAND;
      // WE NEED TO FLUSH
      break;
  
  if (Serial.available() == 16) {
    union Data data;
    //Serial.println("have 16 bytes");
    //Serial.println(Serial.available());
    //read
    int len = 0;
    len = Serial.readBytes(data.bufc, 17);
    
    //Serial.println("after read");
    //Serial.println(len);
    //Serial.println(data.bufi[0]);
    //check start
    if (data.bufi[0] != -1) {
      correctmsg = false;
      Serial.println("-1");
    }
    //Serial.println("start checked"); 
    //check end
    if (data.bufi[7] != -99) {
      correctmsg = false;
      Serial.println("-1");
    }
    //Serial.println("end checked");
    //calculate sum
    int sum = 0;
    for (int i=2; i<7; ++i) {
      sum = sum + data.bufi[i];
    }
    //Serial.println("sum calculated");
    //check sum
    if (data.bufi[1] != sum) {
      correctmsg = false;
      Serial.println("-1");
    }
    //Serial.println("sum checked");
    if (correctmsg) {
      //Serial.println("before for loop");
      for (int i=0; i<5; i++) {
        desired_loc[i] = data.bufi[i+2];
        //Serial.println(desired_loc[i]);
      }
      //Serial.println("before goto");
      go_to();
    }
    //Serial.println("false");
    correctmsg = true;
  }
  //----------------
  //GetPositions
  else if (Serial.available() == 2) {
    union Data data;
    //Serial.println("have 2 bytes");
    //read
    for (int i=0; i<2; ++i) {
      data.bufc[i] = Serial.read();
      if (data.bufc[i] == -1) --i;
    }
    
    //check num
    if (data.bufi[0] != -9999) {
      correctmsg = false;
      //Serial.println("-1");
    }
    
    if (correctmsg) {
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
    }
    correctmsg = true;
  }
  //----------------
  //error msg
  else if( Serial.available() > 16) {
    //Serial.println("else");
    int num_bytes_to_read = Serial.available();
    while (num_bytes_to_read > 0) {
      num_bytes_to_read-=1;
      //Serial.println("read");
      Serial.read();
    }
  }
  
} // loop
