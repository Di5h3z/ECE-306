//------------------------------------------------------------------------------
//Description:  This file contains the functions and variables that control the
//              timers based on time sequence
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------
#include  "msp430.h"
#include "macros.h"

//#define DEBOUNCE_COUNT 20

//refrencing the global timer variables
  unsigned int Second_Count;
  unsigned int Time_Sequence;
  unsigned int Time_Sequence_Prev;

//for shape/drive control
  extern char right_wheel_count;
  extern char left_wheel_count;
  extern unsigned int speed_count;

 //for updating state machine in main (will get replaced with a timer interrupt)
  extern unsigned int state_count;

  //display
  extern volatile unsigned char update_display;

  
//------------------------------------------------------------------------------
//updating the timers every time time sequence increment                        Timers_Process
//------------------------------------------------------------------------------
void Timers_Process(void){
  //as fast as possible
  speed_count++;
  if(speed_count > MAX_SPEED){
    speed_count = RESET_STATE;
  }

  //based on time sequence
  if(Time_Sequence != Time_Sequence_Prev){
    Time_Sequence_Prev = Time_Sequence;
    
    if(Time_Sequence > 250){
      Time_Sequence = RESET_STATE;
    }
    state_count++;

    //this will get replaced with PWM
    right_wheel_count++;
    left_wheel_count++;
  }
}


//------------------------------------------------------------------------------
//Initilizes the B0 timer that controls time sequence and switch debounce       Init_Timer_B0
//------------------------------------------------------------------------------
void Init_Timer_B0(void){
  
  //set SMCLK source
  TB0CTL = TBSSEL__SMCLK;
  //reset TB0
  TB0CTL |= TBCLR;
  //enable continuous mode
  TB0CTL |= MC__CONTINUOUS;
  
  TB0CTL |= ID__8;                      //now 4MHz
  TB0EX0 = TBIDEX__8;                   //now 500Khz
  
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

#define TB1CCR0_INTERVAL 4000
//#define TB1CCR1_INTERVAL 
//#define TB1CCR2_INTERVAL 

void Init_Timer_B1(void){
  TB1CTL = TBSSEL__ACLK;                //ACLK 32Khz
  TB1CTL |= TBCLR;                       //clear timer
  TB1CTL |= MC__CONTINUOUS;              //continuous mode
  
  TB1CTL |= ID__8;                      //now 4khz
  TB1EX0 = TBIDEX__1;                   
  
  TB1CCR0 = TB1CCR0_INTERVAL;
  TB1CCTL0 &= ~CCIFG;
  TB1CCTL0 |= CCIE;
  
  TB1CCTL1 &= ~CCIFG;
  TB1CCTL1 &= ~CCIE;
  
  TB1CCTL2 &= ~CCIFG;
  TB1CCTL2 &= ~CCIE;
  
  TB1CTL &= ~TBIFG;
  TB1CTL |= TBIE;
}


#pragma vector = TIMER1_B0_VECTOR
__interrupt void Timer1_B0_ISR(void){
  Second_Count++;
  TB1CCR0 += TB1CCR0_INTERVAL;
}


#pragma vector=TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void){
                                        // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
switch(__even_in_range(TB1IV,14)){
  case 0: break;                        
  case 2: break;
  case 4: break;
  case 14:break;
  default: break;
  }
}

//------------------------------------------------------------------------------
//init all timers                                                               Init_Timers
//------------------------------------------------------------------------------
void Init_Timers(void){
  Init_Timer_B0();
  Init_Timer_B1();
}

#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void){
                                        // TimerB0 0 Interrupt handler for 5msec timer
Time_Sequence++;
TB0CCR0 += TB0CCR0_INTERVAL;            // Add Offset to TBCCR0
}


#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void){
                                        // TimerB0 1-2, Overflow Interrupt Vector (TBIV) handler
switch(__even_in_range(TB0IV,14)){
  case 0: break;                        // No interrupt
  case 2:                               // CCR1 Used for SW1 Debounce
          TB0CCTL1 &= ~CCIE;            //disable capture control reg 1 interupt
          P4IFG &= ~SW1;                //clear SW1 interupts
          P4IE |= SW1;                  //enable SW1 interupt
          TB0CCR1 += TB0CCR1_INTERVAL;  // Add Offset to TBCCR1
          break;
  case 4: // CCR2 Used for SW2 Debounce
          TB0CCTL2 &= ~CCIE;            //disable capture control reg 2 interupt
          P2IFG &= ~SW2;                //clear SW2 interupts
          P2IE |= SW2;                  //enable SW2 interupt
          TB0CCR2 += TB0CCR2_INTERVAL;  // Add Offset to TBCCR2
          break;
  case 14: 
          update_display = TRUE;
          P6OUT |= LCD_BACKLITE;        // Turn on backlight
          TB0CTL &= ~TBIE;              //disable interupt 
          TB0CTL &= ~TBIFG;             // Clear Overflow Interrupt flag
          break;
  default: break;
  }
}



