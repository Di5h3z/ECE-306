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

#define VLEFT_AVERAGE return_vleft_average()
#define VRIGHT_AVERAGE return_vright_average()


#define MIN_SPEED 0
#define MAX_SPEED_NAV 5000
#define BLACK_LINE_VALUE 621
#define WHITE_VALUE_MAX 150

#define KP 25
#define KD 100

char pid_enable;
char prev_state;
unsigned int lspeed;
unsigned int rspeed;
unsigned int recovery_count;

extern char adc_char[6];
char l_control_str[6];
char r_control_str[6];

unsigned int vl_average;
unsigned int vr_average;


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

#define WHITE_STATE 0
#define LEFT_STATE 1
#define RIGHT_STATE 2
#define LINE_STATE 3

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


#define OFF_SPEED 1500
#define RECOVERY_TIME 150
extern unsigned int Time_Sequence; 
unsigned int off_line_count;


void line_nav(int speed){
  
  if(pid_enable){
  
    vl_average = VLEFT_AVERAGE;
    vr_average = VRIGHT_AVERAGE;
    
    unsigned int control_right = right_side(speed);
    HEXtoBCD(control_right);
    str_cpy(r_control_str, adc_char);
    lcd_line2(r_control_str);
    
    
    unsigned int control_left = left_side(speed); 
    HEXtoBCD(control_left);
    str_cpy(l_control_str, adc_char);
    lcd_line3(l_control_str);    
    
    unsigned int rspeed = speed - control_left; //slows down the opposite wheel
    unsigned int lspeed = speed - control_right;
    
    
    char state = get_state();
    
    if(state == WHITE_STATE){
      recovery_count = 0;
      
      switch(prev_state){
      case RIGHT_STATE:
        rspeed = speed;
        lspeed = MIN_SPEED;
        lcd_line1("RIGHTSTATE");
        break;
      case LEFT_STATE:
        rspeed = MIN_SPEED;
        lspeed = speed;
        lcd_line1("LEFTSTATE");
        break;
      default:
       Time_Sequence = 0;
      char one_time =1;
      while(ALWAYS){ //why did it get here debug
          if(Time_Sequence < 15 && one_time){
                R_reverse(10000);
                L_reverse(10000);
                
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
      if(prev_state = LEFT_STATE && recovery_count < RECOVERY_TIME){
        rspeed = speed;
        lspeed = MIN_SPEED;
        lcd_line1("RECOVERY");
      }else if(prev_state = RIGHT_STATE && recovery_count < RECOVERY_TIME){
        rspeed = MIN_SPEED;
        lspeed = speed;
        lcd_line1("RECOVERY");
      }else{

        
        if( OFF_SPEED > rspeed && OFF_SPEED > lspeed){ // change these numbers to > 0 if it keeps getting stuck
          if(vl_average > vr_average){ //if its to the right of the line
            rspeed = speed;
            lspeed = MIN_SPEED;
          }else{//if its to the left of the line
            rspeed = MIN_SPEED;
            lspeed = speed;        
          }
        }
      off_line_count = 0;
      prev_state = state;
      }
      
    }
    
    R_forward(rspeed);
    L_forward(lspeed);
    pid_enable = FALSE;
  }
  
}
  



