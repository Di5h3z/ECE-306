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
  extern char station_counter;
  char screen4_line1_hold[11];
  
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
  char ip_toggle;
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
    
    clear_lcd();                //clears all characters        
    assign_static_strings();    //display static strings

    iot_course_display();       //display the IOT course information
    display_clock();            //display the clock
    display_averages();         //calculate sensor values
    
                                //get IP and display
    screen3_line2 = get_upper_IP();
    screen3_line3 = get_lower_IP();
    
                                
    ip_toggle++;
    if(ip_toggle < 5){
      screen4_line3 = screen3_line2;
    }else if(ip_toggle >= 5 && ip_toggle < 10){
      screen4_line3 = screen3_line3;
    }else{
      ip_toggle = 0;
    }
    
    //these handle selection functionality
    blink_selected();           //blinking to indicate line
    handle_select();            //handles a button trigger for chosen line
    put_screen();               //puts the string to the dispaly
    
    

  update_menu = FALSE;
  }

}

void iot_course_display(void){
  
  switch(state){
  case WAIT:
    screen4_line1_hold[0] = station_counter + 0x30;
    screen4_line1 = screen4_line1_hold;
    break;
  case FIRST_FORWARD_LEG:
    screen4_line1 = "BLstart";
    break;
  case FIRST_TURN_RIGHT:
  case SECOND_FORWARD_LEG:
  case SECOND_TURN_RIGHT:
  case WHITE_DETECT:
    screen4_line1 = "Intercept";
    break;
  case NAVIGATION:
    screen4_line1 = "BL Travel";
    break;
  case CIRCLE: 
    screen4_line1 = "BL Circle";
    break;
  case BL_EXIT:
    screen4_line1 = "BL Exit";
    break;
  case STOP:
    screen4_line1 = "Stop";
    break;
  default:break;
  }
}

void assign_static_strings(void){
  screen2_line1 = "TGL Baud";
  screen2_line3 = "Send MSG";
  screen3_line1 = "ncsu";
  
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
            //screen3_line1 = clear_line;
            //screen4_line1 = clear_line;
          break;
        case LINE_TWO:
            //screen1_line2 = clear_line;
            //screen2_line2 = clear_line;
            //screen3_line2 = clear_line;
            //screen4_line2 = clear_line;
          break;
        case LINE_THREE:
            //screen1_line3 = clear_line;
            screen2_line3 = clear_line;
            //screen3_line3 = clear_line;
            //screen4_line3 = clear_line;
          break;
        case LINE_FOUR:
            //screen1_line4 = clear_line;
            screen2_line4 = clear_line;
            //screen3_line4 = clear_line;
            //screen4_line4 = clear_line;
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
          lcd_4line();
          lcd_line1(screen1_line1);
          lcd_line2(screen1_line2);
          lcd_line3(screen1_line3);
          lcd_line4(screen1_line4);
          break;
      case SCREEN_TWO:
          lcd_4line();
          lcd_line1(screen2_line1);
          lcd_line2(screen2_line2);
          lcd_line3(screen2_line3);
          lcd_line4(screen2_line4);
          break;
      case SCREEN_THREE:
          lcd_4line();
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



//------------------------------------------------------------------------------
// CLOCK functions all the functions that control the clock                     display_clock, set_clock, get_clock_time, disable_clock, enable_clock
//------------------------------------------------------------------------------
void display_clock(void){
  if(screen_clock > MAX_SCREEN_CLOCK_VALUE)
    screen_clock = 0;
  
  if(clock_enable){
    HEXtoBCD(screen_clock);
    str_cpy(clock, adc_char);

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
