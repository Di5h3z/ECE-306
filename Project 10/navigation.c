//------------------------------------------------------------------------------
//Description:  This file contains the ADC interrupts and init
//
//By:           Nathan Carels
//Date:         3/14/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------
#include "functions.h"
#include "msp430.h"
#include "macros.h"


//Globals
  //Navigation and sub functions
  char nav_enable;
  char prev_state;
  char recovery_state;
  extern unsigned int Time_Sequence; 

  unsigned int lspeed;
  unsigned int rspeed;

  unsigned int recovery_count;
  unsigned int off_line_count;

  extern char adc_char[BCD_MAX_LEN];
  char l_control_str[BCD_MAX_LEN];
  char r_control_str[BCD_MAX_LEN];

  unsigned int vl_average;
  unsigned int vr_average;

//------------------------------------------------------------------------------
// returns the control value for the right side of the car                      right_side
// Passed:      speed(navigation speed)
// Returned:    ret(proportional constrol value)
//------------------------------------------------------------------------------
unsigned int right_side(int speed){
  int ret = KP*(BLACK_LINE_VALUE - vr_average);
  if(ret < ZERO)
    return ZERO;
  if(ret > speed)
    return speed;
  return (unsigned int)ret;

}

//------------------------------------------------------------------------------
// returns the control value for the left side of the car                       left_side
// Passed:      speed(navigation speed)
// Returned:    ret(proportional constrol value)
//------------------------------------------------------------------------------
unsigned int left_side(int speed){
  int ret = KP*(BLACK_LINE_VALUE - vl_average);
  if(ret < ZERO)
    return ZERO;
  if(ret > speed)
    return speed;
  return (unsigned int)ret;
}



//------------------------------------------------------------------------------
// Gets the current state of the car relative to the line                       get_state
// Passed:      None
// Returned:    (state of car relative to line)
//------------------------------------------------------------------------------
char get_state(void){
  if(vl_average < WHITE_VALUE_MAX && vr_average < WHITE_VALUE_MAX){
    return WHITE_STATE;
  }else if(vl_average < WHITE_VALUE_MAX){
    return LEFT_STATE;
  }else if(vr_average < WHITE_VALUE_MAX){
    return RIGHT_STATE;
  }else{
    return LINE_STATE;
  }

}




//------------------------------------------------------------------------------
// Handles the tracking an navigation of a line                                 line_nav
// Passed:      speed(rate of line navagation)
// Returned:    None
//------------------------------------------------------------------------------
void line_nav(int speed){
  
  if(nav_enable){
  
    vl_average = VLEFT_AVERAGE;                         //gets the current averages
    vr_average = VRIGHT_AVERAGE;
    
    unsigned int control_right = right_side(speed);     //gets control value
//    HEXtoBCD(control_right);                          //Debug code
//    str_cpy(r_control_str, adc_char);
//    lcd_line2(r_control_str);
    
    
    unsigned int control_left = left_side(speed);       //gets control value
//    HEXtoBCD(control_left);                           //Debug code
//    str_cpy(l_control_str, adc_char);
//    lcd_line3(l_control_str);    
    
    unsigned int rspeed = speed - control_left;         //slows down the opposite wheel
    unsigned int lspeed = speed - control_right;
    
    
    char state = get_state();                           //gets the current state
    
    if(state == WHITE_STATE){                           //determines the direction if the car has deviated from the line
      recovery_count = RESET;
      
      switch(prev_state){
      case RIGHT_STATE:
        recovery_state = RIGHT_STATE;
        rspeed = speed;
        lspeed = MIN_SPEED;
        screen4_line3 = "R_STATE";
        break;
      case LEFT_STATE:
        recovery_state = LEFT_STATE;
        rspeed = MIN_SPEED;
        lspeed = speed;
        screen4_line3 = "L_STATE";
        break;
      default:
        
        Time_Sequence = RESET;
        rspeed = MIN_SPEED;
        lspeed = speed;   
          break;
        
        }
    
    }else{
                                                        //the recovery functions for it is has deviated from the line when it finds the line again
      if(recovery_state == LEFT_STATE && recovery_count < RECOVERY_TIME){            
        rspeed = speed;
        lspeed = MIN_SPEED;
        screen4_line2 = "L_RECOVER";
      }else if(recovery_state == RIGHT_STATE && recovery_count < RECOVERY_TIME){//has an issue hitting this recovery i.e it doesnt do it
        rspeed = MIN_SPEED;
        lspeed = speed;
        screen4_line2 = "R_RECOVER";
      }else{
        
                                                        //If both speeds are under saturated it chooses a direction based off of the sensors
        if( OFF_SPEED > rspeed && OFF_SPEED > lspeed){  // change these numbers to > 0 if it keeps getting stuck
          if(vl_average > vr_average){                  //if its to the right of the line
            rspeed = speed;
            lspeed = MIN_SPEED;
          }else{                                        //if its to the left of the line
            rspeed = MIN_SPEED;
            lspeed = speed;        
          }
        }
      off_line_count = RESET;
      prev_state = state;
      recovery_state =LINE_STATE;
      }
      
    }
                                                        //ouputs the speed to the wheels
    R_forward(rspeed);
    L_forward(lspeed);
    nav_enable = FALSE;                                 //diables the function until reactivated in the timer
  }     
  
}
  



