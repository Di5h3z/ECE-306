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


#define I 0
#define P 1
#define IPSTART 2
#define IPEND 3

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
  if(command_timer < 20){
    R_forward(command_speed);
    L_forward(command_speed);
  }else{
    state = STOP;
    R_stop();
    L_stop();
  }
  
}


void navigate(int time){
    command_timer = 0;
    
    switch(state){ //intercept and line follow not needed for HW 8 and Project 8
    case WAIT:
      state_count =0;
      state = FIRST_FORWARD_LEG;
      break;
    case FIRST_FORWARD_LEG:
      if(state_count > 800){
        R_stop();L_stop();
        state = FIRST_TURN_RIGHT;
        state_count =0;
      }else{
        R_forward(2000);L_forward(1975);
      }
      break;
    case FIRST_TURN_RIGHT:
      if(state_count > 215){
        R_stop();L_stop();
        state = SECOND_FORWARD_LEG;
        state_count =0;
      }else{
        R_reverse(2000);L_forward(2000);
      }
      break;
    case SECOND_FORWARD_LEG:
      if(state_count > 1400){
        R_stop();L_stop();
        state = SECOND_TURN_RIGHT;
        state_count =0;
      }else{
        R_forward(2000);L_forward(1975);
      }
      break;
    case SECOND_TURN_RIGHT:
      if(state_count > 215){
        R_stop();L_stop();
        state = WHITE_DETECT;
        state_count =0;
      }else{
        R_reverse(2000);L_forward(2000);
      }
      break;
    case WHITE_DETECT:
      if((return_vleft_average() < WHITE_VALUE_MAX) && (return_vright_average() < WHITE_VALUE_MAX)){
        state = LINE_DETECT;
      }else{
        R_forward(2000);L_forward(1975);
      }
      
      break;
    case LINE_DETECT:
      if((return_vleft_average() > BLACK_LINE) && (return_vright_average() > BLACK_LINE)){
        R_stop();L_stop();
        state = REVERSE_STATE;
        state_count =0;
      }else{
        R_forward(2000);L_forward(1975);
      }
      break;
    case REVERSE_STATE:
       if(state_count > 60){
        R_stop();L_stop();
        state = SPIN_BLACK_LINE;
        state_count =0;
      }else{
        R_reverse(2000);L_reverse(2000);
      }
      break;
      
    case SPIN_BLACK_LINE:
      if(state_count > 220){
        R_stop();L_stop();
        state_count =0;
        state = NAVIGATION;
      }else{
        L_reverse(2000);R_forward(2000);
      }
      break;
    case NAVIGATION:
      if(state_count < 400){
        state_count =0;
        state = CIRCLE;
      }else{
        line_nav(1800);
      }
      break;
      
    case CIRCLE:
        line_nav(1800);
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
    command_timer = 0;
    next_command_time = 0;
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
    command_timer = 0;
    next_command_time = 0;
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
    command_timer = 0;
    next_command_time = 0;
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
    command_timer = 0;
    next_command_time = 0;
    str_cpy(curr_screen4_line2, next_screen4_line2);
  }
}

int str_to_int(char*value){//Handles 3 values only
  int intval=0;
  intval+= (value[2]-0x30);
  intval+= (value[1]-0x30)*10;
  intval+= (value[0]-0x30)*100;
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
      IP_index = 0;
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
  char counter=0;
  index=0;
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
  char placement = 0;
  while(IP[index] != ':' && index < 16){
    IP_lower[placement++] = IP[index];
    index++;
  }
  IP_lower[placement] = NULL_CHAR;
  return &IP_lower[0];
}