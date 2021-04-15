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


void right_turn(int time){
  if(command_timer < time){
    R_reverse(2000);
    L_forward(2000);
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
    R_forward(2000);
    L_reverse(2000);
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
    R_reverse(2000);
    L_reverse(2000);
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
    R_forward(2000);
    L_forward(2000);
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