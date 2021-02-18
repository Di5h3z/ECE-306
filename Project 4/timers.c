//------------------------------------------------------------------------------
//Description:  This file contains the functions and variables that control the
//              timers based on time sequence
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------

#include "macros.h"

//refrencing the global timer variables
extern volatile unsigned int Time_Sequence;
unsigned int Time_Sequence_Prev;

//for switch reset
unsigned char switch1_reset_time; //max time of 255
unsigned char switch2_reset_time; //max time of 255

//for shape/drive control
char start_cycle_time;
unsigned int cycle_time;
unsigned int motor_count;

//updating the timers every time time sequence increment
//TODO: bitmask case statment to toggle different timer on and off to decrease overhead of many timer (this is most likely not neccessary as the over head to increment is low)
void Timers_Process(void){
  //as fast as possible
  motor_count++;
  
  //based on time sequence
  if(Time_Sequence != Time_Sequence_Prev){
    Time_Sequence_Prev = Time_Sequence;
     if(Time_Sequence > 250){
      Time_Sequence = 0;
     }
     
     if(start_cycle_time)cycle_time++;
     switch1_reset_time++;
     switch2_reset_time++;
  }


}
