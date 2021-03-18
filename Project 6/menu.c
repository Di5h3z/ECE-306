//------------------------------------------------------------------------------
//Description:  This file contains the functions for reading in switch input
//
//By:           Nathan Carels
//Date:         3/14/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------
#include "functions.h"
#include "msp430.h"
#include "macros.h"


#define SCREEN_ONE 1
#define SCREEN_TWO 0

//Globals
  char update_menu;
  char menu_screen;
  char null[] = "";
//Screen 1
  char* screen1_line1 = null;
  char* screen1_line2 = null;
  char* screen1_line3 = null;
  char* screen1_line4 = null;
//Screen 2
  char* screen2_line1 = "Screen 2";
  char* screen2_line2 = null;
  char* screen2_line3 = null;
  char* screen2_line4 = null;

//------------------------------------------------------------------------------
//Handles the menu based off of the thumbwheel                                  menu
//------------------------------------------------------------------------------
void menu(void){
  if(update_menu){
    //Updating Values once a second
    if(P2OUT & IR_LED)
      screen2_line2 = "Emmiter 1";
    else
      screen2_line2 = "Emmiter 0";
    
    lcd_line4("        ");
    display_averages();
    if((return_vleft_average() > BLACK_LINE) && (return_vright_average() > BLACK_LINE)){
      screen1_line4 = "Detected";
    }    
    
      
    
    if(menu_screen != return_vthumb_average() >> 11){
      menu_screen = return_vthumb_average() >> 11;
      clear_lcd();
    }
      
    //Putting string to display
    switch(menu_screen){
    case SCREEN_ONE:
        lcd_line1(screen1_line1);
        lcd_line2(screen1_line2);
        lcd_line3(screen1_line3);
        lcd_line4(screen1_line4);
        break;
    case SCREEN_TWO:
        lcd_line1(screen2_line1);
        lcd_line2(screen2_line2);
        lcd_line3(screen2_line3);
        lcd_line4(screen2_line4);
        break;
    default: lcd_line1("Menu Err"); break;
    }

  update_menu = 0;
  }

}

void str_cpy(char* str_copy, char* str_orig){
  int i = RESET_STATE;
  while(str_orig[i]!= '\0'){
    str_copy[i] = str_orig[i];
    i++;
  }
  str_copy[i] = '\0';
}
