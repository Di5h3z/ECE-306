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
char* str_crop(char* str_orig, char start);

#define HOME 0
#define RESISTOR 1
#define SHAPES 2
#define SONG 3

#define LINE_ONE 0
#define LINE_TWO 1
#define LINE_THREE 2
#define LINE_FOUR 3

//Globals

//base menu varaibles  
  char update_menu;
  char menu_screen;
  char null[] = " ";
  
//selection varaibles
   char select_fired;
   char screen_state;
   char line_number;
   char blink_toggle;
   
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

  
  int thumb_prev;
  int wheel_div;
  char scroll=0;
  int thumb_wheel;
  char prev_shape;
  char next_shape;
  
  
  char song[] = "We're the Red and White from State And we know we are the best. A hand behind our back, We can take on all the rest. Come over the hill, Carolina. Devils and Deacs stand in line. The Red and White from N.C. State. Go State!";
  char numbers[10][2] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};
  char resistors[10][10] = {"Black", "Brown", "Red", "Orange", "Yellow", "Green", "Blue", "Violet", "Gray", "White"};
  char shapes[10][10] = {"Circle", "Square", "Triangle", "Octagon", "Pentagon", "Hexagon", "Cube", "Oval", "Sphere", "Cylinder"};

  
  
//------------------------------------------------------------------------------
//main menu
//------------------------------------------------------------------------------ 
  void main_menu(void){
    
      if(scroll > 2)
        scroll = 0;
      if(select_fired){
        screen_state = scroll+1;
        scroll = 0;
        select_fired = FALSE;
      }
      
      lcd_line1("Resistors");
      lcd_line2("Shapes");
      lcd_line3("Song");
      
      blink_toggle ^= TRUE;
      if(blink_toggle)
        switch(scroll){
        case LINE_ONE:
          lcd_line1("          ");
          break;
          
        case LINE_TWO:
          lcd_line2("          ");
          break;
        
        case LINE_THREE:
          lcd_line3("          ");
          break;
        
        }

  }  
  
//------------------------------------------------------------------------------
//resistor menu
//------------------------------------------------------------------------------  
  void resistor_menu(void){
  if(scroll > 9)
        scroll = 0;
      
      lcd_line1("Color");
      lcd_line2(resistors[scroll]);
      lcd_line3("Value");
      lcd_line4(numbers[scroll]);

  }
  
//------------------------------------------------------------------------------
//shape menu
//------------------------------------------------------------------------------ 
  void shape_menu(void){
      if(scroll > 9 || scroll <= 0){
        scroll = 0;
        prev_shape = 9;
        next_shape = 1;
      }else if(scroll == 9){
        prev_shape = 8;
        next_shape = 0;
      }else{
        prev_shape = scroll-1;
        next_shape = scroll +1;
      }
      
      lcd_line1(shapes[prev_shape]);
      lcd_line2(shapes[scroll]);
      lcd_line3(shapes[next_shape]);
      lcd_BIG_mid(); //this might have to be a one time thing
  
  }
  
//------------------------------------------------------------------------------
//song menu
//------------------------------------------------------------------------------ 
  void song_menu(void){
  if(thumb_wheel - 100 > thumb_prev){
        scroll += 4;
      }
      
      
      if(scroll > 223)
        scroll = 0;
      
      if(scroll&1){
        lcd_line1("RED");
        lcd_line3("WHITE");
      }else{
        lcd_line1("WHITE");
        lcd_line3("RED");
      }
      
      lcd_line2(str_crop(song, scroll));
  
  }
  
  
//------------------------------------------------------------------------------
//Handles the menu based off of the thumbwheel                                  menu
//------------------------------------------------------------------------------  
void menu(void){
  if(update_menu){
    //Updating Values once ecery 200ms
    thumb_wheel = return_vthumb_average();
   
    
    
    if(thumb_wheel - 100 > thumb_prev){
      scroll++;
      clear_lcd();
    }
    
    switch(screen_state){
    case HOME:
      main_menu();
      break;
    case RESISTOR:
      resistor_menu();
      break;
    case SHAPES:
      shape_menu();
      break;
    case SONG:
      song_menu();
      
      break;
    default:break;
    }
    
    
    
    thumb_prev = thumb_wheel;
    update_menu = 0;
  }

}


char str_crop_ret[11];

char* str_crop(char* str_orig, char start){
  char j = 0;
  for(char i = start; i< start+10;i++){
    str_crop_ret[j] = str_orig[i];
    j++;
  }
  str_crop_ret[10] = '\0';
  return str_crop_ret;
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
