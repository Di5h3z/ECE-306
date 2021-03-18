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
#define WHITE 100
#define REFRENCE 0
#define SAMPLE_TIME 32 //ms in between samples

#define KP 1
#define KI 1
#define KD 1

char pid_enable;
int error;
int sum_error;
int delta_error;
int prev_error;
int control_sig;

extern char adc_char[5];


char sum_error_str[11];
char delta_error_str[11];
char error_str[11];
char control_sig_str[11];


void line_nav(int speed){
  if(pid_enable){
    int control = pid_process();
    
    R_forward(speed - control);
    L_forward(speed + control);
    pid_enable = FALSE;
  }
}




int pid_process(void){

   int left_average = VLEFT_AVERAGE;
   int right_average = VRIGHT_AVERAGE;
   error = REFRENCE - (left_average - right_average); //system trys to settle towards 0 + turn left - turn right.. maybe

   sum_error += error;

   delta_error = (error-prev_error);

   prev_error = error;
   
   control_sig = (KP*error + KI*sum_error + KD*delta_error);

   return control_sig;
  
}

void pid_display(void){

   HEXtoBCD(error);
   str_cpy(error_str, adc_char);
   screen4_line1 = error_str;
   
   HEXtoBCD(sum_error);
   str_cpy(sum_error_str, adc_char);
   screen4_line2 = sum_error_str;
   
   HEXtoBCD(delta_error);
   str_cpy(delta_error_str, adc_char);
   screen4_line3 = delta_error_str;
   
   HEXtoBCD(control_sig);
   str_cpy(control_sig_str, adc_char);
   screen4_line4 = control_sig_str;
   
}
