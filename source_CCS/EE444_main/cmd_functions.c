#include <msp430.h>
#include "init_setups.h"

char _string[70];
//unsigned int i = 0;
unsigned int _start = 0;
char the_buffer[4];

#define DT_NUM  8               // Multiple Data array size
#define VS      2.50            // ADC reference = 2.5V
#define PE      1.00            // Pressure Error, 1 = No error
#define TM40    3.00            // Temp. multiplier for -40 Degree Range
#define TM85    1.00            // Temp. multiplier for 0-85 Degree Range
#define TM125   3.00            // Temp. multiplier for +125 Degree Range
#define ADCRES  (VS/(4096.00))
#define OFFSET0_MIN  0.088
#define OFFSET0_TYP  0.200
#define OFFSET0_MAX  0.313

unsigned int cmd_compare(char  *_buffer)
{
//******************************* Check for characters of interest and process them accordingly *****************************************************
            // Create a buffer array to hold last 4 characters received.
            // This can be thought of as a software shift register for 8 bit characters
  the_buffer[3] = _buffer[3];
  the_buffer[2] = _buffer[2];
  the_buffer[1] = _buffer[1];
  the_buffer[0] = _buffer[0];

 /* if (UCA1RXBUF == 0x00d)                     // Return key pressed
  {                                   //
    UCA1IE |= UCTXIE;                       // Enable interrupt on USCI_A1 TX
    while (!(UCA1IFG & UCTXIFG));           // USCI_A1 TX buffer ready?
    UCA1TXBUF = _string[i++];                //
  }*/

  if (the_buffer[3] == 0x4D /* M */ && the_buffer[2] == 0x45 /* E */ && the_buffer[1] == 0x41 /* A */ && the_buffer[0] == 0x53 /* S */)
  {
    _start = 1;                              // Set start flag to 1
    P1OUT ^= BIT0;                          // Toggle LED1
    TA1CTL |= MC__UP;                       // Set TA1 to start counting UP
    return _start;
  }

  else if (the_buffer[3] == 0x53 /* S */ && the_buffer[2] == 0x54 /* T */ && the_buffer[1] == 0x4F /* O */ && the_buffer[0] == 0x50 /* P */)
  {
    _start = 0;                              // Set start flag to 0
    P1OUT ^= BIT0;                          // Toggle LED1
    TA1CTL |= MC__STOP;                     // Set TA1 to STOP
    return _start;
  }
  return _start;
}

/*void cmd_end_compare()
{
 UCA1TXBUF = _string[i++];                    // Send next character
 if (i >= sizeof(_string) || _string[i] == 0x0)// Detect null character or end of string
 {
  UCA1IE &= ~UCTXIE;                      // Disable interrupt on USCI_A1 TX
 }
}*/

float getPressure(int ADCdata, int TM, long useOffset)
{

    double Pmax = 0, Pmin = 0;
    double Vout = (double) ADCdata * ADCRES;
    double ERR = PE * (double)TM * VS * 0.009;

    Pmax = (((Vout + ERR - useOffset)/VS) + 0.095) / 0.009;
    Pmin = (((Vout - ERR - useOffset)/VS) + 0.095) / 0.009;
    Pmax *=  0.145; // 1 kPa =  0.145 psi
    Pmin *=  0.145; // 1 kPa =  0.145 psi
    return (float)Pmax;
}
