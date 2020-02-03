#include <msp430.h>

void LP_init(void)
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

void BT_pins_init(void)
{
  P3DIR |= BIT3 | BIT4; //                                                            //Tx from 5529 to RX of bluetooth module
  P3SEL |= BIT3 | BIT4; // Pin4 = RX, Pin3 = TX on board                       //Rx from 5529 to Tx of bluetooth module

  P4SEL |= BIT4 | BIT5; //Set Port 4, Pins 4 and 5 as USCI RX and TX
  P4DIR |= BIT4 | BIT5; // to be measured for terminal
}

void Flex_pins_init(void)
{
    P1DIR |= BIT0;                                      // Set Port 1 Pin 0 as output (LED1)
    P1OUT &= ~BIT0;                                     // Turn off LED1

    P2DIR |= BIT0 | BIT2;    //** DEBUGGING             // Set Port 2 Pins 0, 2 as output
    P2SEL |= BIT0 | BIT2;    //** DEBUGGING             // Set Port 2 Pins 0, 2 as aux function (TA1.1 | SMCLK)

    P4DIR |= BIT7;                                      // Set Port 4 Pin 7 output (LED2)
    P4OUT &= ~BIT7;                                     // Turn off LED2
    //P4SEL |= BIT4 | BIT5;                             // Set Port 4, Pins 5 and 6 as USCI RX and TX
    P6DIR = 0x00;                                       // Reset P6DIR to value before LP_Init() function changed it
    P6REN |= BIT0 | BIT1 | BIT2 | BIT4 | BIT6;          // Enable pullup/pulldown resistor on Port 6 Pin 6 Pin 0, 1, 2, 4 (A0 - A2, A4).
    P6OUT |= BIT0 | BIT1| BIT2 | BIT4 | BIT6;           // Set pullup/pulldown to pullup resistor on Port 6 Pin 0, 1, 2, 4 (A0 - A2, A4).
}

void Switch_pins_init(void)
{

    P1REN |= BIT1;                                      // Enable pullup/pulldown resistor on Port 2 Pin 1 (Switch1).
    P1OUT |= BIT1;                                      // Set pullup/pulldown to pullup resistor on Port 2 Pin 1 (Switch1).
    P1IFG &= ~BIT1;                                     // Clear interrupt flag on Port 2 Pin 1 (Switch1).
    P1IE  |= BIT1;

    P2REN |= BIT1;                                      // Enable pullup/pulldown resistor on Port 2 Pin 1 (Switch1).
    P2OUT |= BIT1;                                      // Set pullup/pulldown to pullup resistor on Port 2 Pin 1 (Switch1).
    P2IFG &= ~BIT1;                                     // Clear interrupt flag on Port 2 Pin 1 (Switch1).
    P2IE  |= BIT1;                                      // Enable interrupts on Port 2 Pin 1 (Switch1).
}

void TimerA1_init(void)
{
 //*************** TIMER SETTINGS **************************************************
  TA1CTL = TASSEL__ACLK | TACLR | TAIE | MC__STOP; // TA1 source = SMCLK | Clear TA1 timer | Enable timer interrupts | Set TA1 to STOP
  TA1EX0 = TAIDEX_7;                               // Set TA1 second divider to divide by 8
  TA1CCTL1 = OUTMOD_7;                             // PWM output mode: 7 - PWM reset/set
  //TA1CCR0 |= 2049;                                 // Count is set to produce a 2 Hz signal
  TA1CCR0 |= 513;                                 // Count is set to produce a 8 Hz signal
  //TA1CCR1 = 1025;                                  // PWM signal with 50% duty cycle
}

void ADC_12_settings_init(void)
{
 //*************** ADC12_A Settings *************************************************
    // Temperature sensor needs 30 microseconds sampling time
  REFCTL0 = REFMSTR | REFVSEL_2 | REFOUT | REFON;     // REFCTL0 Master control ON | REFCTL0 reference voltage = 2.5 V | REFCTL0 output reference available externally | Turn on reference voltage
  ADC12CTL0 = ADC12SHT0_4 | ADC12MSC | ADC12ON;       // Set ADC12CLK to 4*0x001 (1024) clock cycles | ADC12 Multiple Sample-Conversion | Turn on ADC12_A
  ADC12CTL1 = ADC12SHS_0 | ADC12SHP | ADC12DIV_7 | ADC12SSEL_3 | ADC12CONSEQ_1; // Set Sample/Hold source to ADC12SC | Turn on Sample/Hold Pulse Mode | Divide input clock by 8 | Set clock source to SMCLK | ADC12 Conversion Sequence Select: Sequence-of-channels
  ADC12CTL2 = ADC12RES_2 | ADC12TCOFF;                // Set conversion resolution to 12 bits | Turn off the ADC temperature sensor to save power
  //P6REN |= BIT0;                                      // Enable pullup/pulldown resistor on Port 6 Pin 0 (A0).
  //P6OUT |= BIT0;                                      // Set pullup/pulldown to pullup resistor on Port 6 Pin 0 (A0).
}

void ADC_12_sequence_init(void)
{
    //*************** ADC12_A Sequence Settings *************************************************
  ADC12MCTL0 = ADC12SREF_1 | ADC12INCH_0;    // Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A0
  ADC12MCTL1 = ADC12SREF_1 | ADC12INCH_0;    // Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A0
  ADC12MCTL2 = ADC12SREF_1 | ADC12INCH_0;    // Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A0
  ADC12MCTL3 = ADC12SREF_1 | ADC12INCH_1;    // Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A0
  ADC12MCTL4 = ADC12SREF_1 | ADC12INCH_1;    // Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A1
  ADC12MCTL5 = ADC12SREF_1 | ADC12INCH_1;    // Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A1
  ADC12MCTL6 = ADC12SREF_1 | ADC12INCH_2;    // Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A1
  ADC12MCTL7 = ADC12SREF_1 | ADC12INCH_2;    // Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A1
  ADC12MCTL8 = ADC12SREF_1 | ADC12INCH_2;    // Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A2
  ADC12MCTL9 = ADC12SREF_1 | ADC12INCH_4;    // Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A2
  ADC12MCTL10 = ADC12SREF_1 | ADC12INCH_4;   // Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A2
  ADC12MCTL11 = ADC12SREF_1 | ADC12INCH_4;   // Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A2
  ADC12MCTL12 = ADC12SREF_1 | ADC12INCH_6;   // Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A3
  ADC12MCTL13 = ADC12SREF_1 | ADC12INCH_6;   // Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A3
  ADC12MCTL14 = ADC12SREF_1 | ADC12INCH_6 | ADC12EOS;   // Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A3
  
  //ADC12MCTL15 = ADC12SREF_1 | ADC12INCH_3 | ADC12EOS;  // Set MEM15 as end of sequence | Select reference V(R+) = AVCC and V(R-) = AVSS | Set input channel to A3
  ADC12IE = ADC12IE14;                                 // Enable interrupt when MEM15 is written (EOS)
  ADC12CTL0 |= ADC12ENC;                             // Enable conversion
}

void BT_UART_init(void)
{
  // For the Bluetooth Module//
  UCA0CTL1 |= UCSSEL_2 | UCSWRST;
  UCA0BR0 = 4;                                      // Set baudrate to 115200
  UCA0BR1 = 0;
  UCA0MCTL |= UCBRF_5 + UCBRS_3 + UCOS16;           // oversampling mode
  UCA0CTL1 &= ~UCSWRST;                             // Initialize USCI state machine
  UCA0IE |= UCRXIE;                                 // Enable USCI_A0 RX interrupt
}

void Terminal_UART_init(void)
{
  // To talk with terminal
  UCA1CTL1 |= UCSSEL_2 | UCSWRST;
  UCA1BR0 = 4; // Set baudrate to 115200
  UCA1BR1 = 0;
  UCA1MCTL |= UCBRF_5 + UCBRS_3 + UCOS16; // oversampling mode
  UCA1CTL1 &= ~UCSWRST; // Initialize USCI state machine
  UCA1IE |= UCRXIE; // Enable USCI_A0 RX interrupt
}
