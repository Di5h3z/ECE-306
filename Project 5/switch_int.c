
#include "macros.h"
#include "msp430.h"
#include "functions.h"

#pragma vector=PORT4_VECTOR

char switch1_readable;
char switch1_pressed;
char switch1_count;

char switch2_readable;
char switch2_pressed;
char switch2_count;

__interrupt void switch_interrupt(void) {
  // Switch 1
  if (P4IFG & SW1) {
    P4IFG &= ~SW1;

    switch1_pressed = TRUE;
    switch1_readable = FALSE;
    switch1_count = RESET_STATE;
    P4IE &= ~SW1;
    // Clear any current timer interrupt
    P6OUT &= ~LCD_BACKLITE ; // LCD_BACKLITE off to indicate boot ISR working
  }
  // Use a Timer Value to control the debounce

}


#pragma vector=PORT2_VECTOR
__interrupt void switch_interrupt(void) {
// Switch 2
  if (P2IFG & SW2) {
    P2IFG &= ~SW2; // IFG SW1 cleared
    switch2_pressed = TRUE;
    switch2_readable = FALSE;
    switch2_count = RESET_STATE;
    P2IE &= ~SW2;
    // Clear any current timer interrupt.
    P6OUT &= ~LCD_BACKLITE ; // LCD_BACKLITE off to indicate boot ISR working
  }
// Use a Timer Value to control the debounce
}
