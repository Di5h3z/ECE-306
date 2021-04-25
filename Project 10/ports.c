//------------------------------------------------------------------------------
//Description:  This file contains the initialization functions and configurations
//              for all the ports. This file handles the calling of each individual
//              port and the configuration. Pin Defines are stored in macros.h
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------

#include "macros.h"
#include  "msp430.h"

void Init_Port1(void){                  //Configure PORT 1

P1OUT = 0x00;                           //P1 set Low
P1DIR = 0x00;                           //Set P1 direction to input

//P1 PIN 0
P1SEL0 &= ~RED_LED;                      //RedLED G I/O operation
P1SEL1 &= ~RED_LED;                      //RedLED G I/O operation
P1OUT &= ~RED_LED;                       //Set RedLED Low
P1DIR |= RED_LED;                        //Set RedLED Output

//P1 PIN 1
P1SELC |= A1_SEEED;                     //A1_SEEED FUNC operation (this is the pin that the light detector is on)

//P1 PIN 2
P1SELC |= V_DETECT_L;                   //V_DETECT_L FUNC operation

//P1 PIN 3
P1SELC |= V_DETECT_R;                   //V_DETECT_R FUNC operation

//P1 PIN 4
P1SELC |= A4_SEEED;                     //A4_SEEED FUNC operation

//P1 PIN 5
P1SELC |= V_THUMB;                      //V_THUMB FUNC operation

//P1 PIN 6
P1SEL0 |= UCA0RXD;                      //UCA0RXD FUNC operation
P1SEL1 &= ~UCA0RXD;                     //UCA0RXD FUNC operation

//P1 PIN 7
P1SEL0 |= UCA0TXD;                      //UCA0TXD FUNC operation
P1SEL1 &= ~UCA0TXD;                     //UCA0TXD FUNC operation

}

void Init_Port2(void){                  //Configure PORT 2

P2OUT = 0x00;                           //P2 set Low
P2DIR = 0x00;                           //Set P2 direction to input

//P2 PIN 0
P2SEL0 &= ~P2_0;                        //P2_0 G I/O operation
P2SEL1 &= ~P2_0;                        //P2_0 G I/O operation
P2DIR &= ~P2_0;                         //Set P2_0 Input

//P2 PIN 1
P2SEL0 &= ~IR_LED;                      //IR_LED G I/O operation
P2SEL1 &= ~IR_LED;                      //IR_LED G I/O operation
P2OUT &= ~IR_LED;                        //Set IR_LED high
P2DIR |= IR_LED;                        //Set IR_LED Output

//P2 PIN 2
P2SEL0 &= ~P2_2;                        //P2_2 G I/O operation
P2SEL1 &= ~P2_2;                        //P2_2 G I/O operation
P2DIR &= ~P2_2;                         //Set P2_2 Input

//P2 PIN 3
P2SEL0 &= ~SW2;                         //SW2 G I/O operation
P2SEL1 &= ~SW2;                         //SW2 G I/O operation
P2OUT |= SW2;                           //Set SW2 High
P2DIR &= ~SW2;                          //Set SW2 Input
P2REN |= SW2;                           //Set SW2 Pull up resistor
P2IES |= SW2;                           //SW2 Hi/Lo edge interrupt
P2IFG &= ~SW2;                          //IFG SW2 cleared
P2IE |= SW2;                            //SW2 interrupt Enabled

//P2 PIN 4
P2SEL0 &= ~IOT_PROGRAM_SELECT;          //IOT_PROGRAM_SELECT G I/O operation
P2SEL1 &= ~IOT_PROGRAM_SELECT;          //IOT_PROGRAM_SELECT G I/O operation
P2DIR &= ~IOT_PROGRAM_SELECT;            //Set IOT_PROGRAM_SELECT Input

//P2 PIN 5
P2SEL0 &= ~P5_5;                        //P5_5 G I/O operation
P2SEL1 &= ~P5_5;                        //P5_5 G I/O operation
P2DIR &= ~P5_5;                         //Set P5_5 Input

//P2 PIN 6
P2SEL0 &= ~XOUT;                        //XOUT FUNC operation
P2SEL1 |= XOUT;                         //XOUT FUNC operation

//P2 PIN 7
P2SEL0 &= ~XIN;                         //XIN FUNC operation
P2SEL1 |= XIN;                          //XIN FUNC operation

}

void Init_Port3(void){                  //Configure PORT 3

P3OUT = 0x00;                           //P3 set Low
P3DIR = 0x00;                           //Set P3 direction to input

//P3 PIN 0
P3SEL0 &= ~TEST_PROBE;                  //TEST_PROBE G I/O operation
P3SEL1 &= ~TEST_PROBE;                  //TEST_PROBE G I/O operation
P3OUT &= ~TEST_PROBE;
P3DIR |= TEST_PROBE;                   //Set TEST_PROBE Input

//P3 PIN 1
P3SEL0 &= ~OPT_INT;                     //OPT_INT G I/O operation
P3SEL1 &= ~OPT_INT;                     //OPT_INT G I/O operation
P3DIR &= ~OPT_INT;                      //Set OPT_INT Input

//P3 PIN 2
P3SEL0 |= OA2M;                         //OA2M FUNC operation
P3SEL1 |= OA2M;                         //OA2M FUNC operation

//P3 PIN 3
P3SEL0 |= OA2P;                         //OA2P FUNC operation
P3SEL1 |= OA2P;                         //OA2P FUNC operation

//P3 PIN 4
P3SEL0 |= SMCLK;                        //SMCLK FUNC operation
P3SEL1 &= ~SMCLK;                       //SMCLK FUNC operation
P3DIR |= SMCLK;

//P3 PIN 5
P3SELC |= DAC_CNTL;

//P3SEL0 &= ~DAC_CNTL;                     //DAC_CNTL OA3O 
//P3SEL1 &= ~DAC_CNTL;
//P3OUT |= DAC_CNTL;  
//P3DIR |= DAC_CNTL;

//P3 PIN 6
P3SEL0 &= ~IOT_LINK;                    //IOT_LINK G I/O operation
P3SEL1 &= ~IOT_LINK;                    //IOT_LINK G I/O operation
P3OUT &= ~IOT_LINK;                     //Set IOT_LINK Low
P3DIR &= ~IOT_LINK;                     //Set IOT_LINK Input

//P3 PIN 7
P3SEL0 &= ~IOT_RESET;                   //IOT_RESET G I/O operation
P3SEL1 &= ~IOT_RESET;                   //IOT_RESET G I/O operation
P3OUT &= ~IOT_RESET;                    //Set IOT_RESET Low
P3DIR |= IOT_RESET;                     //Set IOT_RESET Output

}

void Init_Port4(void){                  //Configure PORT 4

P4OUT = 0x00;                           //P4 set Low
P4DIR = 0x00;                           //Set P4 direction to input

//P4 PIN 0
P4SEL0 &= ~RESET_LCD;                   //RESET_LCD G I/O operation
P4SEL1 &= ~RESET_LCD;                   //RESET_LCD G I/O operation
P4OUT &= ~RESET_LCD;                    //Set RESET_LCD Low
P4DIR |= RESET_LCD;                     //Set RESET_LCD Output

//P4 PIN 1
P4SEL0 &= ~SW1;                         //SW1 G I/O operation
P4SEL1 &= ~SW1;                         //SW1 G I/O operation
P4OUT |= SW1;                           //Set SW1 High
P4DIR &= ~SW1;                          //Set SW1 Input
P4REN |= SW1;                           //Set SW1 Pull up resistor
P4IES |= SW1;                           //SW1 Hi/Lo edge interrupt
P4IFG &= ~SW1;                          //IFG SW1 cleared
P4IE |= SW1;                            //SW1 interrupt Enabled

//P4 PIN 2
P4SEL0 |= UCA1RXD;                      //UCA1RXD FUNC operation
P4SEL1 &= ~UCA1RXD;                     //UCA1RXD FUNC operation

//P4 PIN 3
P4SEL0 |= UCA1TXD;                      //UCA1TXD FUNC operation
P4SEL1 &= ~UCA1TXD;                     //UCA1TXD FUNC operation

//P4 PIN 4
P4SEL0 &= ~UCB1_CS_LCD;                 //UCB1_CS_LCD G I/O operation
P4SEL1 &= ~UCB1_CS_LCD;                 //UCB1_CS_LCD G I/O operation
P4OUT |= UCB1_CS_LCD;                   //Set UCB1_CS_LCD High
P4DIR |= UCB1_CS_LCD;                   //Set UCB1_CS_LCD Output

//P4 PIN 5
P4SEL0 |= UCB1SCK;                      //UCB1SCK FUNC operation
P4SEL1 &= ~UCB1SCK;                     //UCB1SCK FUNC operation

//P4 PIN 6
P4SEL0 |= UCB1SIMO;                     //UCB1SIMO FUNC operation
P4SEL1 &= ~UCB1SIMO;                    //UCB1SIMO FUNC operation

//P4 PIN 7
P4SEL0 |= UCB1SOMI;                     //UCB1SOMI G I/O operation
P4SEL1 &= ~UCB1SOMI;                    //UCB1SOMI G I/O operation

}

void Init_Port5(void){                  //Configure PORT 5

P5OUT = 0x00;                           //P5 set Low
P5DIR = 0x00;                           //Set P5 direction to input

//P5 PIN 0
P5SEL0 &= ~CHECK_BAT;                   //CHECK_BAT G I/O operation
P5SEL1 &= ~CHECK_BAT;                   //CHECK_BAT G I/O operation
P5OUT &= ~CHECK_BAT;                    //Set CHECK_BAT Low
P5DIR |= CHECK_BAT;                     //Set CHECK_BAT Output

//P5 PIN 1
P5SELC |= V_BAT;                        //V_BAT FUNC operation

//P5 PIN 2
P5SELC |= V_DAC;                        //V_DAC FUNC operation

//P5 PIN 3
P5SELC |= V_3_3;                        //V_3_3 FUNC operation

//P5 PIN 4
P5SEL0 &= ~IOT_PROGRAM_SELECT;          //IOT_PROGRAM_SELECT G I/O operation
P5SEL1 &= ~IOT_PROGRAM_SELECT;          //IOT_PROGRAM_SELECT G I/O operation
P5DIR &= ~IOT_PROGRAM_SELECT;           //Set IOT_PROGRAM_SELECT Input

}

void Init_Port6(void){                  //Configure PORT 6

P6OUT = 0x00;                           //P6 set Low
P6DIR = 0x00;                           //Set P6 direction to input

//P6 PIN 0
P6SEL0 |= R_FORWARD;                    //R_FORWARD PWM
P6SEL1 &= ~R_FORWARD;                   //R_FORWARD PWM
P6OUT &= ~R_FORWARD;                    //Set R_FORWARD Low
P6DIR |= R_FORWARD;                     //Set R_FORWARD Output

//P6 PIN 1
P6SEL0 |= L_FORWARD;                    //L_FORWARD PWM
P6SEL1 &= ~L_FORWARD;                   //L_FORWARD PWM
P6OUT &= ~L_FORWARD;                    //Set L_FORWARD Low
P6DIR |= L_FORWARD;                     //Set L_FORWARD Output

//P6 PIN 2
P6SEL0 |= R_REVERSE;                    //R_REVERSE PWM
P6SEL1 &= ~R_REVERSE;                   //R_REVERSE PWM
P6OUT &= ~R_REVERSE;                    //Set R_REVERSE Low
P6DIR |= R_REVERSE;                     //Set R_REVERSE Output

//P6 PIN 3
P6SEL0 |= L_REVERSE;                    //L_REVERSE PWM
P6SEL1 &= ~L_REVERSE;                   //L_REVERSE PWM
P6OUT &= ~L_REVERSE;                    //Set L_REVERSE Low
P6DIR |= L_REVERSE;                     //Set L_REVERSE Output

//P6 PIN 4
P6SEL0 &= ~LCD_BACKLITE;                //LCD_BACKLITE G I/O operation
P6SEL1 &= ~LCD_BACKLITE;                //LCD_BACKLITE G I/O operation
P6OUT &= ~LCD_BACKLITE;                 //Set LCD_BACKLITE Low
P6DIR |= LCD_BACKLITE;                  //Set LCD_BACKLITE Output

//P6 PIN 5
P6SEL0 &= ~P6_5;                        //P6_5 G I/O operation
P6SEL1 &= ~P6_5;                        //P6_5 G I/O operation
P6DIR &= ~P6_5;                         //Set P6_5 Input

//P6 PIN 6
P6SEL0 &= ~GRN_LED;                     //GRN_LED G I/O operation
P6SEL1 &= ~GRN_LED;                     //GRN_LED G I/O operation
P6OUT |= GRN_LED;                       //Set GRN_LED High
P6DIR |= GRN_LED;                       //Set GRN_LED Output

}
