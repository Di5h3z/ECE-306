//------------------------------------------------------------------------------
//Description:  This file contains the functions for reading in switch input
//
//By:           Nathan Carels
//Date:         2/14/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------

#include "macros.h"
#include "msp430.h"
#include "functions.h"

char switch1_readable;
char switch1_pressed;
char switch1_count;

char switch2_readable;
char switch2_pressed;
char switch2_count;

extern char state;


//------------------------------------------------------------------------------
//Port 4 ISR: switch1                                                           switchP4_interrupt
//------------------------------------------------------------------------------
#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void) {
  // Switch 1
  if (P4IFG & SW1) {
    P4IE &= ~SW1;
    P4IFG &= ~SW1;
    switch1_pressed = TRUE;
    switch1_readable = FALSE;
    switch1_count = RESET_STATE;
    state = DRIVE;


    //enables and increments the interupt timer
    TB0CCTL1 &= ~CCIFG;
    TB0CCR1 = TB0R + TB0CCR1_INTERVAL;
    TB0CCTL1 |= CCIE;
  }

}

//------------------------------------------------------------------------------
//Port 2 ISR: switch2                                                           switchP2_interrupt
//------------------------------------------------------------------------------
#pragma vector=PORT2_VECTOR
__interrupt void switchP2_interrupt(void) {
// Switch 2
  if (P2IFG & SW2) {
    P2IE &= ~SW2;
    P2IFG &= ~SW2; // IFG SW1 cleared
    switch2_pressed = TRUE;
    switch2_readable = FALSE;
    switch2_count = RESET_STATE;

    P2OUT ^= IR_LED;
    
    //enables and increments the interupt timer
    TB0CCTL2 &= ~CCIFG;
    TB0CCR2 = TB0R + TB0CCR2_INTERVAL;
    TB0CCTL2 |= CCIE;
  }

}
