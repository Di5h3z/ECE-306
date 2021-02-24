//------------------------------------------------------------------------------
//Description:  This file contains the functions and variables that control the
//              drive functionality of the car.
//
//By:           Nathan Carels
//Date:         2/6/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------

#include "msp430.h"
#include "macros.h"

//globals
extern char start_cycle_time;
extern unsigned int cycle_time;
extern unsigned int motor_count;

extern char next_state;
extern char state;
extern char first_run;
char fig_8_state;
char triangle_state;

char num_turns_completed;

//Moves the both wheels forward
void forward(void){
  if(!(P6OUT&L_FORWARD)){
    P6OUT |= L_FORWARD;
  }

  if(!(P6OUT&R_FORWARD)){
    P6OUT |= R_FORWARD;
  }

}

//stops the car
void stop(void){
  P6OUT &= ~R_FORWARD;
  P6OUT &= ~L_FORWARD;
}

void circle(void){
  start_cycle_time = TRUE;
  if(cycle_time < C_TRAVEL){
    if(motor_count > CCW_R_COUNT){
      P6OUT &= ~R_FORWARD;
    }
    if(motor_count > CCW_L_COUNT){
      P6OUT &= ~L_FORWARD;
    }

    if(motor_count > MAX_COUNT){
      forward();
      motor_count=RESET_STATE;
      }

  }else{
    start_cycle_time = FALSE;
    cycle_time = RESET_STATE;
    next_state = FIGURE_8;
    first_run = 1;
    state= WAIT;
    stop();
    //circle is finished
  }
}


void figure_eight(void){
  switch (fig_8_state){
    case FIRST_CW:
      start_cycle_time = TRUE;
      if(cycle_time < FIG8_CYCLE){
        if(motor_count > CCW_R_COUNT){
          P6OUT &= ~R_FORWARD;
        }
        if(motor_count > CCW_L_COUNT){
          P6OUT &= ~L_FORWARD;
        }

        if(motor_count > MAX_COUNT){
          forward();
          motor_count=RESET_STATE;
          }

      }else{
        start_cycle_time = FALSE;
        cycle_time = RESET_STATE;
        fig_8_state = FIRST_CCW;
        
        stop();
        //circle is finished
      }
      break;
    case FIRST_CCW:
      start_cycle_time = TRUE;
      if(cycle_time < FIG8_CYCLE){
        if(motor_count > CCCW_R_COUNT){
          P6OUT &= ~R_FORWARD;
        }
        if(motor_count > CCCW_L_COUNT){
          P6OUT &= ~L_FORWARD;
        }

        if(motor_count > MAX_COUNT){
          forward();
          motor_count=RESET_STATE;
          }

      }else{
        start_cycle_time = FALSE;
        cycle_time = RESET_STATE;
        fig_8_state = SECOND_CW;
        stop();
        //circle is finished
      }
      break;

      case SECOND_CW:
        start_cycle_time = TRUE;
        if(cycle_time < FIG8_CYCLE){
          if(motor_count > CCW_R_COUNT){
            P6OUT &= ~R_FORWARD;
          }
          if(motor_count > CCW_L_COUNT){
            P6OUT &= ~L_FORWARD;
          }

          if(motor_count > MAX_COUNT){
            forward();
            motor_count=RESET_STATE;
            }

        }else{
          start_cycle_time = FALSE;
          cycle_time = RESET_STATE;
          fig_8_state = SECOND_CCW;
          stop();
          //circle is finished
        }
        break;
      case SECOND_CCW:
        start_cycle_time = TRUE;
        if(cycle_time < FIG8_CYCLE){
          if(motor_count > CCCW_R_COUNT){
            P6OUT &= ~R_FORWARD;
          }
          if(motor_count > CCCW_L_COUNT){
            P6OUT &= ~L_FORWARD;
          }

          if(motor_count > MAX_COUNT){
            forward();
            motor_count=RESET_STATE;
            }

        }else{
          start_cycle_time = FALSE;
          cycle_time = RESET_STATE;
          fig_8_state = DONE;
          stop();
          //circle is finished
        }
        break;
      case DONE:
        state= WAIT;
        next_state = TRIANGLE;
        first_run = 1;
        break;
    default:break;
  }
}



void triangle(void){
  switch (triangle_state) {
    case STRAIGHT:
      start_cycle_time = TRUE;
      if(cycle_time < STRAIGHT_TIME){
        forward();
      }else{
        start_cycle_time = FALSE;
        cycle_time = RESET_STATE;
        stop();
        triangle_state = TURNING;

      }
      break;
    case TURNING:
      start_cycle_time = TRUE;
      if(cycle_time < TURN_TIME){
        P6OUT |= R_FORWARD;
        P6OUT &= ~L_FORWARD;
      }else{

        start_cycle_time = FALSE;
        cycle_time = RESET_STATE;
        stop();

        if(num_turns_completed++ < NUM_TURNS){
          triangle_state = STRAIGHT;
        }else{
          triangle_state = DONE;
        }

      }
      break;
    case DONE:
      state= WAIT;
      next_state = CIRCLE;
      break;
    default: break;
  }
}
