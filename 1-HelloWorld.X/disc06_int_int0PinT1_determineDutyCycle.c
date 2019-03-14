#include <p24Fxxxx.h>
#include <xc.h>

// PIC24FJ64GA002 Configuration Bit Settings
// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled. 
					// Primary Oscillator refers to an external osc connected to the OSC1 and OSC2 pins)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // OSC2/CLKO/RC15 functions as port I/O (RC15)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

// This program measures the duty cycle (i.e., duration of a signal being 1) in
// terms of the num of cycles.
// INT0 pin is connected to the external signal that we want to measure.
// T1 counts the #cycles.
// Both INT0 and T1 are programmed as interrupts.
// The interrupt for T1 is triggered when TMR1==PR1, and as a result _T1IF==1. 
// If a variable counts at every T1 interrupt, the count value would be the
// number of times TMR1 rolled over.
// The interrupt for INT0 happens when the external signal changes polarity
// (you can program INT0 to respond to rising edge or falling edge).

volatile unsigned int overflow = 0;   // # of times TMR1 rolled over
volatile unsigned long int numCycles = 0;  // duty cycle of the external signal
                                           // in terms of # TCY

void __attribute__((__interrupt__,__auto_psv__)) _T1Interrupt(void)
{
    IFS0bits.T1IF = 0;
    overflow++;
}

void __attribute__((__interrupt__,__auto_psv__)) _INT0Interrupt(void)
{
    _INT0IF = 0;

    if (_INT0EP == 0){   // I was waiting for a rising edge
        TMR1 = 0;
        overflow = 0;
    } else { // I was waiting for a falling edge
        numCycles = TMR1 + 16000L * overflow;
//        TMR1 = 0;         // uncomment these two lines if you want the period
//        overflow = 0;     //       and not just active duty cycle. also
                            //       delete the if part, and the last line
                            //       that changes polarity of INT0EP
    }

    _INT0EP = 1 - _INT0EP;
}

void setup(void)
{
    CLKDIVbits.RCDIV = 0; // make 16MHz
    // setup INT0 (RB7) pin as input
    AD1PCFG = 0x9fff;  // make all pins digital, not analog.
    TRISBbits.TRISB7 = 1; // make RB7 an input pin. RB7 and INT0 pin are multiplexed.

    T1CON = 0;    // turn off the timer for now, set PRE=1, TCY as input clock.
    PR1 = 15999;  // could be any number. The larger the better.
    TMR1 = 0;
    T1CONbits.TON = 1;

    IFS0bits.T1IF = 0;
    //IPC0bits.T1IP = 4;
    // enable Timer 1 interrupt. If you forget the next line, _T1Interrupt is never called
    // even if there are _T1IF events.
    IEC0bits.T1IE = 1;

    _INT0EP = 0;  // edge polarity set to detection of positive edge.
    _INT0IF = 0;  // reset so that you don't respond to a previously generated event.
    _INT0IE = 1;  // enable interrupt for INT0 events.
}


int main(void)
{
    unsigned long int count = 0;

    setup();

    while (1) {
        count++;  // dummy instruction... 
    }
}
