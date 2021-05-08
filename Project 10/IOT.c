//------------------------------------------------------------------------------
//Description:  This file contains the serial communication interrupts and init
//
//By:           Nathan Carels
//Date:         3/25/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------
#include "functions.h"
#include "msp430.h"
#include "macros.h"


//Globals
  //Command Queue 
  int command_timer;
  extern int command_time;
  extern int next_command_time;
  extern void (*command)(int);
  extern void (*next_command)(int);
  extern char curr_screen4_line2[MAX_LCD_LENGTH];
  extern char next_screen4_line2[MAX_LCD_LENGTH];
  extern int command_speed;

  //Intercept State Machine
  char state = WAIT;
  char command_state;
  unsigned int state_count;
  
  //PIN verification
  char pin[PIN_LENGTH] = "1509";
  
  //IP Detection
  char IP_detect_state;
  char IP[SMALL_RING_SIZE];
  char IP_index;
  
  //IP Display
  char IP_upper[MAX_LCD_LENGTH];
  char IP_lower[MAX_LCD_LENGTH];
  char index;

//------------------------------------------------------------------------------
// command to exit the circle                                                   exit
// Passed:      time(duration the command will run)
// Returned:    None
//------------------------------------------------------------------------------
void exit(int time){
  if(command_timer < EXIT_DRIVE_TIME){
    R_forward(command_speed);
    L_forward(command_speed-EXIT_DRIVE_ADJUST);
  }else{
    state = STOP;
    R_stop();
    L_stop();
  }
  
}

//------------------------------------------------------------------------------
// command to intercept and navigate the black line                             navigate
// Passed:      time(duration the command will run)
// Returned:    None
//------------------------------------------------------------------------------
void navigate(int time){
    command_timer = ZERO;
    
    switch(state){ 
    case WAIT:
      state_count =ZERO;
      state = WHITE_DETECT;
      break;
    case WHITE_DETECT:
      if((return_vleft_average() < WHITE_VALUE_MAX) && (return_vright_average() < WHITE_VALUE_MAX)){
        state = LINE_DETECT;
      }else{
        R_forward(INTERCEPT_ARC_RSPEDD);L_forward(INTERCEPT_ARC_LSPEED);
      }
      
      break;
    case LINE_DETECT:
      if((return_vleft_average() > BLACK_LINE) && (return_vright_average() > BLACK_LINE)){
        R_stop();L_stop();
        state = REVERSE_STATE;
        state_count =ZERO;
      }else{
        R_forward(INTERCEPT_ARC_RSPEDD);L_forward(INTERCEPT_ARC_LSPEED);
      }
      break;
    case REVERSE_STATE:
       if(state_count > MOMENTUM_BREAK_TIME){
        R_stop();L_stop();
        state = SPIN_BLACK_LINE;
        state_count =ZERO;
      }else{
        R_reverse(SLOW_SPEED);L_reverse(SLOW_SPEED);
      }
      break;
      
    case SPIN_BLACK_LINE:
      if(state_count > SPIN_TIME){
        R_stop();L_stop();      
        if(state_count > SPIN_TIME+FIVE_SECONDS){
          state_count =ZERO;
          state = NAVIGATION;
        }   
      }else{
        L_reverse(SLOW_SPEED);R_forward(SLOW_SPEED);
      }
      break;
    case NAVIGATION:
      if(state_count > LINE_NAV_TIME){
        R_stop();L_stop();
        if(state_count > LINE_NAV_TIME+FIVE_SECONDS){
          state_count =ZERO;
          state = CIRCLE;
        }
      }else{
        line_nav(NAV_SPEED);
      }
      break;
      
    case CIRCLE:
        line_nav(NAV_SPEED);
        break;
    default:break;
    }
}



//------------------------------------------------------------------------------
// command to turn right                                                        right_turn
// Passed:      time(duration the command will run)
// Returned:    None
//------------------------------------------------------------------------------
void right_turn(int time){
  if(command_timer < time){
    R_reverse(command_speed);
    L_forward(command_speed);
  }else{
    R_stop();
    L_stop();
    command = next_command;
    command_time = next_command_time;
    command_timer = ZERO;
    next_command_time = ZERO;
    str_cpy(curr_screen4_line2, next_screen4_line2);
  }
}

//------------------------------------------------------------------------------
// command to turn left                                                         left_turn
// Passed:      time(duration the command will run)
// Returned:    None
//------------------------------------------------------------------------------
void left_turn(int time){
  if(command_timer < time){
    R_forward(command_speed);
    L_reverse(command_speed);
  }else{
    R_stop();
    L_stop();
    command = next_command;
    command_time = next_command_time;
    command_timer = ZERO;
    next_command_time = ZERO;
    str_cpy(curr_screen4_line2, next_screen4_line2);
  }
}

//------------------------------------------------------------------------------
// command to reverse                                                           reverse
// Passed:      time(duration the command will run)
// Returned:    None
//------------------------------------------------------------------------------
void reverse(int time){
  if(command_timer < time){
    R_reverse(command_speed);
    L_reverse(command_speed);
  }else{
    R_stop();
    L_stop();
    command = next_command;
    command_time = next_command_time;
    command_timer = ZERO;
    next_command_time = ZERO;
    str_cpy(curr_screen4_line2, next_screen4_line2);
  }
}

//------------------------------------------------------------------------------
// command to go forward                                                        forward
// Passed:      time(duration the command will run)
// Returned:    None
//------------------------------------------------------------------------------
void forward(int time){
  if(command_timer < time){
    R_forward(command_speed);
    L_forward(command_speed);
  }else{
    R_stop();
    L_stop();
    command = next_command;
    command_time = next_command_time;
    command_timer = ZERO;
    next_command_time = ZERO;
    str_cpy(curr_screen4_line2, next_screen4_line2);
  }
}

//------------------------------------------------------------------------------
// converts a string to integer (max 3 digits)                                  str_to_int
// Passed:      value(pointer to int as string)
// Returned:    inval(value from string)
//------------------------------------------------------------------------------
int str_to_int(char*value){//Handles 3 values only
  int intval=ZERO;
  intval+= (value[THIRD_CHAR]-ASCII_TO_DEC);
  intval+= (value[SECOND_CHAR]-ASCII_TO_DEC)*TEN;
  intval+= (value[FIRST_CHAR]-ASCII_TO_DEC)*HUNDRED;
  return intval;
}


//------------------------------------------------------------------------------
// checks the pin and returns command if pin is verified                        verify_pin
// Passed:      command(pointer to pin)
// Returned:    command(pointer to start of command)
//------------------------------------------------------------------------------
char* verify_pin(char*command){
   for(int i =BEGINNING; i< PIN_LENGTH-1;i++){
      if(pin[i] != command[i])
        return NULL_PTR;
   }
   return &command[FITH_CHAR];
   
}

  
//------------------------------------------------------------------------------
// checks for and captures the IP when detect                                   capture_IP
// Passed:      command(pointer to pin)
// Returned:    command(pointer to start of command)
//------------------------------------------------------------------------------
char capture_IP(char current){
  switch(IP_detect_state){              //state machine to find when IP occurs
  case I:
    if(current == I_CHAR)
      IP_detect_state = P;
    break;
  case P:
    if(current == P_CHAR)
      IP_detect_state = IPSTART;
    else
      IP_detect_state = I;
    break;
  case IPSTART:
    if(current == NEWLINE_CHAR){
      IP_detect_state = IPEND;
      return TRUE;
    }
    break;
  case IPEND:                           //Captures the IP
    IP[IP_index++] = current;
    
    if(current == COLON_CHAR){
      IP_detect_state = I;
      IP_index = ZERO;
    }
    return TRUE;
  default: break;
  }
  
  return FALSE;
}   



//------------------------------------------------------------------------------
// finds the Upper portion of the IP from the IP buffer                         get_upper_IP
// Passed:      None
// Returned:    IP_upper(pointer upper portion of the IP)
//------------------------------------------------------------------------------
char* get_upper_IP(void){
  char counter=ZERO;
  index=ZERO;
  while(counter < NUM_PERIODS_DETECTION && index < SMALL_RING_SIZE){    //finds the first portion of the IP
    if(IP[index] == PERIOD_CHAR){
      counter++;
    }
    IP_upper[index] = IP[index];
    index++;
  }
  IP_upper[index-1] = NULL_CHAR;                                        
  return &IP_upper[SECOND_CHAR];
}

//------------------------------------------------------------------------------
// finds the Lower portion of the IP from the IP buffer                         get_lower_IP
// Passed:      None
// Returned:    IP_lower(pointer lower portion of the IP)
//------------------------------------------------------------------------------
char* get_lower_IP(void){
  char placement = ZERO;
  while(IP[index] != COLON_CHAR && index < SMALL_RING_SIZE){            //gets the second portion of the IP
    IP_lower[placement++] = IP[index];
    index++;
  }
  IP_lower[placement] = NULL_CHAR;
  return &IP_lower[ZERO];
}