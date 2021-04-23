//------------------------------------------------------------------------------
//Description:  This file contains the functions and variables that control the
//              timers and thier interrupts
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------
#include  "msp430.h"
#include "macros.h"
#include "functions.h"

//Refrencing the global timer variables
  unsigned int Second_Count;
  unsigned int Time_Sequence;                                     
  unsigned char nav_count;        
  
  //for switch debounce
  extern char switch1_debounce;
  extern char switch2_debounce;
  
  //for shape/drive control
  extern char right_wheel_count;
  extern char left_wheel_count;
  extern unsigned int speed_count;
  
  //for black line navigation
  extern char nav_enable;
  extern unsigned int recovery_count;
  extern unsigned int off_line_count;
                                        //for updating state machine in main (will get replaced with a timer interrupt)
   extern unsigned int state_count;

  //Display Updates
   extern volatile unsigned char update_display;
   extern int screen_clock;

   //IOT enable
   int IOT_enable_count;
   extern int command_timer;
//------------------------------------------------------------------------------
//Initilizes the B0 timer that controls time sequence and switch debounce       Init_Timer_B0
//------------------------------------------------------------------------------
void Init_Timer_B0(void){

  TB0CTL = TBSSEL__SMCLK;               //set SMCLK source

  TB0CTL |= TBCLR;                      //reset TB0

  TB0CTL |= MC__CONTINUOUS;             //enable continuous mode


  TB0CTL |= ID__8;                      //now 1MHz
  TB0EX0 = TBIDEX__8;                   //now 150Khz

                                        //Capture Compare Registers
  TB0CCR0 = TB0CCR0_INTERVAL;           // CCR0
  TB0CCTL0 &= ~CCIFG;                   // Clear possible pending interrupt
  TB0CCTL0 |= CCIE;                     // CCR0 enable interrupt

  TB0CCR1 = TB0CCR1_INTERVAL;           // CCR1
  TB0CCTL1 &= ~CCIFG;                   // Clear possible pending interrupt
  TB0CCTL1 |= CCIE;                     // CCR1 enable interrupt

  TB0CCR2 = TB0CCR2_INTERVAL;           // CCR2
  TB0CCTL2 &= ~CCIFG;                   // Clear possible pending interrupt
  TB0CCTL2 |= CCIE;                     // CCR2 enable interrupt



//USE THIS TO SET THE IOT MODULE ON FIRST OVERFLOW THEN STOP THE INTERUPT
//THIS ALSO SETS update_display = TRUE;
  TB0CTL |= TBIE;                       // Enable Overflow Interrupt
  TB0CTL &= ~TBIFG;                     // Clear Overflow Interrupt flag

}

//------------------------------------------------------------------------------
//TimerB0 CCR0 interrupt                                                        Timer0_B0_ISR
//------------------------------------------------------------------------------
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
                                                // TimerB0 0 Interrupt handler for 5msec timer
  if(Time_Sequence++ > 250)
    Time_Sequence = RESET_STATE;

  if(nav_count++ > 2){ //every 10 ms
    nav_enable = TRUE;
    nav_count = 0;
  }
  
  if(SAC3OA & OAEN && SAC3DAT > 1200){          //controls the DAC voltage rise NOTE: consider adding this to a one time setup time used to handel all the various setup when the IOT module is added
    SAC3DAT -= 3;
  }
  
  
  //Various 5ms timers
  state_count++;
  right_wheel_count++;
  left_wheel_count++;
  recovery_count++;
  off_line_count++;

  
  
  TB0CCR0 = TB0R + TB0CCR0_INTERVAL;            // Add Offset to TBCCR0
}

//------------------------------------------------------------------------------
//TimerB0 CCR1-2 interrupt                                                      TIMER0_B1_ISR
//------------------------------------------------------------------------------
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
                                        // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
switch(__even_in_range(TB0IV,14)){
  case 0: break;                        // No interrupt
  case 2:                               // CCR1 Used for SW1 Debounce
          TB0CCTL1 &= ~CCIE;            //disable capture control reg 1 interupt
          P4IFG &= ~SW1;                //clear SW1 interupts
          P4IE |= SW1;                  //enable SW1 interupt
          break;
  case 4: // CCR2 Used for SW2 Debounce
          TB0CCTL2 &= ~CCIE;            //disable capture control reg 2 interupt
          P2IFG &= ~SW2;                //clear SW2 interupts
          P2IE |= SW2;                  //enable SW2 interupt
          break;
  case 14:
    
    
          
          update_display = TRUE;
          SAC3OA |= OAEN;               // Enable DAC
          LCD_ON;                       // Turn on backlight
          TB0CTL &= ~TBIE;              //disable interupt
          TB0CTL &= ~TBIFG;             // Clear Overflow Interrupt flag
          break;
  default: break;
  }
}

//------------------------------------------------------------------------------
//Initilizes the B1 timer that controls human timers .25ms-8 seconds              Init_Timer_B1
//------------------------------------------------------------------------------
void Init_Timer_B1(void){
  TB1CTL = TBSSEL__ACLK;                //ACLK 32Khz
  TB1CTL |= TBCLR;                      //clear timer
  TB1CTL |= MC__CONTINUOUS;             //continuous mode

  TB1CTL |= ID__8;                      //now 4khz
  TB1EX0 = TBIDEX__1;                   

  TB1CCR0 = TB1CCR0_INTERVAL;           //CCR0
  TB1CCTL0 &= ~CCIFG;                   // Clear possible pending interrupt
  TB1CCTL0 |= CCIE;                     // CCR2 enable interrupt

  TB1CCR1 = TB1CCR1_INTERVAL;
  TB1CCTL1 &= ~CCIFG;                   // Clear possible pending interrupt
  TB1CCTL1 &= ~CCIE;                    // CCR2 enable interrupt

  TB1CCR2 = TB1CCR2_INTERVAL;
  TB1CCTL2 &= ~CCIFG;                   // Clear possible pending interrupt
  TB1CCTL2 |= CCIE;                    // CCR2 disable interrupt

  TB1CTL &= ~TBIFG;                     // Clear possible pending interrupt
  TB1CTL &= ~TBIE;                      // CCR2 disable interrupt
}

//------------------------------------------------------------------------------
//TimerB1 CCR0 interrupt                                                        Timer1_B0_ISR
//------------------------------------------------------------------------------
#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void){
  Second_Count++;
  
  
  TB1CCR0 = TB1R + TB1CCR0_INTERVAL;
}

//------------------------------------------------------------------------------
//TimerB1 CCR1-2 interrupt                                                      TIMER1_B1_ISR
//------------------------------------------------------------------------------
#pragma vector=TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void){
                                        // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
switch(__even_in_range(TB1IV,14)){
  case 0: break;
  case 2: 
    if(switch1_debounce){
      switch1_debounce = FALSE;
      TB1CCTL1 &= ~CCIE;            //disable capture control reg 1 interupt
      P4IFG &= ~SW1;                //clear SW1 interupts
      P4IE |= SW1;                  //enable SW1 interupt
    }
    
    if(switch2_debounce){
      switch2_debounce = FALSE;
      TB1CCTL1 &= ~CCIE;            //disable capture control reg 2 interupt
      P2IFG &= ~SW2;                //clear SW2 interupts
      P2IE |= SW2;                  //enable SW2 interupt
    }
    
    break;
  case 4: 
    command_timer += 1;
    screen_clock += 2;
    update_menu = TRUE;
    TB1CCR2 = TB1R + TB1CCR2_INTERVAL;
    break;
  case 14:break;
  default: break;
  }
}

//------------------------------------------------------------------------------
//Initilizes the B2 timer that controls very fast timer                         Init_Timer_B2
//------------------------------------------------------------------------------
void Init_Timer_B2(void){
TB2CTL = TBSSEL__SMCLK;
TB2CTL |= TBCLR;
TB2CTL |= MC__CONTINUOUS; 

TB2CTL |= ID__1; //8MHz
TB2CTL |= TBIDEX_1;

//TB2CCR0 = 
TB2CCTL0 &= ~CCIFG;
TB2CCTL0 &= ~CCIE;

TB2CCR1 = TB2CCR1_INTERVAL;
TB2CCTL1 &= ~CCIFG;
TB2CCTL1 |= CCIE;

//TB2CCR2 =
TB2CCTL2 &= ~CCIFG;
TB2CCTL2 &= ~CCIE;

TB2CTL &= ~TBIFG;
TB2CTL |= TBIE;
}

#pragma vector = TIMER2_B0_VECTOR
__interrupt void Timer2_b0_isr(void){


}

#define IOT_ENABLE_COUNT_VAL 15

#pragma vector = TIMER2_B1_VECTOR
__interrupt void Timer2_b1_isr(void){
switch(__even_in_range(TB2IV,14)){
  case 0: break;
  case 2: 
    //takes an ADC measurement every .25 ms
    ADCCTL0 |= ADCENC;                  // Enable Conversions
    ADCCTL0 |= ADCSC; 
    
    TB2CCR1 = TB2R + TB2CCR1_INTERVAL;
    break;
  case 4: break;
  case 14:
          if(IOT_enable_count++ > IOT_ENABLE_COUNT_VAL)
              P3OUT |= IOT_RESET;                    //Set IOT_RESET high
          if(IOT_enable_count == IOT_ENABLE_COUNT_VAL + 700)
            IOT_tx("\nAT+WSYNCINTRL=60000\r");
          if(IOT_enable_count == IOT_ENABLE_COUNT_VAL + 1000)
            IOT_tx("\nAT+NSTCP=25565,1\r");
          if(IOT_enable_count == IOT_ENABLE_COUNT_VAL + 1200){
            IOT_tx("\nAT+PING=8.8.8.8,1\r");
            TB2CTL &= ~TBIE;
          }
          break;
  
  default: break;
  }

}

//------------------------------------------------------------------------------
//Initilizes the B1 timer that controls PWM for wheel speed                     Init_Timer_B3
//------------------------------------------------------------------------------
void Init_Timer_B3(void) {
//------------------------------------------------------------------------------
// SMCLK source, up count mode, PWM Right Side
// TB3.1 P6.0 R_FORWARD
// TB3.2 P6.1 L_FORWARD
// TB3.3 P6.2 R_REVERSE
// TB3.4 P6.3 L_REVERSE
//------------------------------------------------------------------------------
 TB3CTL = TBSSEL__SMCLK; // SMCLK
 TB3CTL |= MC__UP; // Up Mode
 TB3CTL |= TBCLR; // Clear TAR
 TB3CCR0 = WHEEL_PERIOD; // PWM Period
 TB3CCTL1 = OUTMOD_7; // CCR1 reset/set
 RIGHT_FORWARD_SPEED = WHEEL_OFF; // P6.0 Right Forward PWM duty cycle
 TB3CCTL2 = OUTMOD_7; // CCR2 reset/set
 LEFT_FORWARD_SPEED = WHEEL_OFF; // P6.1 Left Forward PWM duty cycle
 TB3CCTL3 = OUTMOD_7; // CCR3 reset/set
 RIGHT_REVERSE_SPEED = WHEEL_OFF; // P6.2 Right Reverse PWM duty cycle
 TB3CCTL4 = OUTMOD_7; // CCR4 reset/set
 LEFT_REVERSE_SPEED = WHEEL_OFF; // P6.3 Left Reverse PWM duty cycle
//------------------------------------------------------------------------------
}



//------------------------------------------------------------------------------
//init all timers                                                               Init_Timers
//------------------------------------------------------------------------------
void Init_Timers(void){
  Init_Timer_B0();
  Init_Timer_B1();
  Init_Timer_B2();
  Init_Timer_B3();
}






