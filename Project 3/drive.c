//------------------------------------------------------------------------------
//Description:  This file contains the functions and variables that control the
//              drive functionality of the car.
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------

#include  "msp430.h"
#include "macros.h"

//refrenceing the global variables in main
extern char start_moving;
extern char moving;


//Moves the car forward 
void forward(void){
  if(~(P6OUT&L_FORWARD)){
    P6OUT |= L_FORWARD;
  }
  
  if(!(P6OUT&R_FORWARD)){
    P6OUT |= R_FORWARD;
  }
  
}

//stops the car
void stop(void){
  P6OUT &= ~R_FORWARD;
  P6OUT &= ~L_FORWARD;
}
