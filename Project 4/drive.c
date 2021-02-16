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


#define C_TRAVEL      3700
#define MAX_COUNT     10

#define C_R_COUNT     10
#define C_L_COUNT      5


//refrenceing the global variables in main
extern char start_cycle_time;
extern unsigned int cycle_time;
extern unsigned char motor_count;
extern char state;

char fig_8_state;
//Moves the both wheels forward
void forward(void){
  if(!(P6OUT&L_FORWARD)){
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

void circle(void){
  start_cycle_time = TRUE;
  if(cycle_time < C_TRAVEL){
    if(motor_count > C_R_COUNT){
      P6OUT &= ~R_FORWARD;
    }
    if(motor_count > C_L_COUNT){
      P6OUT &= ~L_FORWARD;
    }

    if(motor_count > MAX_COUNT){
      forward();
      motor_count=RESET_STATE;
      }
    
  }else{
    start_cycle_time = FALSE;
    cycle_time = 0;
    state = 0;
    stop();
    //circle is finished
  }
}


void figure_eight(void){
  

void triangle(void){}
