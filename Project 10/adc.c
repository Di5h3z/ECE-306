//------------------------------------------------------------------------------
//Description:  This file contains the ADC interrupts and init
//
//By:           Nathan Carels
//Date:         3/9/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------
#include "functions.h"
#include "msp430.h"
#include "macros.h"


//Globals
  char eight_count_mask = EIGHT_COUNT_MASK;

  int v_left_average[NUM_READINGS];
  char v_left_next;
  char adc_char_left[BCD_MAX_LEN];

  int v_right_average[NUM_READINGS];
  char v_right_next;
  char adc_char_right[BCD_MAX_LEN];

  int v_thumb_average[NUM_READINGS];
  char v_thumb_next;
  char adc_char_thumb[BCD_MAX_LEN];

  char adc_char[BCD_MAX_LEN];
  char ADC_Channel;

//------------------------------------------------------------------------------
// puts the averages to the menu display                                        display_averages
// Passed:      None
// Returned:    None
//------------------------------------------------------------------------------
void display_averages(void){

  int v_left_total=ZERO;
  int v_right_total=ZERO;
  int v_thumb_total=ZERO;
  
  for(int i = BEGINNING; i< NUM_READINGS; i++){
    v_left_total += v_left_average[i];
    v_right_total += v_right_average[i];
    v_thumb_total += v_thumb_average[i];
  }
  
  screen1_line1 = "L,R,Thumb";
  HEXtoBCD(v_left_total>>BITSHIFT_DIVIDE_8);
  str_cpy(adc_char_left, adc_char);
  screen1_line2 = adc_char_left;
  HEXtoBCD(v_right_total>>BITSHIFT_DIVIDE_8);
  str_cpy(adc_char_right, adc_char);
  screen1_line3 = adc_char_right;
  HEXtoBCD(v_thumb_total>>BITSHIFT_DIVIDE_8);
  str_cpy(adc_char_thumb, adc_char);
  screen1_line4 = adc_char_thumb;
}

//------------------------------------------------------------------------------
// gets and computes the average for the left IR detector                       return_vleft_average
// Passed:      None
// Returned:    left IR detector average
//------------------------------------------------------------------------------
int return_vleft_average(void){
  int v_left_total=ZERO;
  
  for(int i = BEGINNING; i< NUM_READINGS; i++){
    v_left_total += v_left_average[i];
  }
  
  return v_left_total>>BITSHIFT_DIVIDE_8;
}

//------------------------------------------------------------------------------
// gets and computes the average for the right IR detector                       return_vright_average
// Passed:      None
// Returned:    right IR detector average
//------------------------------------------------------------------------------
int return_vright_average(void){
  int v_right_total=ZERO;
  
  for(int i = BEGINNING; i< NUM_READINGS; i++){
    v_right_total += v_right_average[i];
  }
  
  return v_right_total>>BITSHIFT_DIVIDE_8;
}

//------------------------------------------------------------------------------
// gets and computes the average for the thumbwheel                             return_vthumb_average
// Passed:      None
// Returned:    thumbwheel average
//------------------------------------------------------------------------------
int return_vthumb_average(void){
  int v_thumb_total=ZERO;
  
  for(int i = BEGINNING; i< NUM_READINGS; i++){
    v_thumb_total += v_thumb_average[i];
  }
  
  return v_thumb_total>>BITSHIFT_DIVIDE_8;
}

//------------------------------------------------------------------------------
// returns a decimal string from a hex number                                   HEXtoBCD
// Passed:      init_val
// Returned:    None
//------------------------------------------------------------------------------
void HEXtoBCD(int init_val){
  int value=ZERO;
  int hex_value;
  if( init_val>=ZERO){
    adc_char[BEGINNING] = PLUS_CHAR;
    hex_value = init_val;
  }else{
    adc_char[BEGINNING] = MINUS_CHAR;
    hex_value = ZERO-init_val;
  }
    
  
  while (hex_value > MAX_SUB_THOUSAND){
    hex_value = hex_value - THOUSAND;
    value++;
  }
  adc_char[SECOND_CHAR] = ASCII_TO_DEC + value;
  value = ZERO;
  while (hex_value > MAX_SUB_HUNDRED){
    hex_value = hex_value - HUNDRED;
    value++;
  }
  adc_char[THIRD_CHAR] = ASCII_TO_DEC + value;
  value = ZERO;
  while (hex_value > MAX_SUB_TEN){
    hex_value = hex_value - TEN;
    value++;
  }
  adc_char[FOURTH_CHAR] = ASCII_TO_DEC + value;
  adc_char[FITH_CHAR] = ASCII_TO_DEC + hex_value;
  adc_char[SIXTH_CHAR] = NULL_CHAR;
}


//------------------------------------------------------------------------------
// initilizes the ADC                                                           Init_ADC
// Passed:      None
// Returned:    None
//------------------------------------------------------------------------------
void Init_ADC(void){
  //------------------------------------------------------------------------------
  // V_DETECT_L (0x04) // Pin 2 A2
  // V_DETECT_R (0x08) // Pin 3 A3
  // V_THUMB (0x20) // Pin 5 A5
  //------------------------------------------------------------------------------

  // ADCCTL0 Register
  ADCCTL0 = ZERO;                          // Reset
  ADCCTL0 |= ADCSHT_2;                  // 16 ADC clocks
  ADCCTL0 |= ADCMSC;                    // MSC
  ADCCTL0 |= ADCON;                     // ADC ON
  // ADCCTL1 Register
  ADCCTL2 = ZERO;                          // Reset
  ADCCTL1 |= ADCSHS_0;                  // 00b = ADCSC bit
  ADCCTL1 |= ADCSHP;                    // ADC sample-and-hold SAMPCON signal from sampling timer.
  ADCCTL1 &= ~ADCISSH;                  // ADC invert signal sample-and-hold.
  ADCCTL1 |= ADCDIV_0;                  // ADC clock divider - 000b = Divide by 1
  ADCCTL1 |= ADCSSEL_0;                 // ADC clock MODCLK
  ADCCTL1 |= ADCCONSEQ_0;               // ADC conversion sequence 00b = Single-channel single-conversion
  // ADCCTL1 & ADCBUSY identifies a conversion is in process
  // ADCCTL2 Register
  ADCCTL2 = ZERO;                          // Reset
  ADCCTL2 |= ADCPDIV0;                  // ADC pre-divider 00b = Pre-divide by 1
  ADCCTL2 |= ADCRES_1;                  // ADC resolution 0x01b 10bit
  ADCCTL2 &= ~ADCDF;                    // ADC data read-back format 0b = Binary unsigned.
  ADCCTL2 &= ~ADCSR;                    // ADC sampling rate 0b = ADC buffer supports up to 200 ksps
  // ADCMCTL0 Register
  ADCMCTL0 |= ADCSREF_0;                // VREF - 000b = {VR+ = AVCC and VR– = AVSS }
  ADCMCTL0 |= ADCINCH_5;                // V_THUMB (0x20) Pin 5 A5
  ADCIE |= ADCIE0;                      // Enable ADC conv complete interrupt
  ADCCTL0 |= ADCENC;                    // ADC enable conversion.
  ADCCTL0 |= ADCSC;                     // ADC start conversion.
}


//------------------------------------------------------------------------------
// ISR to read ADC values                                                       ADC_ISR
// Passed:      None
// Returned:    None
//------------------------------------------------------------------------------
#pragma vector=ADC_VECTOR
__interrupt void ADC_ISR(void){
  switch(__even_in_range(ADCIV,ADCIV_ADCIFG)){
    case ADCIV_NONE:
      break;
    case ADCIV_ADCOVIFG:                // When a conversion result is written to the ADCMEM0
                                        // before its previous conversion result was read.
      break;
    case ADCIV_ADCTOVIFG:               // ADC conversion-time overflow
      break;
    case ADCIV_ADCHIIFG:                // Window comparator interrupt flags
      break;
    case ADCIV_ADCLOIFG:                // Window comparator interrupt flag
      break;
    case ADCIV_ADCINIFG:                // Window comparator interrupt flag
      break;
    case ADCIV_ADCIFG:                  // ADCMEM0 memory register with the conversion result
      
      //do the thumb wheel first to test and then add the other stuff
      
      ADCCTL0 &= ~ADCENC;               // Disable ENC bit.
      
        switch (ADC_Channel++){
          case L_DETECT_CASE:                    // Channel A2 Interrupt aka V_DETECT_L
            ADCMCTL0 &= ~ADCINCH_2;     // Disable Last channel A2
            ADCMCTL0 |= ADCINCH_3;      // Enable Next channel A3
                                        // adds the value to the circular buffer
            v_left_average[v_left_next++ & eight_count_mask] = ADCMEM0; 
            
            ADCCTL0 |= ADCENC;          // Enable Conversions 
            ADCCTL0 |= ADCSC; 
            break;
          case R_DETECT_CASE:
            ADCMCTL0 &= ~ADCINCH_3;     // Disable Last channel A2
            ADCMCTL0 |= ADCINCH_5;      // Enable Next channel A3
                                        // adds the value to the circular buffer           
            v_right_average[v_right_next++ & eight_count_mask] = ADCMEM0 + VRIGHT_CALIBRATION;
            
            ADCCTL0 |= ADCENC;          // Enable Conversions
            ADCCTL0 |= ADCSC; 
            break;
          case T_DETECT_CASE:
            
            ADCMCTL0 &= ~ADCINCH_5;     // Disable Last channel A2
            ADCMCTL0 |= ADCINCH_2;      // Enable Next channel A3
                                        // adds the value to the circular buffer           
            v_thumb_average[v_thumb_next++ & eight_count_mask] = ADCMEM0;
          
            ADC_Channel=ZERO;
            break;
          default:
            break;
        }

      break;
    default:
      break;
  }
}


