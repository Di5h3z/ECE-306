//------------------------------------------------------------------------------
//Description:  This file contains the functions and variables that control the
//              drive functionality of the car.
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------

#include "msp430.h"
#include "macros.h"

#define FORWARD 1
#define REVERSE 0
#define CHANGE_COUNT 10
#define MAX_SPEED 1000;


//globals
char right_wheel_state;
char right_wheel_count;
char right_forward_flag = FALSE;
char right_reverse_flag = FALSE;

char left_wheel_state;
char left_wheel_count;
char left_forward_flag = FALSE;
char left_reverse_flag = FALSE;

unsigned int speed_count;
//#define R_FORWARD            (0x01)
//#define L_FORWARD            (0x02)
//#define R_REVERSE            (0x04)
//#define L_REVERSE            (0x08)


// Turns the left wheel off
void L_stop(void){
  P6OUT &= ~L_FORWARD;
  P6OUT &= ~L_REVERSE;
}

//Turns the right wheel off
void R_stop(void){
  P6OUT &= ~R_FORWARD;
  P6OUT &= ~R_REVERSE;
}

//speed is out of 1000
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
    if(left_forward_flag && speed_count < speed && !(P6OUT & L_REVERSE)){ 
      P6OUT |= L_FORWARD;
    }else{
      P6OUT &= ~L_FORWARD;
    }
  }
 
}

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
   if(right_forward_flag && speed_count < speed && !(P6OUT & R_REVERSE)){ 
    P6OUT |= R_FORWARD;
   }else{
    P6OUT &= ~R_FORWARD;
   }
 }

}

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
    if(left_reverse_flag && speed_count < speed && !(P6OUT & L_FORWARD)){
      P6OUT |= L_REVERSE;
    }else{
      P6OUT &= ~L_REVERSE;
    }
  }
}

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
     if(right_reverse_flag && speed_count < speed && !(P6OUT & R_FORWARD)){ 
      P6OUT |= R_REVERSE;
     }else{
      P6OUT &= ~R_REVERSE;
     }
   }

}


