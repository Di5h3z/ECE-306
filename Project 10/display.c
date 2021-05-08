//------------------------------------------------------------------------------
//Description:  This file contains the functions and variables that control the
//              display easily
//
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------
#include "functions.h"
#include "msp430.h"
#include "macros.h"

//------------------------------------------------------------------------------
//clear the lcd                                                                 clear_lcd                        
// Passed:      None
// Returned:    None
//------------------------------------------------------------------------------
void clear_lcd(void){
  char display_clear[MAX_LCD_LENGTH] = "          ";
  for(int i=LINE1;i<NUM_LINES;i++)
    update_string(display_clear,i);
  Display_Update(CHAR_0,CHAR_0,CHAR_0,CHAR_0);
}

//------------------------------------------------------------------------------
//displays the string in line to LCD line 1                                     lcd_line1
// Passed:      line, pointer to display string
// Returned:    None
//------------------------------------------------------------------------------
void lcd_line1(char* line){
  update_string(line,LINE1);
  Display_Update(CHAR_0,CHAR_0,CHAR_0,CHAR_0);
}

//------------------------------------------------------------------------------
//displays the string in line to LCD line 2                                     lcd_line2
// Passed:      line, pointer to display string
// Returned:    None
//------------------------------------------------------------------------------
void lcd_line2(char* line){
  update_string(line,LINE2);
  Display_Update(CHAR_0,CHAR_0,CHAR_0,CHAR_0);
}

//------------------------------------------------------------------------------
//displays the string in line to LCD line 3                                     lcd_line3
// Passed:      line, pointer to display string
// Returned:    None
//------------------------------------------------------------------------------
void lcd_line3(char* line){
  update_string(line,LINE3);
  Display_Update(CHAR_0,CHAR_0,CHAR_0,CHAR_0);
}

//------------------------------------------------------------------------------
//displays the string in line to LCD line 4                                     lcd_line4
// Passed:      line, pointer to display string
// Returned:    None
//------------------------------------------------------------------------------
void lcd_line4(char* line){
  update_string(line,LINE4);
  Display_Update(CHAR_0,CHAR_0,CHAR_0,CHAR_0);
}
