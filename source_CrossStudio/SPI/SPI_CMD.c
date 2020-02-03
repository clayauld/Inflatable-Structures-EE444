#include <msp430.h>
#include "nR24L01.h"

void LP_init()
{
  P1OUT = 0x00; // disables all pins
  P2OUT = 0x00;
  P3OUT = 0x00;
  P4OUT = 0x00;
  P5OUT = 0x00;
  P6OUT = 0x00;
  P7OUT = 0x00;
  P8OUT = 0x00;
  PJOUT = 0x00;

  P1DIR = 0xFF;
  P2DIR = 0xFF;
  P3DIR = 0xFF;
  P4DIR = 0xFF;
  P5DIR = 0xFF;
  P6DIR = 0xFF;
  P7DIR = 0xFF;
  P8DIR = 0xFF;
  PJDIR = 0xFF;
}

void SPI_init()
{
  // Setup SPI //
  UCA0CTL1 |= UCSWRST; // Puts state machine to rest
  UCA0CTL0 |= UCCKPL | UCMSB | UCMST | UCSYNC; // Clock polarity is high | Most Significant bit | 8 bit by default | Master mode | 3 pin mode by default | synchronous mode
  UCA0CTL1 |= UCSSEL_2; // Select SMCLK as source
  UCA0BR0 = 0x02;
  UCA0BR1 = 0;
  UCA0CTL1 &= ~UCSWRST;
}

void SPI_pins_init()
{
  P2SEL |= BIT2;
  P2DIR |= BIT2;
                                                                                      // 2.7 clock
  P2SEL |= BIT7; // Setup output pins                                                 // 3.3 UCA0TXD/ UCA0SIMO
  P2DIR |= BIT7;  // Set direction out so information hits transceiver                // 3.4 UCA0RXD/ UCA0SOMI
  P3SEL |= BIT3 | BIT4 | BIT5 | BIT6;                                                 // 3.5 Chip Select CSN
  P3DIR |= BIT3 | BIT5 | BIT6; // Output                                              // 3.6 CE control
  P3DIR &= ~BIT4; // Input  
}

void CE_LOW()
{
 P3OUT &= ~BIT6; // P3.6 LOW
}
void CE_HIGH()
{
 P3OUT |= BIT6; // P3.6 HIGH
}

void CSN_LOW ()
{
 P3OUT &= ~BIT5; // P3.5 LOW
}
void CSN_HIGH ()
{
 P3OUT |= BIT5; // P3.5 HIGH
}