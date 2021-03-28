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

char nav_enable;
char prev_state;
extern unsigned int Time_Sequence; 

unsigned int lspeed;
unsigned int rspeed;

unsigned int recovery_count;
unsigned int off_line_count;

extern char adc_char[6];
char l_control_str[6];
char r_control_str[6];

unsigned int vl_average;
unsigned int vr_average;

//------------------------------------------------------------------------------
// returns the control value for the eitehr side of the car                     right_side, left_side
//------------------------------------------------------------------------------
unsigned int right_side(int speed){
  int ret = KP*(BLACK_LINE_VALUE - vr_average);
  if(ret < 0)
    return 0;
  if(ret > speed)
    return speed;
  return (unsigned int)ret;

}

unsigned int left_side(int speed){
  int ret = KP*(BLACK_LINE_VALUE - vl_average);
  if(ret < 0)
    return 0;
  if(ret > speed)
    return speed;
  return (unsigned int)ret;
}



//------------------------------------------------------------------------------
// Gets the current state of the car relative to the line                       get_state
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
// Handels the tracking an navigation of a line                                 line_nav
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
    
    if(state == WHITE_STATE){                           //dertermins the direction if the car has deviated from the line
      recovery_count = 0;
      
      switch(prev_state){
      case RIGHT_STATE:
        rspeed = speed;
        lspeed = MIN_SPEED;
        break;
      case LEFT_STATE:
        rspeed = MIN_SPEED;
        lspeed = speed;
        break;
      default:
        
        Time_Sequence = 0;
        char one_time =1;
        while(ALWAYS){                                    //why did it get here debug (this breaks the code intentionally)
            if(Time_Sequence < 15 && one_time){
                  R_reverse(MAX_SPEED);
                  L_reverse(MAX_SPEED);
                  
            }else{
                  one_time = 0;
                  R_stop();L_stop();
            }
            if(state == WHITE_STATE)
              lcd_line1("WHITESTATE");
           

            }
          break;
        
        }
    
    }else{
                                                        //the recovery functions for it is has deviated from the line when it finds the line again
      if(prev_state = LEFT_STATE && recovery_count < RECOVERY_TIME){            
        rspeed = speed;
        lspeed = MIN_SPEED;
      }else if(prev_state = RIGHT_STATE && recovery_count < RECOVERY_TIME){
        rspeed = MIN_SPEED;
        lspeed = speed;
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
      off_line_count = 0;
      prev_state = state;
      }
      
    }
                                                        //ouputs the speed to the wheels
    R_forward(rspeed);
    L_forward(lspeed);
    nav_enable = FALSE;                                 //diables the function until reactivated in the timer
  }     
  
}
  



