//------------------------------------------------------------------------------
//Description:  This file contains the functions and variables that control the
//              drive functionality of the car.
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------
#include "functions.h"
#include "msp430.h"
#include "macros.h"

//Globals
  char right_wheel_state;
  char right_wheel_count;
  char right_forward_flag = FALSE;
  char right_reverse_flag = FALSE;

  char left_wheel_state;
  char left_wheel_count;
  char left_forward_flag = FALSE;
  char left_reverse_flag = FALSE;

  unsigned int speed_count;


//------------------------------------------------------------------------------
// Turns the left wheel off                                                     L_stop
// Passed:      None
// Returned:    None
//------------------------------------------------------------------------------
void L_stop(void){
  LEFT_FORWARD_SPEED = WHEEL_OFF;
  LEFT_REVERSE_SPEED = WHEEL_OFF;
}
//------------------------------------------------------------------------------
//Turns the right wheel off                                                     R_stop
// Passed:      None
// Returned:    None
//------------------------------------------------------------------------------
void R_stop(void){
  RIGHT_FORWARD_SPEED = WHEEL_OFF;
  RIGHT_REVERSE_SPEED = WHEEL_OFF;
}

//------------------------------------------------------------------------------
//Turns the wheel forward checks to make sure proper time since reverse         L_forward
// Passed:      speed
// Returned:    None
//------------------------------------------------------------------------------
void L_forward(unsigned int speed){
  
  //check to see state is reverse if not proceed as normal
  if(left_wheel_state==REVERSE){
    L_stop();
    left_wheel_state = FORWARD;
    left_wheel_count = RESET_STATE;
    left_forward_flag = FALSE;
    left_reverse_flag = FALSE;
  }else{

    // if the count is done set the flag to allow drive
    if(left_wheel_count > CHANGE_COUNT){ 
      left_forward_flag = TRUE;
    }

    //if the flag is set and verification that L_reverse is zero
    if(left_forward_flag && !(P6OUT & L_REVERSE)){ 
      LEFT_FORWARD_SPEED = speed;
    }else{
      LEFT_FORWARD_SPEED = WHEEL_OFF;
    }
  }
 
}

//------------------------------------------------------------------------------
//Turns the wheel forward checks to make sure proper time since reverse         R_forward
// Passed:      speed
// Returned:    None
//------------------------------------------------------------------------------
void R_forward(unsigned int speed){
  
 //check to see state is reverse if not proceed as normal 
 if(right_wheel_state==REVERSE){
  R_stop();
  right_wheel_state = FORWARD;
  right_wheel_count = RESET_STATE;
  right_forward_flag = FALSE;
  right_reverse_flag = FALSE;
 }else{
 
   // if the count is done set the flag to allow drive 
   if(right_wheel_count > CHANGE_COUNT){ 
    right_forward_flag = TRUE;
   }
   
   //if the flag is set and verification that R_reverse is zero 
   if(right_forward_flag && !(P6OUT & R_REVERSE)){ 
    RIGHT_FORWARD_SPEED = speed;
   }else{
    RIGHT_FORWARD_SPEED =WHEEL_OFF;
   }
 }

}

//------------------------------------------------------------------------------
//Turns the wheel reverse checks to make sure proper time since forward         L_reverse
// Passed:      speed
// Returned:    None
//------------------------------------------------------------------------------
void L_reverse(unsigned int speed){
  
  //check to see state is reverse if not proceed as normal
  if(left_wheel_state == FORWARD){
    L_stop();
    left_wheel_state = REVERSE;
    left_wheel_count = RESET_STATE;
    left_forward_flag = FALSE;
    left_reverse_flag = FALSE;
  }else{
    
    // if the count is done set the flag to allow drive
    if(left_wheel_count > CHANGE_COUNT){
      left_reverse_flag = TRUE;
    }
    
    //if the flag is set and verification that L_forward is zero
    if(left_reverse_flag && !(P6OUT & L_FORWARD)){
      LEFT_REVERSE_SPEED = speed;
    }else{
      LEFT_REVERSE_SPEED = WHEEL_OFF;
    }
  }
}

//------------------------------------------------------------------------------
//Turns the wheel reverse checks to make sure proper time since forward         R_reverse
// Passed:      speed
// Returned:    None
//------------------------------------------------------------------------------
void R_reverse(unsigned int speed){
   
     //check to see state is reverse if not proceed as normal
   if(right_wheel_state==FORWARD){
    R_stop();
    right_wheel_state = REVERSE;
    right_wheel_count = RESET_STATE;
    right_forward_flag = FALSE;
    right_reverse_flag = FALSE;
   }else{
   
     // if the count is done set the flag to allow drive
     if(right_wheel_count > CHANGE_COUNT){ 
      right_reverse_flag = TRUE;
     }
     //if the flag is set and verification that R_forward is zero
     if(right_reverse_flag && !(P6OUT & R_FORWARD)){ 
      RIGHT_REVERSE_SPEED = speed;
     }else{
      RIGHT_REVERSE_SPEED = WHEEL_OFF;
     }
   }

}









