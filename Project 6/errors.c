//------------------------------------------------------------------------------
//  Description: This file contains the function for error checking
//
//By:           Nathan Carels
//Date:         3/9/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------
#include "functions.h"
#include "msp430.h"
#include "macros.h"

extern unsigned int Time_Sequence;

void wheel_polarity_error(void){
  //checks to see if the same wheel is being driven in oposite directions
  if((RIGHT_FORWARD_SPEED && RIGHT_REVERSE_SPEED) || (LEFT_FORWARD_SPEED && LEFT_REVERSE_SPEED)){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;
    lcd_line4("WHEEL ERR");
    while(ALWAYS){
      if(Time_Sequence == 250){
        LCD_TOGGLE;
      }
    }
  }

  if(((P6OUT & R_FORWARD)&&(P6OUT & R_REVERSE)) || ((P6OUT & L_FORWARD)&&(P6OUT & L_REVERSE))){
    P6OUT &= ~R_FORWARD;
    P6OUT &= ~R_REVERSE;
    P6OUT &= ~L_FORWARD;
    P6OUT &= ~L_REVERSE;
    lcd_line4("WHEEL ERR");
    while(ALWAYS){
      if(Time_Sequence == 250){
        LCD_TOGGLE;
      }
    }
  }
}
