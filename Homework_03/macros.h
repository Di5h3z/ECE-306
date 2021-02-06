//------------------------------------------------------------------------------
//
//Description:  This file contains all the macros used throughout the project
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
//Port macros
//------------------------------------------------------------------------------
//Port 1
#define RedLED               (0x01)
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

//from main.c
#define ALWAYS                  (1)
#define RESET_STATE             (0)
#define RED_LED              (0x01) // RED LED 0 (move these later after ports is finished)
#define GRN_LED              (0x40) // GREEN LED 1

//from clocks.c
#define MCLK_FREQ_MHZ 8             // MCLK = 8MHz
#define CLEAR_REGISTER     (0X0000)

//