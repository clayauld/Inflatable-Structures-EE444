/*
  Clayton Auld
  EE 444 Embedded Systems Design
  Design Project: MSP430F5529 - Flex Sensor Voltage over UART
*/

#include <msp430.h>                                     // Include MSP430 header file
#include <stdio.h>                                      // Enable sprintf function
extern int IncrementVcore(void);                        // Declare prototype for IncrementVcore
extern int DecrementVcore(void);                        // Declare prototype for DecrementVcore

// *********** ADC Calibration Register variables ****************
unsigned int ADC12_1_5V_50C;                            // Variable for first point of ADC12 calibration register (50 C)
unsigned int ADC12_1_5V_85C;                            // Variable for second point of ADC12 calibration register (85 C)

// ************ Variables **************************************
float temp_cal_C = 0.0;                                 // Store calibrated temp in C
float temp_cal_F = 0.0;                                 // Store calibrated temp in F
char string[70];                                        // Set up string for output over UART or LCD
int count = 0;                                          // Set a counter variable
unsigned int i=0;                                       // Another counter variable for USCI TX
unsigned int start = 0;                                 // Flag for UART start or stop input received
char buffer[4];                                         // Buffer for RX data
float time = 0.0;                                       // Counter to calculate time since TEMP received over UART

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    //****************************** Core Voltage Settings ********************************
    IncrementVcore();                                   // Increase the core voltage to next level higher
    IncrementVcore();                                   //

    //************************** UCS Control Register Settings ************************
    UCSCTL1 = DCORSEL_5;                                // Set DCO frequency range

    /* Crystal is 32,768 kHz. To get a 25 MHz clock solve this: 32768*(x-1)=25,000,000
        Remember that the FLL multiplies by n+1 and not n
     */

    UCSCTL2 = 519 | FLLD_0;                             // Set n multiplier for FLL prescaler and proper FLL divider
    UCSCTL3 = SELREF_0;                                 // Set SELREF to XT1CLK
    UCSCTL4 = SELM_3 |                                  // Select DCOCLK as MCLK source
              SELS__DCOCLK;                             // Select XT1 as SMCLK source
    UCSCTL5 = DIVS__1 | DIVM__1;                        // Set SMCLCK divider to 1 and MCLK divider to 1.
    UCSCTL8 ^= SMCLKREQEN;                              // Disable clock request logic for SMCLK

    //*************** TIMER SETTINGS **************************************************
    TA0CTL = TASSEL__ACLK |                             // TA0 source = SMCLK
             TACLR |                                    // Clear TA0 timer
             TAIE |                                     // Enable timer interrupts
             MC__STOP;                                  // Set TA0 to STOP
             ID__8;                                     // Set TA0 first divider to 8
    TA0EX0 = TAIDEX_7;                                  // Set TA0 second divider to divide by 8

    TA0CCTL0 = OUTMOD_7;                                // PWM output mode: 2 - PWM toggle/reset
    TA0CCTL1 = OUTMOD_7;                                // PWM output mode: 2 - PWM toggle/reset

    TA0CCR0 |= 2049;                                    // Count is set to produce a 2 Hz signal
    TA0CCR1 = 1024;                                     // PWM signal with 50% duty cycle

    //*************** ADC12_A Settings *************************************************
    // Temperature sensor needs 30 microseconds sampling time

    // ADC temperature calibration
    ADC12_1_5V_50C = *(unsigned int *)0x01A1A;
    ADC12_1_5V_85C = *(unsigned int *)0x01A1C;

    REFCTL0 = REFMSTR |                                 // REFCTL0 Master control ON
              REFVSEL_0 |                               // REFCTL0 reference voltage = 1.5 V
              REFOUT |                                  // REFCTL0 output reference available externally
              REFON;                                    // Turn on reference voltage

    ADC12CTL0 = ADC12SHT0_6 |                           // Set ADC12CLK to 256 clock cycles
                ADC12MSC |                              // ADC12 Multiple Sample-Conversion
                ADC12ON;                                // Turn on ADC12_A

    ADC12CTL1 = ADC12SHS_0 |                            // Set Sample/Hold source to ADC12SC
                ADC12SHP |                              // Turn on Sample/Hold Pulse Mode
                ADC12DIV_7 |                            // Divide input clock by 8
                ADC12SSEL_3 |                           // Set clock source to SMCLK
                ADC12CONSEQ_1;                          // ADC12 Conversion Sequence Select: Sequence-of-channels

    ADC12CTL2 = ADC12RES_2;                             // Set conversion resolution to 12 bits

    //*************** ADC12_A Sequence Settings *************************************************
    ADC12MCTL0 = ADC12SREF_1 |                           // Select reference V(R+) = AVCC and V(R-) = AVSS
                 ADC12INCH_10;                           // Set input channel to temperature diode

    ADC12MCTL1 = ADC12SREF_1 |                           // Select reference V(R+) = AVCC and V(R-) = AVSS
                 ADC12INCH_10;                           // Set input channel to temperature diode

    ADC12MCTL2 = ADC12SREF_1 |                           // Select reference V(R+) = AVCC and V(R-) = AVSS
                 ADC12INCH_10;                           // Set input channel to temperature diode

    ADC12MCTL3 = ADC12SREF_1 |                           // Select reference V(R+) = AVCC and V(R-) = AVSS
                 ADC12INCH_10;                           // Set input channel to temperature diode

    ADC12MCTL4 = ADC12SREF_1 |                           // Select reference V(R+) = AVCC and V(R-) = AVSS
                 ADC12INCH_10;                           // Set input channel to temperature diode

    ADC12MCTL5 = ADC12SREF_1 |                           // Select reference V(R+) = AVCC and V(R-) = AVSS
                 ADC12INCH_10;                           // Set input channel to temperature diode

    ADC12MCTL6 = ADC12SREF_1 |                           // Select reference V(R+) = AVCC and V(R-) = AVSS
                 ADC12INCH_10;                           // Set input channel to temperature diode

    ADC12MCTL7 = ADC12EOS |                              // Set MEM7 as end of sequence
                 ADC12SREF_1 |                           // Select reference V(R+) = AVCC and V(R-) = AVSS
                 ADC12INCH_10;                           // Set input channel to temperature diode

    ADC12IE = ADC12IE7;                                  // Enable interrupt when MEM7 is written (EOS)

    ADC12CTL0 |= ADC12ENC;                               // Enable conversion

    //************************** UART Settings ****************************************

    UCA1CTL1 |= UCSWRST |                               // USCI Software Reset
                UCSSEL_2;                               // USCI 0 Clock Source: 2 (SMCLK)

    // ************** Baud rate oversampling settings requires some calculations.
    // **********Refer to Raskovic's slides in ...USCI_UART_with_ink.pdf page 51

    UCA1BR0 = 2;                                        // UCBRx = 2; Set low byte to 2
    UCA1BR1 = 0;                                        // UCBRx = 2; Set high byte to 0
    UCA1MCTL |= UCBRF_5 +                               // UCBRFx = 6
                UCBRS_6 +                               // UCBRSx = 5
                UCOS16;                                 // Enable oversampling mode

    // ****************** End baud rate settings*******************************

    UCA1CTL1 &= ~UCSWRST;                               // Start the USCI state machine
    UCA1IE |= UCRXIE;                                   // Enable interrupt on USCI_A1 RX

    //************************** Port Settings ****************************************

    P1DIR |= BIT0 | BIT1 | BIT2;                        // Set Port 1 Pins 0, 1,2 as output (LED1 | TA0.0 | TA0.1)
    P1OUT ^= ~BIT0;                                     // Turn off LED1
    P1SEL |= BIT1 | BIT2;                               // Set Port 1 Pin 0, 1, 2 as special functions (TA0.0 | TA0.1)

    P2DIR |= BIT2;                                      // Set Port 2 Pin 2 as output
    P2SEL |= BIT2;                                      // Set Port 2 Pin 2 as aux function (SMCLK)

    P4DIR |= BIT7;                                      // Set Port 4 Pin 7 output (LED2)
    P4OUT ^= ~BIT7;                                     // Turn off LED2
    P4SEL |= BIT5 | BIT6;                               // Set Port 4, Pins 5 and 6 as USCI RX and TX

    //************************************************************************************


    _EINT();                                            // Globally enable interrupts

    while(1) {}                                       //


    //LPM0;                                               // Enter low-power mode 0; ACLK, SMCLK active; CPU, MCLK disabled;
    //LPM3;                                             // Enter low-power mode 3; ACLK active; CPU, SMCLK, MCLK, FLL disabled;
    //return 0;

}                                                       //



//********************* UART Interrupt Service Routine ****************************

__attribute__((interrupt(USCI_A1_VECTOR))) void __USCI_ISR(void) // USCI ISR
//void USCI_A1_ISR(void) __interrupt [USCI_A1_VECTOR]
{
    switch(UCA1IV)
    {
        case 0: break;                                  // Vector 0 - no interrupt
        case 2:                                         // Vector 2 - RXIFG
            i=0;                                        //
            string[0] = UCA1RXBUF;                      //
            string[1] = 0x0;
            string[2] = 0x0;
            UCA1IE |= UCTXIE;                           // Enable interrupt on USCI_A1 TX
            while (!(UCA0IFG & UCTXIFG));               // USCI_A1 TX buffer ready?
            UCA1TXBUF = string[i++];                    // Echo character received
            count++;                                    // Add up the number of received characters

//******************************* Check for characters of interest and process them accordingly *****************************************************
            // Create a buffer array to hold last 4 characters received.
            // This can be thought of as a software shift register for 8 bit characters
            buffer[3] = buffer[2];
            buffer[2] = buffer[1];
            buffer[1] = buffer[0];
            buffer[0] = UCA1RXBUF;

            if (UCA1RXBUF == 0x00d)                     // Return key pressed
            {
                i=0;                                    //
                if (buffer[1] == 0x00d)
                    sprintf(string, "Characters Entered: %d\n\r%c", count, 0x0);
                else sprintf(string, "\nCharacters Entered: %d\n\r%c", count, 0x0);       //
                UCA1IE |= UCTXIE;                       // Enable interrupt on USCI_A1 TX
                while (!(UCA0IFG & UCTXIFG));           // USCI_A1 TX buffer ready?
                UCA1TXBUF = string[i++];                //
                count--;
            }

            else if (buffer[3] == 0x54 /* T */ && buffer[2] == 0x45 /* E */ && buffer[1] == 0x4D /* M */ && buffer[0] == 0x50 /* P */)
            {
                start = 1;                              // Set start flag to 1
                P1OUT ^= BIT0;                          // Toggle Port 1 Pin 0
                P2IFG &= ~BIT6;                         // Clear interrupt on Port 2 Pin 6
                TA0CTL |= MC__UP;                       // Set TA0 to start counting UP
            }

            else if (buffer[3] == 0x53 /* S */ && buffer[2] == 0x54 /* T */ && buffer[1] == 0x4F /* O */ && buffer[0] == 0x50 /* P */)
            {
                start = 0;                              // Set start flag to 0
                time = 0;                               // Reset the timer variable
                P1OUT ^= BIT0;                          // Toggle Port 1 Pin 0
                P2IFG &= ~BIT6;                         // Clear interrupt on Port 2 Pin 6
                TA0CTL |= MC__STOP;                     // Set TA0 to STOP
            }
            break;
//***************************************** End checking for characters of interest *******************************************

        case 4:                                         // Vector 4 - TXIFG
            UCA1TXBUF = string[i++];                    // Send next character
            if (i >= sizeof(string) || string[i] == 0x0)// Detect null character or end of string
            {
                UCA1IE &= ~UCTXIE;                      // Disable interrupt on USCI_A1 TX
            }
            break;                                      //
        default: break;                                 //
    }
}
//********************* ADC12 Interrupt Service Routine ****************************

//void __ADC12_ISR(void) __interrupt [ADC12_VECTOR]     // ADC12 ISR
__attribute__((interrupt(ADC12_VECTOR))) void __ADC12_ISR(void) // ADC12 ISR
{
    float temp0=0;                                      //
    int temp1=0;                                        //
    int counter;                                        //
    float time1 = 0.0;                                  // Local variable for time calculation
    switch(ADC12IV)                                     //
    {
        case ADC12IV_ADC12IFG7:                         // Vector 20: ADC12IFG7
            ADC12IFG &= ~ADC12IFG7;                     // Clear interrupt flag
            for (counter=0; counter<8; counter++)       //
            {
                temp1 += *(&ADC12MEM0 + counter);       //
            }
            temp0 = temp1/8;                            //
            temp_cal_C = ((temp0 - ADC12_1_5V_50C)*((85.0-30.0)/(ADC12_1_5V_85C-ADC12_1_5V_50C)))+30.0;
            temp_cal_F = temp_cal_C * 1.8 + 32;         //
            time1 = time * 0.5;                         // Calculate number of seconds since TEMP
            sprintf(string, "Chip Temperature: %.2f C = %.2f F\n\rTime since TEMP: %.1f seconds\n\r", temp_cal_C, temp_cal_F, time1);
            while (!(UCA0IFG & UCTXIFG));               // USCI_A1 TX buffer ready?
            i=0;                                        //
            UCA1IE |= UCTXIE;                           // Enable interrupt on USCI_A1 TX
            UCA1TXBUF = string[i++];                    //
            break;
        default: break;
    }
}

//********************* ADC12 Interrupt Service Routine ****************************
__attribute__((interrupt(TIMER0_A1_VECTOR))) void __T0A1_ISR(void)  // Timer0_A1 ISR
//void __T0A1_ISR(void) __interrupt [TIMER0_A1_VECTOR]
{
    //TA0IV = 0;                                        // Reset all TimerA interrupt flags
    switch(TA0IV)
    {
    case 14:
      if (start == 1)
      {
          time++;                                       // Iterate the time variable
          ADC12CTL0 |= ADC12SC;                         // Start ADC sample-and-conversion
          P1OUT ^= BIT1;                                // Toggle Port 1 Pin 1
      }
    }
}
//***********************************************************************************
