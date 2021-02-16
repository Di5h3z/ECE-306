//------------------------------------------------------------------------------
//
//Description:  This file contains initialization code for first time set up 
//
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------

#include "macros.h"
#include  "msp430.h"
#include  "functions.h"

//globals declared so they can be seen within init
extern char display_line[4][11];
extern char *display[4];
extern volatile unsigned char update_display;


//initilizes the display
void Init_Conditions(void){
//------------------------------------------------------------------------------
  int i;

  for(i=0;i<11;i++){
    display_line[0][i] = RESET_STATE;
    display_line[1][i] = RESET_STATE;
    display_line[2][i] = RESET_STATE;
    display_line[3][i] = RESET_STATE;
  }
  display_line[0][10] = 0;
  display_line[1][10] = 0;
  display_line[2][10] = 0;
  display_line[3][10] = 0;

  display[0] = &display_line[0][0];
  display[1] = &display_line[1][0];
  display[2] = &display_line[2][0];
  display[3] = &display_line[3][0];
  update_display = 0;
// Interrupts are disabled by default, enable them.
  enable_interrupts();
//------------------------------------------------------------------------------
}

//initiializes the ports 1-6
void Init_Ports(void){
  Init_Port1();
  Init_Port2();
  Init_Port3();
  Init_Port4();
  Init_Port5();
  Init_Port6();
}