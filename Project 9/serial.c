//------------------------------------------------------------------------------
//Description:  This file contains the serial communication interrupts and init
//
//By:           Nathan Carels
//Date:         3/25/2021
//Build:        Built with IAR Embedded Workbench Version: V7.20.1.997 (7.20.1)
//------------------------------------------------------------------------------
#include "functions.h"
#include "msp430.h"
#include "macros.h"

//GLOBALS
  //USB
    //RX
    volatile unsigned char USB_rxbuf_index;
    volatile char USB_rxbuf[SMALL_RING_SIZE];
    volatile char USB_message_ready;
    
    volatile char USB_message_index;
    char USB_message[SMALL_RING_SIZE];
    
    //TX
    unsigned char USB_txbuf_index;
    char USB_txbuf[SMALL_RING_SIZE];
    volatile char USB_transmitting;

  //IOT
    //RX
    volatile unsigned char IOT_rxbuf_index;
    volatile char IOT_rxbuf[SMALL_RING_SIZE];
    volatile char IOT_message_ready;

    volatile char IOT_message_index;
    char IOT_message[SMALL_RING_SIZE];

    //TX
    unsigned char IOT_txbuf_index;
    char IOT_txbuf[SMALL_RING_SIZE];
    volatile char IOT_transmitting;

//------------------------------------------------------------------------------
// Inits all of the serial ports @ 115200                                       Init_Serial
//------------------------------------------------------------------------------
void Init_Serial(void){
  Init_Serial_UCA1();
  Init_Serial_UCA0();
}


//------------------------------------------------------------------------------
// Inits the IOT serial port                                                    Init_Serial_UCA0
//------------------------------------------------------------------------------
void Init_Serial_UCA0(void){
//------------------------------------------------------------------------------
// TX error (%) RX error (%)
// BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg   pos  neg   pos
// 8000000 4800   1      104   2    0xD6 -0.08 0.04 -0.10 0.14 
// 8000000 9600   1      52    1    0x49 -0.08 0.04 -0.10 0.14
// 8000000 19200  1      26    0    0xB6 -0.08 0.16 -0.28 0.20
// 8000000 57600  1      8     10   0xF7 -0.32 0.32 -1.00 0.36
// 8000000 115200 1      4     5    0x55 -0.80 0.64 -1.12 1.76
// 8000000 460800 0      17    0    0x4A -2.72 2.56 -3.76 7.28
//------------------------------------------------------------------------------
// Configure eUSCI_A0 for UART mode
UCA0CTLW0 = 0;
UCA0CTLW0 |= UCSWRST ;                  // Put eUSCI in reset
UCA0CTLW0 |= UCSSEL__SMCLK;             // Set SMCLK as fBRCLK
UCA0CTLW0 &= ~UCMSB;                    // MSB, LSB select
UCA0CTLW0 &= ~UCSPB;                    // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
UCA0CTLW0 &= ~UCPEN;                    // No Parity
UCA0CTLW0 &= ~UCSYNC;
UCA0CTLW0 &= ~UC7BIT;
UCA0CTLW0 |= UCMODE_0;
// BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg pos neg pos
// 8000000 115200 1 4 5 0x55 -0.80 0.64 -1.12 1.76
// UCA?MCTLW = UCSx + UCFx + UCOS16
UCA0BRW = 4 ;                           // 115,200 baud
UCA0MCTLW = 0x5551 ;
UCA0CTLW0 &= ~UCSWRST ;                 // release from reset
UCA0TXBUF = NEWLINE_CHAR;               // Prime the Pump
UCA0IE |= UCRXIE;                       // Enable RX interrupt
UCA0IE |= UCTXIE;                       // Enable TX interrupt
//-----------------------------------------------------------------------------

}

//------------------------------------------------------------------------------
// Begins the transmission of a command to the IOT                              OIT_tx
//------------------------------------------------------------------------------
void IOT_tx(char* command){
  if(IOT_transmitting){
    return;
  }
  
  IOT_transmitting = TRUE;
  char* temp = IOT_txbuf;               
  str_cpy(temp, command);               //copys the command into the USB_tx buffer
  IOT_txbuf_index = BEGINNING;          //sets index to zero (gets incremented before 2nd char is sent)
  UCA0IE |= UCTXIE;                     // Enable TX interrupt
  UCA0TXBUF = IOT_txbuf[BEGINNING];     //kicks off the sending
}

//------------------------------------------------------------------------------
// gets the last IOT command recieved                                           IOT_rx
//------------------------------------------------------------------------------
char* IOT_rx(void){
  if(!IOT_message_ready){
    return NULL_PTR;
  }      
  
  char temp = IOT_message_index;        //holds IOT_message_index
  char index = 0;                       //will places the recieved command at the beginning of IOT_message
                                        //loops from the start of the command to the end
  while(IOT_rxbuf[temp & SMALL_BUFFER_MASK] != CARRIAGE_RETURN_CHAR && index < (SMALL_RING_SIZE-1)){
    char iot_char = IOT_rxbuf[temp++ & SMALL_BUFFER_MASK];  //hold volatile
    IOT_message[index++] = iot_char;    //stores it in the message
  }
  IOT_message[index] = NULL_CHAR;       //appends null
  
  IOT_message_ready = FALSE;            //message has been read wait for next
  return IOT_message;                   //returns the pointer for convient access
  
}

#pragma vector=EUSCI_A0_VECTOR
__interrupt void eUSCI_A0_ISR(void){
  unsigned int temp;
  char IOT_rx_char;
  switch(__even_in_range(UCA0IV,0x08)){
    case 0: // Vector 0 - no interrupt
      break;
    case 2: // Vector 2 – RXIFG
      
      temp = IOT_rxbuf_index++;         //saves the voltile and post increments
      IOT_rx_char = UCA0RXBUF;          //gets the char from the recieve buffer
      
                                        //saves the char in a 16byte circular buffer
      IOT_rxbuf[temp & SMALL_BUFFER_MASK] = IOT_rx_char;
      
                                        //detects when the message is done finished
      if(IOT_rx_char == NEWLINE_CHAR)
        IOT_message_ready = TRUE;
      
                                        //detects when a new message is being recieved and saves the location
      if(IOT_rxbuf[(temp-1) & SMALL_BUFFER_MASK] == NEWLINE_CHAR){
        IOT_message_index = temp & SMALL_BUFFER_MASK;
      }                      
  
      break;
    case 4: // Vector 4 – TXIFG
      
      
      switch(IOT_txbuf_index++ & SMALL_BUFFER_REV_MASK){
        case TRANSMIT_STATE:            //transmits the first 16 character unless a null is found
          UCA0TXBUF = IOT_txbuf[IOT_txbuf_index];
          if(IOT_txbuf[IOT_txbuf_index] == NULL_CHAR){
            IOT_txbuf_index = CARRIAGE_RETURN_STATE;
          }
          break;
        case CARRIAGE_RETURN_STATE:     //carriage return
          UCA0TXBUF = CARRIAGE_RETURN_CHAR;
          IOT_txbuf_index = NEWLINE_STATE;
          break;
        case NEWLINE_STATE:            //line feed
          UCA0TXBUF = NEWLINE_CHAR;
          IOT_txbuf_index = DISABLE_TX_INTERRUPT;
          break;
        default:
          IOT_transmitting = FALSE;
          UCA0IE &= ~UCTXIE;           // Disable TX interrupt
          break;
      }
    default:break;
    }
}


//------------------------------------------------------------------------------
// Inits the USB serial port                                                    Init_Serial_UCA1
//------------------------------------------------------------------------------
void Init_Serial_UCA1(void){
//------------------------------------------------------------------------------
// TX error (%) RX error (%)
// BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg   pos  neg   pos
// 8000000 4800   1      104   2    0xD6 -0.08 0.04 -0.10 0.14 
// 8000000 9600   1      52    1    0x49 -0.08 0.04 -0.10 0.14
// 8000000 19200  1      26    0    0xB6 -0.08 0.16 -0.28 0.20
// 8000000 57600  1      8     10   0xF7 -0.32 0.32 -1.00 0.36
// 8000000 115200 1      4     5    0x55 -0.80 0.64 -1.12 1.76
// 8000000 460800 0      17    0    0x4A -2.72 2.56 -3.76 7.28
//------------------------------------------------------------------------------
// Configure eUSCI_A0 for UART mode
UCA1CTLW0 = 0;
UCA1CTLW0 |= UCSWRST ;                  // Put eUSCI in reset
UCA1CTLW0 |= UCSSEL__SMCLK;             // Set SMCLK as fBRCLK
UCA1CTLW0 &= ~UCMSB;                    // MSB, LSB select
UCA1CTLW0 &= ~UCSPB;                    // UCSPB = 0(1 stop bit) OR 1(2 stop bits)
UCA1CTLW0 &= ~UCPEN;                    // No Parity
UCA1CTLW0 &= ~UCSYNC;
UCA1CTLW0 &= ~UC7BIT;
UCA1CTLW0 |= UCMODE_0;
// BRCLK Baudrate UCOS16 UCBRx UCFx UCSx neg   pos   neg  pos
// 8000000 115200 1      4     5    0x55 -0.80 0.64 -1.12 1.76
// UCA1MCTLW = UCSx + UCFx + UCOS16
UCA1BRW = 4 ;                           // 115,200 baud
UCA1MCTLW = 0x5551 ;
UCA1CTLW0 &= ~UCSWRST ;                 // release from reset
UCA1TXBUF = NEWLINE_CHAR;               // Prime the Pump
UCA1IE |= UCRXIE;                       // Enable RX interrupt
UCA1IE |= UCTXIE;                       // Enable TX interrupt
//------------------------------------------------------------------------------
}


//------------------------------------------------------------------------------
// Begins the transmission of a command to the USB                              USB_tx
//------------------------------------------------------------------------------
void USB_tx(char* command){
  if(USB_transmitting){
    return;
  }
  
  USB_transmitting = TRUE;
  char* temp = USB_txbuf;  
  str_cpy(temp, command);               //copys the command into the USB_tx buffer
  USB_txbuf_index = BEGINNING;          //sets index to zero (gets incremented before 2nd char is sent)
  UCA1IE |= UCTXIE;                     // Enable TX interrupt
  UCA1TXBUF = USB_txbuf[BEGINNING];     //kicks off the sending
}



//------------------------------------------------------------------------------
// gets the last USB command recieved                                           USB_rx
//------------------------------------------------------------------------------
char* USB_rx(void){
  if(!USB_message_ready){
    return NULL_PTR;
  }
  
  char temp = USB_message_index;        //holds IOT_message_index
  char index = 0;                       //will places the recieved command at the beginning of IOT_message
                                        //loops from the start of the command to the end
  while(USB_rxbuf[temp & SMALL_BUFFER_MASK] != NULL_CHAR && index < (SMALL_RING_SIZE-1)){
    char usb_char = USB_rxbuf[temp++ & SMALL_BUFFER_MASK];  //hold volatile
    USB_message[index++] = usb_char;    //stores it in the message
  }
  USB_message[index] = NULL_CHAR;       //appends null
  USB_message_ready = FALSE;
  return USB_message;                   //returns the pointer for convient access
}


#pragma vector=EUSCI_A1_VECTOR
__interrupt void eUSCI_A1_ISR(void){
  unsigned int temp;
  char USB_rx_char;
  switch(__even_in_range(UCA1IV,0x08)){
    case 0: // Vector 0 - no interrupt
      break;
    case 2: // Vector 2 – RXIFG
      temp = USB_rxbuf_index++;
      USB_rx_char = UCA1RXBUF;
                                        //saves the char in a 16byte circular buffer
      USB_rxbuf[temp & SMALL_BUFFER_MASK] = USB_rx_char;
      
                                        //detects when a messaage it done being recieved
      if(USB_rx_char == NEWLINE_CHAR)
        USB_message_ready = TRUE;
   
                                        //Finds the beginning of the message(when it occurs) and saves the location
      if(USB_rxbuf[(temp-1) & SMALL_BUFFER_MASK] == NEWLINE_CHAR){
        USB_message_index = temp & SMALL_BUFFER_MASK;
      }
      
      break;
    case 4: // Vector 4 – TXIFG
      
      
      switch(++USB_txbuf_index & SMALL_BUFFER_REV_MASK){
        case TRANSMIT_STATE:            //transmits the first 16 character unless a null is found
          UCA1TXBUF = USB_txbuf[USB_txbuf_index];
          if(USB_txbuf[USB_txbuf_index] == NULL_CHAR){
            USB_txbuf_index = CARRIAGE_RETURN_STATE;
          }
          break;
        case CARRIAGE_RETURN_STATE:     //carriage return
          UCA1TXBUF = CARRIAGE_RETURN_CHAR;
          USB_txbuf_index = NEWLINE_STATE;
          break;
        case NEWLINE_STATE:             //line feed
          UCA1TXBUF = NEWLINE_CHAR;
          USB_txbuf_index = DISABLE_TX_INTERRUPT;
          break;
        default:
          USB_transmitting = FALSE;
          UCA1IE &= ~UCTXIE;            // Disable TX interrupt
          break;
      }
      break;
    default:break;
    }
}
