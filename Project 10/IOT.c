//------------------------------------------------------------------------------
//Description:  This file contains the serial communication interrupts and init
//
//By:           Nathan Carels
//Date:         3/25/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------

//NOTES: use function pointers that accepts a integer time value to easily assing and save comutational power.
#include "functions.h"
#include "msp430.h"
#include "macros.h"



int command_timer;
extern int command_time;
extern int next_command_time;
extern void (*command)(int);
extern void (*next_command)(int);
extern char curr_screen4_line2[10];
extern char next_screen4_line2[10];
extern int command_speed;


char state = WAIT;
char command_state;
unsigned int state_count;

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


void navigate(int time){
    command_timer = ZERO;
    
    switch(state){ //intercept and line follow not needed for HW 8 and Project 8
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

int str_to_int(char*value){//Handles 3 values only
  int intval=ZERO;
  intval+= (value[2]-ASCII_TO_DEC);
  intval+= (value[1]-ASCII_TO_DEC)*10;
  intval+= (value[0]-ASCII_TO_DEC)*100;
  return intval;
}

char pin[5] = "1509";
char* verify_pin(char*command){
   for(int i =0; i< 4;i++){
      if(pin[i] != command[i])
        return NULL_PTR;
   }
   return &command[4];
   
}


char IP_detect_state;
char IP[SMALL_RING_SIZE];
char IP_index;
  
char capture_IP(char current){
  switch(IP_detect_state){
  case I:
    if(current == 'I')
      IP_detect_state = P;
    break;
  case P:
    if(current == 'P')
      IP_detect_state = IPSTART;
    else
      IP_detect_state = I;
    break;
  case IPSTART:
    if(current == '\n'){
      IP_detect_state = IPEND;
      return TRUE;
    }
    break;
  case IPEND:
    IP[IP_index++] = current;
    
    if(current == ':'){
      IP_detect_state = I;
      IP_index = ZERO;
    }
    return TRUE;
  default: break;
  }
  
  return FALSE;
}   

char IP_upper[10];
char IP_lower[10];
char index;
char* get_upper_IP(void){
  char counter=ZERO;
  index=ZERO;
  while(counter < 2 && index < 16){
    if(IP[index] == '.'){
      counter++;
    }
    IP_upper[index] = IP[index];
    index++;
  }
  IP_upper[index-1] = NULL_CHAR;
  return &IP_upper[1];
}

char* get_lower_IP(void){
  char placement = ZERO;
  while(IP[index] != ':' && index < 16){
    IP_lower[placement++] = IP[index];
    index++;
  }
  IP_lower[placement] = NULL_CHAR;
  return &IP_lower[ZERO];
}