//------------------------------------------------------------------------------
//Description:  This file contains the functions for reading in switch input
//
//By:           Nathan Carels
//Date:         2/14/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------

#include "macros.h"
#include "msp430.h"
#include "functions.h"

//declarations


//globals that define the switch reset and operation state
extern unsigned char switch1_reset_time; //max time of 255
char switch1_readable;
char switch1_state;

extern unsigned char switch2_reset_time; //max time of 255
char switch2_readable;
char switch2_state;

//external globals the the switches trigger
extern char state;

//updates both switches
void Switches_Process(void){
  Switch1_Process();
  Switch2_Process();
}

// updates the first switch
void Switch1_Process(void){
  if(switch1_readable){
    if(!(P4IN & SW1)){
      switch1_reset_time = RESET_STATE;
      switch1_readable = FALSE;
      switch1_state = PRESSED;

      //what to do
      state = 1;

    }else{
      switch1_state = RELEASED;
    }

  }else{
    if(switch1_reset_time > SWITCH_RESET_TIME){
      switch1_readable = TRUE;
    }
  }
}

//updates the second switch
void Switch2_Process(void){
  if(switch2_readable){
    if(!(P2IN & SW2)){
      switch2_reset_time = RESET_STATE;
      switch2_readable = FALSE;
      switch2_state = PRESSED;

      //what to do
      

    }else{
      switch2_state = RELEASED;
    }

  }else{
    if(switch2_reset_time > SWITCH_RESET_TIME){
      switch2_readable = TRUE;
    }
  }
}
