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


#define SCREEN_ONE 0
#define SCREEN_TWO 1
#define SCREEN_THREE 2
#define SCREEN_FOUR 3
//Globals
  extern char adc_char[6];
  char clock[7];
  int screen_clock;
  char clock_enable;
  
  char update_menu;
  char menu_screen;
  char null[] = "";
  
  extern char state;
  
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
//Screen 3
  char* screen3_line1 = "Screen 3";
  char* screen3_line2 = null;
  char* screen3_line3 = null;
  char* screen3_line4 = null;
//Screen 4
  char* screen4_line1 = "Screen 4";
  char* screen4_line2 = null;
  char* screen4_line3 = null;
  char* screen4_line4 = null;

//------------------------------------------------------------------------------
//Handles the menu based off of the thumbwheel                                  menu
//------------------------------------------------------------------------------
void menu(void){
  if(update_menu){
    //Updating Values once ecery 200ms
    display_clock();
    display_state();
    
    lcd_line4("        ");
    display_averages();
    if((return_vleft_average() > BLACK_LINE) && (return_vright_average() > BLACK_LINE)){
      screen1_line4 = "Detected";
    }    
    
      
    
    if(menu_screen != return_vthumb_average() >> 8){
      menu_screen = return_vthumb_average() >> 8;
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
    case SCREEN_THREE:
        lcd_line1(screen3_line1);
        lcd_line2(screen3_line2);
        lcd_line3(screen3_line3);
        lcd_line4(screen3_line4);
        break;
    case SCREEN_FOUR:
        lcd_line1(screen4_line1);
        lcd_line2(screen4_line2);
        lcd_line3(screen4_line3);
        lcd_line4(screen4_line4);
        break;
    default: lcd_line1("Menu Err"); break;
    }

  update_menu = 0;
  }

}


#define WAIT 1
#define DRIVE 2
#define SPIN_BLACK_LINE 3
#define REVERSE_STATE 5
#define NAVIGATION 6
#define SPIN_CENTER 7
#define DRIVE_CENTER 8

void display_state(void){
  switch(state){
  case WAIT:
    screen4_line1 = "Stopped";
    break;
  case DRIVE:
    screen4_line1 = "Intercep";
    break;
  case SPIN_BLACK_LINE:
    screen4_line1 = "Turning";
    break;
  case NAVIGATION:
    screen4_line1 = "Circling";
    break;
  case SPIN_CENTER:
    screen4_line1 = "Turning";
    break;
  default:break;
  }
}



//------------------------------------------------------------------------------
// CLOCK functions all the functions that control the clock                     display_clock, set_clock, get_clock_time, disable_clock, enable_clock
//------------------------------------------------------------------------------
void display_clock(){
  if(screen_clock > MAX_SCREEN_CLOCK_VALUE)
    screen_clock = 0;
  
  if(clock_enable){
    HEXtoBCD(screen_clock);
    str_cpy(clock, adc_char);
    clock[6] = clock[5];
    clock[5] = clock[4];
    clock[4] = '.';
  }
  screen4_line4 = clock;
}

void set_clock(int setting){
  screen_clock = setting;
}

int get_clock_time(void){
  return screen_clock;
}

void disable_clock(void){
  clock_enable = FALSE;
}

void enable_clock(void){
  clock_enable = TRUE;
}

//------------------------------------------------------------------------------
// String copy, copies one string to another                                    str_cpy
//------------------------------------------------------------------------------
void str_cpy(char* str_copy, char* str_orig){
  int i = RESET_STATE;
  while(str_orig[i]!= '\0'){
    str_copy[i] = str_orig[i];
    i++;
  }
  str_copy[i] = '\0';
}
