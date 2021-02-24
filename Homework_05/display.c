//------------------------------------------------------------------------------
//Description:  This file contains the functions and variables to update the 
//              LCD display
//
//By:           Nathan Carels
//Date:         2/21/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------

#include "macros.h"
#include "msp430.h"
#include "functions.h"


extern char port3_select;
//changes the lcd to reflect the state of port 3 pin 4
void smclk_gpio_display(char select){
  
  switch(select){
     case USE_GPIO: break;
     case USE_SMCLK: break;
  default: port3_select = USE_GPIO; break;
  }

}