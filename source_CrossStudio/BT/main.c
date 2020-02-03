#include <msp430.h>
#include <stdio.h>
#include "init_setups.h"
#include "cmd_functions.h"
//Built for MSP-EXP430F5529LP board
extern int IncrementVcore(void);                       
extern int DecrementVcore(void);

// ************ Variables **************************************
char string[80];                                        // Set up string for output over UART or LCD
                                     
unsigned int start = 0;                                 // Flag for UART start or stop input received
char buffer[4];                                         // Buffer for RX data
float time = 0.0;                                       // Counter to calculate time since MEAS received over UART
char buffer2[4];
float volts_calibrated[4] = {0.0};
float press_calibrated = 0.0;

unsigned int CAL_ADC2_5VREF_FACTOR;
unsigned int CAL_ADC_GAIN_FACTOR;
unsigned int CAL_ADC_OFFSET;

unsigned int i;                                       // Counter variable for USCI TX

float volts_raw_calibrated[4] = {0};   // Variable to hold calibrated raw voltage
float press_raw_calibrated = 0;
//float volts_raw_calibrated2, volts_raw_calibrated3;   // Variable to hold calibrated raw voltage

union float2bytes { float f; char b[sizeof(float)]; };  // Create a union to store the byte array for voltage
                                                        // We can then use this to transmit the bytes over UART or other
                                                        // serial communication in a properly formatted data packet
union float2bytes volts[4];       // Union variable to store voltage bytes for transmission
union float2bytes press;

void main(void)
{
  DecrementVcore();
  DecrementVcore();
  DecrementVcore();
  //IncrementVcore(); // Level 1 up to 12 MHz

  CAL_ADC2_5VREF_FACTOR = *(unsigned int *)0x01A2C;
  CAL_ADC_GAIN_FACTOR = *(unsigned int *)0x01A16;
  CAL_ADC_OFFSET = *(unsigned int *)0x01A18;

  LP_init();

  UCSCTL1 = DCORSEL_4;       ///////////////////
  UCSCTL2 = 243;             // Sets McLK and 
  UCSCTL3 = SELREF_0;        // SMCLK to 10 MHZ
  UCSCTL4 = SELM_3 | SELS_3; /////////////////

  P2SEL |= BIT2; // Out put SMCLK
  P2DIR |= BIT2;
  BT_pins_init();
  Flex_pins_init();
  Switch_pins_init();

  TimerA1_init();
  ADC_12_settings_init();
  ADC_12_sequence_init();

  BT_UART_init();
  Terminal_UART_init();

  _EINT();
  LPM0;
}

void USCI_A0_isr(void)__interrupt[USCI_A0_VECTOR]
{
  switch(UCA0IV)
  {
    case 0: break;
    case 2:	
      while (!(UCA0IFG & UCTXIFG));               // USCI_A1 TX buffer ready?
      UCA1TXBUF = UCA0RXBUF; // Receive input back from the Bluetooth module and display to tera term
      buffer2[3] = buffer2[2];
      buffer2[2] = buffer2[1];
      buffer2[1] = buffer2[0];
      buffer2[0] = UCA0RXBUF;
      start = cmd_compare(buffer2);
      break;
    case 4: break;
    default: break;
  }
}

void USCI_A1_isr(void)__interrupt[USCI_A1_VECTOR]
{
  switch(UCA1IV)
  {
    case 0: break;
    case 2:	                                        

     // UCA1IE |= UCTXIE;                           // Enable interrupt on USCI_A1 TX
      while (!(UCA1IFG & UCTXIFG));               // USCI_A1 TX buffer ready?
      UCA1TXBUF = UCA1RXBUF;//mirror the input to tera term
      UCA0TXBUF = UCA1RXBUF;//mirror input to bluetooth module
      buffer[3] = buffer[2];
      buffer[2] = buffer[1];
      buffer[1] = buffer[0];
      buffer[0] = UCA1RXBUF;
      start = cmd_compare(buffer);
      break;
    case 4:                                         // Vector 4 - TXIFG
      UCA1TXBUF = string[i++];                    // Send next character
      if (i >= sizeof(string) || string[i] == 0x0)// Detect null character or end of string
      {
        UCA1IE &= ~UCTXIE;                      // Disable interrupt on USCI_A1 TX
      }
      break;
    default: break;
  }
}

void __ADC12_ISR(void) __interrupt [ADC12_VECTOR]       // ADC12 ISR
//__attribute__((interrupt(ADC12_VECTOR))) void __ADC12_ISR(void) // ADC12 ISR
{
    float volts_raw[4] = {0.0};//, volts_raw1, volts_raw2, volts_raw3;
    float press_raw = 0.0;
    int counter;                                        //
    float time1 = 0.0;                                  // Local variable for time calculation
    switch(ADC12IV)                                     //
    {
        case ADC12IV_ADC12IFG14:                         // Vector 20: ADC12IFG7
            ADC12IFG &= ~ADC12IFG14;                     // Clear interrupt flag
            for (counter=0; counter<3; counter++)       //
            {
                volts_raw[0] += *(&ADC12MEM0 + counter);
                volts_raw[1] += *(&ADC12MEM3 + counter);
                volts_raw[2] += *(&ADC12MEM6 + counter);
                volts_raw[3] += *(&ADC12MEM9 + counter);
                press_raw += *(&ADC12MEM12 + counter);
            }
            //volts = volts_raw/4*(2.497/4095);
            /********************************************* TO DO: ADC Calibration ****************************************
            To calibrate the ADC and use the voltage reference offsets, use the following formula taken from
            
            https://e2e.ti.com/support/microcontrollers/msp430/f/166/t/204428 
            
            and from pages 82 and 83 of the document slau208p.pdf (MSP430x5xx and MSP430x6xx Family User's Guide).

            ADC(calibrated) = ( (ADC(raw) x CAL_ADC15VREF_FACTOR / 2^15) x (CAL_ADC_GAIN_FACTOR / 2^15) ) + CAL_ADC_OFFSET

            The values in the equation are available in the TLV structure in the datasheet for each chip.

            *************************************************************************************************************/
            volts_raw_calibrated[0] = (((volts_raw[0] * CAL_ADC2_5VREF_FACTOR / 32768.0)*(CAL_ADC_GAIN_FACTOR / 32768.0)) + CAL_ADC_OFFSET)/3;
            volts_raw_calibrated[1] = (((volts_raw[1] * CAL_ADC2_5VREF_FACTOR / 32768.0)*(CAL_ADC_GAIN_FACTOR / 32768.0)) + CAL_ADC_OFFSET)/3;
            volts_raw_calibrated[2] = (((volts_raw[2] * CAL_ADC2_5VREF_FACTOR / 32768.0)*(CAL_ADC_GAIN_FACTOR / 32768.0)) + CAL_ADC_OFFSET)/3;
            volts_raw_calibrated[3] = (((volts_raw[3] * CAL_ADC2_5VREF_FACTOR / 32768.0)*(CAL_ADC_GAIN_FACTOR / 32768.0)) + CAL_ADC_OFFSET)/3;
            //press_raw_calibrated = (((press_raw * CAL_ADC2_5VREF_FACTOR / 32768.0)*(CAL_ADC_GAIN_FACTOR / 32768.0)) + CAL_ADC_OFFSET)/3;
            press.f = getPressure(press_raw, 1, 0);
            volts[0].f = volts_raw_calibrated[0]*(2.5/4096.0);
            volts[1].f = volts_raw_calibrated[1]*(2.5/4096.0);
            volts[2].f = volts_raw_calibrated[2]*(2.5/4096.0);
            volts[3].f = volts_raw_calibrated[3]*(2.5/4096.0);
            //press.f = press_raw_calibrated*(2.5/4096.0);
            sprintf(string, "Flex: %.5f %.5f %.5f %.5f\r\nPressure: %.5f\r\n", volts[0].f, volts[1].f, volts[2].f, volts[3].f, press.f);
                    //volts_raw_calibrated[0], volts_raw_calibrated[1], volts_raw_calibrated[2], volts_raw_calibrated[3], press_raw_calibrated);

           /* while (!(UCA1IFG & UCTXIFG));               // USCI_A1 TX buffer ready?
            i=0;                                        //
            UCA1IE |= UCTXIE;                           // Enable interrupt on USCI_A1 TX
            UCA1TXBUF = string[i++];*/

            
            for(i=0;i<80;i++)
            {
              while (!(UCA1IFG & UCTXIFG));// USCI_A1 TX buffer ready?
              UCA1TXBUF = string[i];//mirror the input to tera term
              UCA0TXBUF = string[i];//mirror input to bluetooth module
            }
             /*while (!(UCA1IFG & UCTXIFG));// USCI_A1 TX buffer ready?

             UCA1TXBUF = 0x0d;
             UCA0TXBUF = 0x0d;
             while (!(UCA1IFG & UCTXIFG));// USCI_A1 TX buffer ready?
             UCA1TXBUF = 0x0a;
             UCA0TXBUF = 0x0a;*/

             
            //i=0;                                        //

            //UCA1IE |= UCTXIE;                           // Enable interrupt on USCI_A1 TX
            //UCA1TXBUF = string[i++]; 
            ADC12CTL0 &= ~ADC12ENC;
            break;
        
        default: break;
    }
}

void __TIMER_A_ISR(void) __interrupt [TIMER1_A1_VECTOR]
{
  switch(TA1IV)
  {
    case 14:
      if (start == 1)
      {
        ADC12CTL0 |= ADC12SC | ADC12ENC;              // Start ADC sample-and-conversion                                    
        P4OUT ^= BIT7;                                // Toggle LED2
      }
      break;
    default: break;
  }
}

//********************* Port2 (Switch1) Interrupt Service Routine ****************************
void __SWITCH_ISR(void) __interrupt [PORT2_VECTOR]

/* This switch shows a lot of bouncing that has to be accounted for in software.
   The basic way of doing this is adding delay, turning off interrupts on the switch during the ISR,
   and clearing interrupt flags as the first and last tasks of the ISR. This is not the most low-power
   way of doing things, but it **mostly** works. A better way of doing this would be to use a timer
   and check that a specific amount of time has passed before triggering the ISR process.
*/

{
  switch(P2IV)
  {
    case P2IV_P2IFG1:                                // Start TimerA1 when switch is pressed
        P2IE ^= BIT1;                                // Disable switch interrupt for switch debouncing
        __delay_cycles(5000);                      // Basic switch debouncing delay
        P2IFG = 0x0;                                 // Clear Port2 ISR flags
        start = 1;                                   // Set start flag to 1
        P1OUT |= BIT0;                               // Toggle LED1 on
        TA1CTL |= MC__UP;                            // Set TA1 to start counting UP
        P2IE ^= BIT1;                                // Reenable switch interrupt
        __delay_cycles(5000);                      // Add delay for switch debouncing
      break;
    default:
      break;
  }
}

//********************* Port1 (Switch2) Interrupt Service Routine ****************************
void __SWITCH2_ISR(void) __interrupt [PORT1_VECTOR]
{
  switch(P1IV)   // Switch 2 off
  {
    case P1IV_P1IFG1:                               // Start TimerA1 when switch is pressed
        P1IE ^= BIT1;                               // Disable switch interrupt for switch debouncing
        __delay_cycles(5000);                       // Basic switch debouncing delay
        start = 0;                                  // Set start flag to 0
        time = 0;                                   // Reset the timer variable
        P1OUT &= ~BIT0;                             // Turn off LED1
        P4OUT &= ~BIT7;                             // Turn off LED2
        TA1CTL |= MC__STOP |                        // Set TA1 to STOP
                  TACLR;                            // Clear the timer
        __delay_cycles(5000);                       // Add delay for switch debouncing
        P1IE ^= BIT1;                               // Reenable switch interrupt
      break;
    default:
      break;
  }
}
