//------------------------------------------------------------------------------
//  Description: This file contains the function for error checking
//
//By:           Nathan Carels
//Date:         3/9/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------

void error(void){
  //checks to see if the same wheel is being driven in oposite directions
  if((RIGHT_FORWARD_SPEED && RIGHT_REVERSE_SPEED) || (LEFT_FORWARD_SPEED && LEFT_REVERSE_SPEED)){
    RIGHT_FORWARD_SPEED = WHEEL_OFF;
    RIGHT_REVERSE_SPEED = WHEEL_OFF;
    LEFT_FORWARD_SPEED = WHEEL_OFF;
    LEFT_REVERSE_SPEED = WHEEL_OFF;

    lcd_line4("WHEEL ERR");
  }
}
