//------------------------------------------------------------------------------
//
//Description:  This file contains all the macros used throughout the project
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//Main macros
//------------------------------------------------------------------------------
#define ONE_SECOND 200
#define TWO_SECONDS 400
#define THREE_SECONDS 600
#define FOUR_SECONDS 800
#define FIVE_SECONDS 1000

#define WAIT 1
#define DRIVE 2
#define BLACK_LINE_DETECTED 3
#define SPIN 4
#define REVERSE_STATE 5
#define NAVIGATION 6
#define BLACK_LINE 500

//------------------------------------------------------------------------------
//ADC macros
//------------------------------------------------------------------------------


#define VRIGHT_CALIBRATION 3
//------------------------------------------------------------------------------
//Timer macros
//------------------------------------------------------------------------------

//timer 1
#define TB0CCR0_INTERVAL 625
#define TB0CCR1_INTERVAL 62500
#define TB0CCR2_INTERVAL 62500
//timer 2
#define TB1CCR0_INTERVAL 4000
#define TB1CCR1_INTERVAL 1

//------------------------------------------------------------------------------
//Drive macros
//------------------------------------------------------------------------------
#define FORWARD 1
#define REVERSE 0
#define CHANGE_COUNT 5
#define MAX_SPEED 10000

#define WHEEL_PERIOD 10000
#define WHEEL_OFF 0

#define RIGHT_FORWARD_SPEED (TB3CCR1)
#define LEFT_FORWARD_SPEED (TB3CCR2)
#define RIGHT_REVERSE_SPEED (TB3CCR3)
#define LEFT_REVERSE_SPEED (TB3CCR4)
//------------------------------------------------------------------------------
//LCD Dispaly Macros
//------------------------------------------------------------------------------
#define LINE1 0
#define LINE2 1
#define LINE3 2
#define LINE4 3

#define LCD_OFF P6OUT &= ~LCD_BACKLITE
#define LCD_ON P6OUT |= LCD_BACKLITE
#define LCD_TOGGLE P6OUT ^= LCD_BACKLITE

#define MAX_LCD_LENGTH 11
#define CHAR_0 0

//------------------------------------------------------------------------------
//General macros
//------------------------------------------------------------------------------
#define TRUE 1
#define FALSE 0
#define ALWAYS                  (1)
#define RESET_STATE             (0)


//------------------------------------------------------------------------------
//Port macros
//------------------------------------------------------------------------------
//Port 1
#define RED_LED              (0x01)
#define A1_SEEED             (0x02)
#define V_DETECT_L           (0x04)
#define V_DETECT_R           (0x08)
#define A4_SEEED             (0x10)
#define V_THUMB              (0x20)
#define UCA0RXD              (0x40)
#define UCA0TXD              (0x80)

//Port 2
#define P2_0                 (0x01)
#define IR_LED               (0x02)
#define P2_2                 (0x04)
#define SW2                  (0x08)
#define IOT_PROGRAM_SELECT   (0x10)
#define P5_5                 (0x20)
#define XOUT                 (0x40)
#define XIN                  (0x80)

//Port 3
#define TEST_PROBE           (0x01)
#define OPT_INT              (0x02)
#define OA2M                 (0x04)
#define OA2P                 (0x08)
#define SMCLK                (0x10)
#define DAC_CNTL             (0x20)
#define IOT_LINK             (0x40)
#define IOT_RESET            (0x80)

//Port 4
#define RESET_LCD            (0x01)
#define SW1                  (0x02)
#define UCA1RXD              (0x04)
#define UCA1TXD              (0x08)
#define UCB1_CS_LCD          (0x10)
#define UCB1SCK              (0x20)
#define UCB1SIMO             (0x40)
#define UCB1SOMI             (0x80)

//Port 5
#define CHECK_BAT            (0x01)
#define V_BAT                (0x02)
#define V_DAC                (0x04)
#define V_3_3                (0x08)
#define IOT_PROGRAM_SELECT   (0x10)

//Port 6
#define R_FORWARD            (0x01)
#define L_FORWARD            (0x02)
#define R_REVERSE            (0x04)
#define L_REVERSE            (0x08)
#define LCD_BACKLITE         (0x10)
#define P6_5                 (0x20)
#define GRN_LED              (0x40)


//------------------------------------------------------------------------------
//from clocks.c
//------------------------------------------------------------------------------
#define MCLK_FREQ_MHZ 8             // MCLK = 8MHz
#define CLEAR_REGISTER     (0X0000)

//------------------------------------------------------------------------------
//switch macros
//------------------------------------------------------------------------------
#define SWITCH_RESET_TIME 20
#define PRESSED  1
#define RELEASED 0
