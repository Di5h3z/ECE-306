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


#define C_TRAVEL      3700
#define MAX_COUNT     10

#define CCW_R_COUNT     10
#define CCW_L_COUNT      5

#define CCCW_R_COUNT     5
#define CCCW_L_COUNT     10


//refrenceing the global variables in main
extern char start_cycle_time;
extern unsigned int cycle_time;
extern unsigned char motor_count;
extern char next_state;

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

void circle_cw(void){
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
    //state = 0; update the the next shape here
    stop();
    //circle is finished
  }
}

void circle_ccw(void){
  start_cycle_time = TRUE;
  if(cycle_time < C_TRAVEL){
    if(motor_count > CCCW_L_COUNT){
      P6OUT &= ~R_FORWARD;
    }
    if(motor_count > CCCW_R_COUNT){
      P6OUT &= ~L_FORWARD;
    }

    if(motor_count > MAX_COUNT){
      forward();
      motor_count=RESET_STATE;
      }

  }else{
    start_cycle_time = FALSE;
    cycle_time = 0;
    next_state = 0;
    stop();
    //circle is finished
  }
}

#define FIRST_CW    0
#define FIRST_CCW   1
#define SECOND_CW   2
#define SECOND_CCW  3
#define DONE        4
#define FIG8_CYCLE  1800

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
        if(motor_count > CCCW_L_COUNT){
          P6OUT &= ~R_FORWARD;
        }
        if(motor_count > CCCW_R_COUNT){
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
          if(motor_count > CCCW_L_COUNT){
            P6OUT &= ~R_FORWARD;
          }
          if(motor_count > CCCW_R_COUNT){
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
        //update the state to the next shape
    default:break;
  }
}

#define STRAIGHT 200
#define NUM_TURNS 6
#define TURN_TIME 50

#define TURNING 1
#define STRAIGHT 0

void triangle(void){
  switch (triangle_state) {
    case STRAIGHT:
      start_cycle_time = TRUE;
      if(cycle_time < STRAIGHT){
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
        6OUT &= ~L_FORWARD;
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
      //update the next state here
      break;
    default: break;
  }
}
