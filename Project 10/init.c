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

//Globals
  extern char display_line[4][MAX_LCD_LENGTH];
  extern char *display[4];
  extern volatile unsigned char update_display;


//------------------------------------------------------------------------------
// initialilzes the display                                                     Init_Conditions      
// Passed:      None
// Returned:    None
//------------------------------------------------------------------------------
void Init_Conditions(void){
  int i;

  for(i=BEGINNING;i<MAX_LCD_LENGTH;i++){
    display_line[LINE1][i] = RESET_STATE;
    display_line[LINE2][i] = RESET_STATE;
    display_line[LINE3][i] = RESET_STATE;
    display_line[LINE4][i] = RESET_STATE;
  }
  display_line[LINE1][MAX_LCD_LENGTH-1] = ZERO;
  display_line[LINE2][MAX_LCD_LENGTH-1] = ZERO;
  display_line[LINE3][MAX_LCD_LENGTH-1] = ZERO;
  display_line[LINE4][MAX_LCD_LENGTH-1] = ZERO;

  display[LINE1] = &display_line[LINE1][FIRST_CHAR];
  display[LINE2] = &display_line[LINE2][FIRST_CHAR];
  display[LINE3] = &display_line[LINE3][FIRST_CHAR];
  display[LINE4] = &display_line[LINE4][FIRST_CHAR];
  update_display = FALSE;
  // Interrupts are disabled by default, enable them.
  enable_interrupts();
}

//------------------------------------------------------------------------------
// initialilzes the ports                                                       Init_Ports      
// Passed:      None
// Returned:    None
//------------------------------------------------------------------------------
void Init_Ports(void){
  Init_Port1();
  Init_Port2();
  Init_Port3();
  Init_Port4();
  Init_Port5();
  Init_Port6();
}


