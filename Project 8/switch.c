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
char switch1_debounce;
char switch1_count;

char switch2_readable;
char switch2_debounce;
char switch2_count;

extern char state;
extern char line_selected;
extern char select_fired;

//------------------------------------------------------------------------------
//Port 4 ISR: switch1                                                           switchP4_interrupt
//------------------------------------------------------------------------------
#pragma vector=PORT4_VECTOR
__interrupt void switchP4_interrupt(void) {
  // Switch 1
  if (P4IFG & SW1) {
    P4IE &= ~SW1;
    P4IFG &= ~SW1;
    switch1_debounce = TRUE;
    switch1_readable = FALSE;
    switch1_count = RESET_STATE;
    
    select_fired = TRUE;

    //enables and increments the interupt timer
    TB1CCTL1 &= ~CCIFG;
    TB1CCR1 = TB1R + TB1CCR1_INTERVAL;
    TB1CCTL1 |= CCIE;
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
    switch2_debounce = TRUE;
    switch2_readable = FALSE;
    switch2_count = RESET_STATE;

    line_selected = (line_selected+1) & TWO_BIT_MASK;
    //enables and increments the interupt timer
    TB1CCTL1 &= ~CCIFG;
    TB1CCR1 = TB1R + TB1CCR1_INTERVAL;
    TB1CCTL1 |= CCIE;
  }

}
