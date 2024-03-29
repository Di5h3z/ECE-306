//------------------------------------------------------------------------------
//
//Description:  This file contains all the macros used throughout the project
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------



//------------------------------------------------------------------------------
//General macros
//------------------------------------------------------------------------------
#define TRUE 1
#define FALSE 0
#define ALWAYS (1)
#define RESET_STATE (0)
#define NULL 0x00
#define NULL_PTR ((void*)0)
#define ZERO 0
#define RESET 0 

#define CARRIAGE_RETURN_CHAR '\r'
#define NULL_CHAR '\0'
#define NEWLINE_CHAR '\n'

#define P_CHAR  'P'
#define I_CHAR  'I'
#define COLON_CHAR ':'
#define PERIOD_CHAR '.'

//------------------------------------------------------------------------------
//ADC macros
//------------------------------------------------------------------------------
#define NUM_READINGS 8
#define BCD_MAX_LEN 6
#define EIGHT_COUNT_MASK 0x07
#define BITSHIFT_DIVIDE_8 3
                                        //macros to make gettings the sensors averages easy
#define VLEFT_AVERAGE return_vleft_average()    
#define VRIGHT_AVERAGE return_vright_average()

#define VRIGHT_CALIBRATION 3            //value to get the senors close

#define MAX_SUB_THOUSAND 999
#define MAX_SUB_HUNDRED 99
#define MAX_SUB_TEN 9
#define THOUSAND 1000
#define HUNDRED 100
#define TEN 10

#define PLUS_CHAR '+'
#define MINUS_CHAR '-'
#define FIRST_CHAR 0 
#define SECOND_CHAR 1
#define THIRD_CHAR 2
#define FOURTH_CHAR 3
#define FITH_CHAR 4
#define SIXTH_CHAR 5

#define L_DETECT_CASE 0x00
#define R_DETECT_CASE 0x01
#define T_DETECT_CASE 0x02

//------------------------------------------------------------------------------
//DAC macros
//------------------------------------------------------------------------------
#define DAC_ERROR_TIMEOUT 4             //seconds before setteling fails
#define MIN_DAC 0x0fff                  //set the smalles DAC V out
#define DAC_6V 1200                     //value to get to 6 volts

//------------------------------------------------------------------------------
//Display
//------------------------------------------------------------------------------
#define LINE1 0                         
#define LINE2 1
#define LINE3 2
#define LINE4 3
#define NUM_LINES 4

#define LCD_OFF P6OUT &= ~LCD_BACKLITE  //controls the backlight
#define LCD_ON P6OUT |= LCD_BACKLITE
#define LCD_TOGGLE P6OUT ^= LCD_BACKLITE

#define MAX_LCD_LENGTH 11
#define CHAR_0 0


//------------------------------------------------------------------------------
//Drive macros
//------------------------------------------------------------------------------
#define FORWARD 1
#define REVERSE 0
#define CHANGE_COUNT 10                 //the 50 ms between forward and reverse
#define MAX_SPEED 10000                 

#define WHEEL_PERIOD 10000              //max pwm period
#define WHEEL_OFF 0                     //0 is min turn the wheel off

#define RIGHT_FORWARD_SPEED (TB3CCR1)   //PWM CCR's 
#define LEFT_FORWARD_SPEED (TB3CCR2)
#define RIGHT_REVERSE_SPEED (TB3CCR3)
#define LEFT_REVERSE_SPEED (TB3CCR4)


//------------------------------------------------------------------------------
//IOT
//------------------------------------------------------------------------------
#define WAIT 1                          //Line intercept and nav state machine
#define FIRST_FORWARD_LEG 10
#define FIRST_TURN_RIGHT 11
#define SECOND_FORWARD_LEG 12
#define SECOND_TURN_RIGHT 13
#define WHITE_DETECT 14
#define LINE_DETECT 2
#define SPIN_BLACK_LINE 3
#define REVERSE_STATE 5
#define NAVIGATION 6
#define CIRCLE 15
#define BL_EXIT 7
#define STOP 8

#define I 0                             //IP detection state machine
#define P 1
#define IPSTART 2
#define IPEND 3

#define EXIT_DRIVE_TIME 30
#define EXIT_DRIVE_ADJUST 300

#define INTERCEPT_ARC_LSPEED 2500
#define INTERCEPT_ARC_RSPEDD 1700

#define NAV_SPEED 2050

#define MOMENTUM_BREAK_TIME 60
#define SPIN_TIME 250

#define LINE_NAV_TIME 2200

#define ASCII_TO_DEC 0x30

#define PIN_LENGTH 5

#define NUM_PERIODS_DETECTION 2

//------------------------------------------------------------------------------
//Main macros
//------------------------------------------------------------------------------   
#define FOWARD_COMMAND_CHAR 'F'
#define REVERSE_COMMAND_CHAR 'B'
#define LEFT_COMMAND_CHAR 'L'
#define RIGHT_COMMAND_CHAR 'R'
#define SPEED_COMMAND_CHAR 'S'
#define INTNAV_COMMAND_CHAR 'N'
#define EXIT_COMMAND_CHAR 'E'
#define INC_COMMAND_CHAR 'I' 
#define CLOCK_COMMAND_CHAR 'C'


#define SPEED_MUTIPLIER 200
#define MAX_SPEED_PRIOR_MULT 50
#define SLOW_SPEED 2000

#define TWO_CIRCLE_TIME 1010
#define RX 2
#define TX 3

#define BLACK_LINE 500                  //used to find the black line is made defunct below

//------------------------------------------------------------------------------
//Menu
//------------------------------------------------------------------------------
#define SCREEN_ONE 0
#define SCREEN_TWO 1
#define SCREEN_THREE 2
#define SCREEN_FOUR 3

#define LINE_ONE 0
#define LINE_TWO 1
#define LINE_THREE 2
#define LINE_FOUR 3

#define CLOCK_LENGTH 7

#define WAIT 1
#define DRIVE 2
#define SPIN_BLACK_LINE 3
#define REVERSE_STATE 5
#define NAVIGATION 6
#define SPIN_CENTER 7
#define DRIVE_CENTER 8

#define IP_TOGGLE_UP_COUNT 5
#define IP_TOGGLE_LOW_COUNT 10
#define BITSHIFT_8 8

//------------------------------------------------------------------------------
//Serial
//------------------------------------------------------------------------------
#define S115200B_BRW 4
#define S115200B_CTLW 0x5551

#define S460800B_BRW 17
#define S460800B_CTLW 0x4A00

#define MESSAGE_END '%'
#define MESSAGE_BEGIN '$'

#define BEGINNING (0)                   
#define SMALL_RING_SIZE (16)            
#define SMALL_BUFFER_MASK 0x0f          // 0b 0000_1111
#define SMALL_BUFFER_REV_MASK 0xf0      // 0b 1111_0000


#define MEDIUM_RING_SIZE 32
#define MEDIUM_BUFFER_REV_MASK 0xff00      // 0b 1111_0000

#define TRANSMIT_STATE 0x000             //TX state machine
#define CARRIAGE_RETURN_STATE 0x100
#define NEWLINE_STATE 0x200
#define DISABLE_TX_INTERRUPT 0x300

//------------------------------------------------------------------------------
//Masks
//------------------------------------------------------------------------------
#define TWO_BIT_MASK 0x03               // 0b 0000_0011


//------------------------------------------------------------------------------
//Navigation macros
//------------------------------------------------------------------------------


#define MIN_SPEED 0                     //smallest pwm period allowed
#define MAX_SPEED_NAV 5000              //largest pwm period allowed
#define BLACK_LINE_VALUE 631            //The value that the black line is above (was 631)
#define WHITE_VALUE_MAX 150             //the value the white line is below

#define OFF_SPEED 1750                  //speed the wheel must be above to stop stalling
#define RECOVERY_TIME 150               //time needed to turn to get back on the line

#define KP 25                           //the mutiplicative constant for the controller

#define WHITE_STATE 0                   //states to determine the cars position
#define LEFT_STATE 1
#define RIGHT_STATE 2
#define LINE_STATE 3

//------------------------------------------------------------------------------
//Timer macros
//------------------------------------------------------------------------------
#define ONE_SECOND 200                  //values for a 5ms timer
#define TWO_SECONDS 400
#define THREE_SECONDS 600
#define FOUR_SECONDS 800
#define FIVE_SECONDS 1000

#define MAX_SCREEN_CLOCK_VALUE 9998     //sets the largest value the timer can reach

//timer 0
#define TB0CCR0_INTERVAL 625
#define TB0CCR1_INTERVAL 62500
#define TB0CCR2_INTERVAL 62500
//timer 1
#define TB1CCR0_INTERVAL 4096
#define TB1CCR1_INTERVAL 2048
#define TB1CCR2_INTERVAL 819
//time 2
#define TB2CCR1_INTERVAL 2000

#define TIME_SEQUENCE_MAX_COUNT 250
#define DAV_V_DEC_VAL 3   
#define NAV_ENABLE_PERIOD 2
   
#define TB_NO_INT 0
#define TB_CTL1_INT 2
#define TB_CTL2_INT 4
#define TB_OVER_INT 14
   
#define WSYNCINTRL_TIMER 700
#define NSTCP_TIMER 1000
#define PING_TIMER 1200   

#define IOT_ENABLE_COUNT_VAL 15

//------------------------------------------------------------------------------
//Port macros
//------------------------------------------------------------------------------
//Port 1
#define RED_LED (0x01)
#define A1_SEEED (0x02)
#define V_DETECT_L (0x04)
#define V_DETECT_R (0x08)
#define A4_SEEED (0x10)
#define V_THUMB (0x20)
#define UCA0RXD (0x40)
#define UCA0TXD (0x80)

//Port 2
#define P2_0 (0x01)
#define IR_LED (0x02)
#define P2_2 (0x04)
#define SW2 (0x08)
#define IOT_PROGRAM_SELECT (0x10)
#define P5_5 (0x20)
#define XOUT (0x40)
#define XIN (0x80)

//Port 3
#define TEST_PROBE (0x01)
#define OPT_INT (0x02)
#define OA2M (0x04)
#define OA2P (0x08)
#define SMCLK (0x10)
#define DAC_CNTL (0x20)
#define IOT_LINK (0x40)
#define IOT_RESET (0x80)

//Port 4
#define RESET_LCD (0x01)
#define SW1 (0x02)
#define UCA1RXD (0x04)
#define UCA1TXD (0x08)
#define UCB1_CS_LCD (0x10)
#define UCB1SCK (0x20)
#define UCB1SIMO (0x40)
#define UCB1SOMI (0x80)

//Port 5
#define CHECK_BAT (0x01)
#define V_BAT (0x02)
#define V_DAC (0x04)
#define V_3_3 (0x08)
#define IOT_PROGRAM_SELECT (0x10)

//Port 6
#define R_FORWARD (0x01)
#define L_FORWARD (0x02)
#define R_REVERSE (0x04)
#define L_REVERSE (0x08)
#define LCD_BACKLITE (0x10)
#define P6_5 (0x20)
#define GRN_LED (0x40)


//------------------------------------------------------------------------------
//from clocks.c
//------------------------------------------------------------------------------
#define MCLK_FREQ_MHZ 8             // MCLK = 8MHz
#define CLEAR_REGISTER (0X0000)

//------------------------------------------------------------------------------
//switch macros
//------------------------------------------------------------------------------
#define SWITCH_RESET_TIME 20       //the debounce time for the swithces
#define PRESSED  1
#define RELEASED 0
