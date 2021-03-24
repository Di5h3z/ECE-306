//------------------------------------------------------------------------------
//Description:  This file contains the ADC interrupts and init
//
//By:           Nathan Carels
//Date:         3/14/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------

#include "functions.h"
#include "msp430.h"
#include "macros.h"


void Init_REF(void){
  // Configure reference module
  PMMCTL0_H = PMMPW_H; // Unlock the PMM registers
  PMMCTL2 = INTREFEN; // Enable internal reference
  PMMCTL2 |= REFVSEL_2; // Select 2.5V reference
  Second_Count = 0;
  while(!(PMMCTL2 & REFGENRDY)){
    if(Second_Count >= 4){
      lcd_line1("IntVref Er");
    }
  } 
}

void Init_DAC(void){
  SAC3DAT = MIN_DAC; // Initial DAC data
  SAC3DAC = DACSREF_1; // Select int Vref as DAC reference
  SAC3DAC |= DACLSEL_0; // DAC latch loads when DACDAT written
  // SAC3DAC |= DACIE; // generate an interrupt
  SAC3DAC |= DACEN; // Enable DAC
  SAC3OA = NMUXEN; // SAC Negative input MUX controL
  SAC3OA |= PMUXEN; // SAC Positive input MUX control
  SAC3OA |= PSEL_1; // 12-bit reference DAC source selected
  SAC3OA |= NSEL_1; // Select negative pin input
  SAC3OA |= OAPM; // Select low speed and low power mode
  SAC3PGA = MSEL_1; // Set OA as buffer mode
  SAC3OA |= SACEN; // Enable SAC
}
