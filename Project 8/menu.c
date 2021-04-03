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

#define LINE_ONE 0
#define LINE_TWO 1
#define LINE_THREE 2
#define LINE_FOUR 3

//Globals
//externals refrenced in menu.c
  extern char state;

//number display varaibles
  extern char adc_char[6];
  
//baud varaibles 
  char baud_state;
  
  
//clock variables
  char clock[7];
  int screen_clock;
  char clock_enable;
  
//base menu varaibles  
  char update_menu;
  char menu_screen;
  char null[] = " ";
  
//selection varaibles
  char select_fired;
  char toggle_selected;
  char line_selected;
  char clear_line[] = "          ";
  
//Screen 1
  char* screen1_line1 = "Screen 1";
  char* screen1_line2 = null;
  char* screen1_line3 = null;
  char* screen1_line4 = null;
//Screen 2
  char* screen2_line1 = "Screen 2";
  char* screen2_line2 = "115,200";
  char* screen2_line3 = null;
  char* screen2_line4 = null;
//Screen 3
  char* screen3_line1 = "Screen 3";
  char* screen3_line2 = null;
  char* screen3_line3 = null;
  char* screen3_line4 = null;
//Screen 4
  char* screen4_line1 = "States+Clk";
  char* screen4_line2 = null;
  char* screen4_line3 = null;
  char* screen4_line4 = null;

//------------------------------------------------------------------------------
//Handles the menu based off of the thumbwheel                                  menu
//------------------------------------------------------------------------------
void menu(void){
  if(update_menu){
    //Updating Values once ecery 200ms
    assign_static_strings();
    display_clock();
    display_state();
    
    //lcd_line4("        ");
    display_averages();
      
    screen2_line4 = USB_rx();
    
    
    //these handle selection functionality
    blink_selected();
    handle_select();
    put_screen();               //puts the string to the dispaly

    

  update_menu = 0;
  }

}
void assign_static_strings(void){
  screen4_line1 = "States+Clk";
  screen2_line1 = "TGL Baud";
  screen2_line3 = "Send MSG";
  
}


void blink_selected(void){
toggle_selected ^= TRUE;            //Toggles the variable
    if(toggle_selected){
                                        //makes the line to blink when selected
                                        //commented out lines mean the screen should not have selection indicators
      switch(line_selected){
        case LINE_ONE:
            //screen1_line1 = clear_line;
            screen2_line1 = clear_line;
            screen3_line1 = clear_line;
            screen4_line1 = clear_line;
          break;
        case LINE_TWO:
            //screen1_line2 = clear_line;
            //screen2_line2 = clear_line;
            screen3_line2 = clear_line;
            screen4_line2 = clear_line;
          break;
        case LINE_THREE:
            //screen1_line3 = clear_line;
            screen2_line3 = clear_line;
            screen3_line3 = clear_line;
            screen4_line3 = clear_line;
          break;
        case LINE_FOUR:
            //screen1_line4 = clear_line;
            screen2_line4 = clear_line;
            screen3_line4 = clear_line;
            screen4_line4 = clear_line;
          break;
      }
    }

}

void handle_select(void){
//this selects the current line and screen and will preform the action inside when the select is fired
    if(select_fired){
      switch(menu_screen){
        case SCREEN_ONE:
            switch(line_selected){
              case LINE_ONE:
                
                break;
              case LINE_TWO:
                  
                break;
              case LINE_THREE:
                  
                break;
              case LINE_FOUR:
                  
                break;
            }
            break;
        case SCREEN_TWO:
            switch(line_selected){
              case LINE_ONE:
                baud_state ^= TRUE;
                
                if(baud_state){
                  screen2_line2 = "460,800";
                  UCA0BRW = 17 ; // 460,800 baud
                  UCA0MCTLW = 0x4A00 ;
                  UCA1BRW = 17 ; // 460,800 baud
                  UCA1MCTLW = 0x4A00 ;
                }else{
                  screen2_line2 = "115,200"; 
                  UCA0BRW = 4 ; // 115,200 baud
                  UCA0MCTLW = 0x5551 ;
                  UCA1BRW = 4 ; // 115,200 baud
                  UCA1MCTLW = 0x5551 ;   
                }
                
                
                break;
              case LINE_TWO:
                  
                break;
              case LINE_THREE:
                USB_tx("NCSU  #1");
                break;
              case LINE_FOUR:
                USB_tx("        ");
                break;
            }
            break;
        case SCREEN_THREE:
            switch(line_selected){
              case LINE_ONE:
                
                break;
              case LINE_TWO:
                  
                break;
              case LINE_THREE:
                  
                break;
              case LINE_FOUR:
                  
                break;
            }
            break;
        case SCREEN_FOUR:
            switch(line_selected){
              case LINE_ONE:
                
                break;
              case LINE_TWO:
                  
                break;
              case LINE_THREE:
                  
                break;
              case LINE_FOUR:
                  
                break;
            }
            break;
      }
      select_fired = FALSE;
    }

}

void put_screen(void){
  if(menu_screen != return_vthumb_average() >> 8){
      menu_screen = return_vthumb_average() >> 8;
      clear_lcd();
    }
    //Putting string to the correct display
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
