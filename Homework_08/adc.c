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


char eight_count_mask = 0x07;

int v_left_average[8];
char v_left_next;
char adc_char_left[6];

int v_right_average[8];
char v_right_next;
char adc_char_right[6];

int v_thumb_average[8];
char v_thumb_next;
char adc_char_thumb[6];

char adc_char[6];
char ADC_Channel;


void display_averages(void){

  int v_left_total=0;
  int v_right_total=0;
  int v_thumb_total=0;
  
  for(int i = 0; i< 8; i++){
    v_left_total += v_left_average[i];
    v_right_total += v_right_average[i];
    v_thumb_total += v_thumb_average[i];
  }
  
  screen1_line1 = "L,R,Thumb";
  HEXtoBCD(v_left_total>>3);
  str_cpy(adc_char_left, adc_char);
  screen1_line2 = adc_char_left;
  HEXtoBCD(v_right_total>>3);
  str_cpy(adc_char_right, adc_char);
  screen1_line3 = adc_char_right;
  HEXtoBCD(v_thumb_total>>3);
  str_cpy(adc_char_thumb, adc_char);
  screen1_line4 = adc_char_thumb;
}

int return_vleft_average(void){
  int v_left_total=0;
  
  for(int i = 0; i< 8; i++){
    v_left_total += v_left_average[i];
  }
  
  return v_left_total>>3;
}

int return_vright_average(void){
  int v_right_total=0;
  
  for(int i = 0; i< 8; i++){
    v_right_total += v_right_average[i];
  }
  
  return v_right_total>>3;
}

int return_vthumb_average(void){
  int v_thumb_total=0;
  
  for(int i = 0; i< 8; i++){
    v_thumb_total += v_thumb_average[i];
  }
  
  return v_thumb_total>>3;
}


void HEXtoBCD(int init_val){
  int value=0;
  int hex_value;
  if( init_val>=0){
    adc_char[0] = '+';
    hex_value = init_val;
  }else{
    adc_char[0] = '-';
    hex_value = 0-init_val;
  }
    
  
  while (hex_value > 999){
    hex_value = hex_value - 1000;
    value = value + 1;
  }
  adc_char[1] = 0x30 + value;
  value = 0;
  while (hex_value > 99){
    hex_value = hex_value - 100;
    value = value + 1;
  }
  adc_char[2] = 0x30 + value;
  value = 0;
  while (hex_value > 9){
    hex_value = hex_value - 10;
    value = value + 1;
  }
  adc_char[3] = 0x30 + value;
  adc_char[4] = 0x30 + hex_value;
  adc_char[5] = '\0';
}

void Init_ADC(void){
  //------------------------------------------------------------------------------
  // V_DETECT_L (0x04) // Pin 2 A2
  // V_DETECT_R (0x08) // Pin 3 A3
  // V_THUMB (0x20) // Pin 5 A5
  //------------------------------------------------------------------------------

  // ADCCTL0 Register
  ADCCTL0 = 0;                          // Reset
  ADCCTL0 |= ADCSHT_2;                  // 16 ADC clocks
  ADCCTL0 |= ADCMSC;                    // MSC
  ADCCTL0 |= ADCON;                     // ADC ON
  // ADCCTL1 Register
  ADCCTL2 = 0;                          // Reset
  ADCCTL1 |= ADCSHS_0;                  // 00b = ADCSC bit
  ADCCTL1 |= ADCSHP;                    // ADC sample-and-hold SAMPCON signal from sampling timer.
  ADCCTL1 &= ~ADCISSH;                  // ADC invert signal sample-and-hold.
  ADCCTL1 |= ADCDIV_0;                  // ADC clock divider - 000b = Divide by 1
  ADCCTL1 |= ADCSSEL_0;                 // ADC clock MODCLK
  ADCCTL1 |= ADCCONSEQ_0;               // ADC conversion sequence 00b = Single-channel single-conversion
  // ADCCTL1 & ADCBUSY identifies a conversion is in process
  // ADCCTL2 Register
  ADCCTL2 = 0;                          // Reset
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
          case 0x00:                    // Channel A2 Interrupt aka V_DETECT_L
            ADCMCTL0 &= ~ADCINCH_2;     // Disable Last channel A2
            ADCMCTL0 |= ADCINCH_3;      // Enable Next channel A3
                                        // adds the value to the circular buffer
            v_left_average[v_left_next++ & eight_count_mask] = ADCMEM0; 
            
            ADCCTL0 |= ADCENC; // Enable Conversions
            ADCCTL0 |= ADCSC; 
            break;
          case 0x01:
            ADCMCTL0 &= ~ADCINCH_3;     // Disable Last channel A2
            ADCMCTL0 |= ADCINCH_5;      // Enable Next channel A3
                                        // adds the value to the circular buffer           
            v_right_average[v_right_next++ & eight_count_mask] = ADCMEM0 + VRIGHT_CALIBRATION;
            
            ADCCTL0 |= ADCENC; // Enable Conversions
            ADCCTL0 |= ADCSC; 
            break;
          case 0x02:
            
            ADCMCTL0 &= ~ADCINCH_5;     // Disable Last channel A2
            ADCMCTL0 |= ADCINCH_2;      // Enable Next channel A3
                                        // adds the value to the circular buffer           
            v_thumb_average[v_thumb_next++ & eight_count_mask] = ADCMEM0;
          
            ADC_Channel=0;
            break;
          default:
            break;
        }

      break;
    default:
      break;
  }
}


