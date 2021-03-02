//------------------------------------------------------------------------------
//Description:  This file contains the functions and variables that control the
//              timers based on time sequence
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------
#include  "msp430.h"
#include "macros.h"

//refrencing the global timer variables
  extern volatile unsigned int Time_Sequence;
  unsigned int Time_Sequence_Prev;

//for switch reset
  unsigned char switch1_reset_time; //max time of 255
  unsigned char switch2_reset_time; //max time of 255

//for shape/drive control
  extern char right_wheel_count;
  extern char left_wheel_count;

  extern unsigned int speed_count;

 //for updating state machine in main
  extern unsigned int state_count;
  
  
//updating the timers every time time sequence increment
void Timers_Process(void){
  //as fast as possible
  speed_count++;
  if(speed_count > MAX_SPEED){
    speed_count = RESET_STATE;
  }
  
  //based on time sequence
  if(Time_Sequence != Time_Sequence_Prev){
    Time_Sequence_Prev = Time_Sequence;
     if(Time_Sequence > 250){
      Time_Sequence = RESET_STATE;
     }
     
     state_count++;
     right_wheel_count++;
     left_wheel_count++;
     switch1_reset_time++;
     switch2_reset_time++;
  }


}
