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

//Globals
  //externals refrenced in menu.c
  extern char state;
  extern char station_counter;
  char screen4_line1_hold[MAX_LCD_LENGTH];
  
  //number display varaibles
  extern char adc_char[BCD_MAX_LEN];
  
  //baud varaibles 
  char baud_state;
  
  //clock variables
  char clock[CLOCK_LENGTH];
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
// Passed:      None
// Returned:    None
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
    
                                
    ip_toggle++;                //Toggles which part of the IP is displayed for compactness
    if(ip_toggle < IP_TOGGLE_UP_COUNT){
      screen4_line3 = screen3_line2;
    }else if(ip_toggle >= IP_TOGGLE_UP_COUNT && ip_toggle < IP_TOGGLE_LOW_COUNT){
      screen4_line3 = screen3_line3;
    }else{
      ip_toggle = RESET;
    }
    
    //these handle selection functionality
    blink_selected();           //blinking to indicate line
    handle_select();            //handles a button trigger for chosen line
    put_screen();               //puts the string to the dispaly
    
    

  update_menu = FALSE;
  }

}


//------------------------------------------------------------------------------
//Displays the state of the IOT and line navigation                             iot_course_display
// Passed:      None
// Returned:    None
//------------------------------------------------------------------------------
void iot_course_display(void){
  
  switch(state){        //state machine to display state of navigation and intercrption
  case WAIT:
    screen4_line1_hold[FIRST_CHAR] = station_counter + ASCII_TO_DEC;
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

//------------------------------------------------------------------------------
//Displays the strings that are always present on a menu                        assign_static_strings
// Passed:      None
// Returned:    None
//------------------------------------------------------------------------------
void assign_static_strings(void){
  screen2_line1 = "TGL Baud";
  screen2_line3 = "Send MSG";
  screen3_line1 = "ncsu";
  
}

//------------------------------------------------------------------------------
//Blinks lines that are enabled as an indicator                                 blink_selected
// Passed:      None
// Returned:    None
//------------------------------------------------------------------------------
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


//------------------------------------------------------------------------------
//preforms a function when the selct button is fired                            handle_select
// Passed:      None
// Returned:    None
//------------------------------------------------------------------------------
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
                  UCA0BRW = S460800B_BRW ; // 460,800 baud
                  UCA0MCTLW = S460800B_CTLW ;
                  UCA1BRW = S460800B_BRW ; // 460,800 baud
                  UCA1MCTLW = S460800B_CTLW ;
                }else{
                  screen2_line2 = "115,200"; 
                  UCA0BRW = S115200B_BRW ; // 115,200 baud
                  UCA0MCTLW = S115200B_CTLW ;
                  UCA1BRW = S115200B_BRW ; // 115,200 baud
                  UCA1MCTLW = S115200B_CTLW ;   
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

//------------------------------------------------------------------------------
//Puts the correct strings to the screen for a given menu                       put_screen
// Passed:      None
// Returned:    None
//------------------------------------------------------------------------------
void put_screen(void){
  if(menu_screen != return_vthumb_average() >> BITSHIFT_8){
      menu_screen = return_vthumb_average() >> BITSHIFT_8;
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






//------------------------------------------------------------------------------
// CLOCK functions all the functions that control the clock                     display_clock, set_clock, get_clock_time, disable_clock, enable_clock
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Puts the clock value to the display                                          display_clock
// Passed:      None    
// Returned:    None
//------------------------------------------------------------------------------
void display_clock(void){
  if(screen_clock > MAX_SCREEN_CLOCK_VALUE)
    screen_clock = RESET;
  
  if(clock_enable){
    HEXtoBCD(screen_clock);
    str_cpy(clock, adc_char);

  }
  screen4_line4 = clock;
}

//------------------------------------------------------------------------------
// changes the clock value                                                      set_clock
// Passed:      setting(value for clock to start at)    
// Returned:    None
//------------------------------------------------------------------------------
void set_clock(int setting){
  screen_clock = setting;
}

//------------------------------------------------------------------------------
// changes the clock value                                                      get_clock_time
// Passed:      None    
// Returned:    screen_clock(numerical value of the clock)
//------------------------------------------------------------------------------
int get_clock_time(void){
  return screen_clock;
}

//------------------------------------------------------------------------------
// disables the clock                                                           disable_clock
// Passed:      None    
// Returned:    screen_clock(numerical value of the clock)
//------------------------------------------------------------------------------
void disable_clock(void){
  clock_enable = FALSE;
}

//------------------------------------------------------------------------------
// enalbes the clock                                                            enable_clock
// Passed:      None    
// Returned:    screen_clock(numerical value of the clock)
//------------------------------------------------------------------------------
void enable_clock(void){
  clock_enable = TRUE;
}

//------------------------------------------------------------------------------
// String copy, copies one string to another                                    str_cpy
// Passed:      None
// Returned:    None
//------------------------------------------------------------------------------
void str_cpy(char* str_copy, char* str_orig){
  int i = RESET_STATE;
  while(str_orig[i]!= NULL_CHAR){
    str_copy[i] = str_orig[i];
    i++;
  }
  str_copy[i] = NULL_CHAR;
}
