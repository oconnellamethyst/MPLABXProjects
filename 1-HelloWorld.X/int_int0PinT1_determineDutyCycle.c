#include <p24Fxxxx.h>
#include <xc.h>

// PIC24FJ64GA002 Configuration Bit Settings
// 'C' source line config statements

// CONFIG2
#pragma config POSCMOD = NONE           // Primary Oscillator Select (Primary oscillator disabled)
#pragma config I2C1SEL = PRI            // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF            // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = OFF           // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as CLKO (FOSC/2))
#pragma config FCKSM = CSECME           // Clock Switching and Monitor (Clock switching is enabled, Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL           // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))
#pragma config SOSCSEL = SOSC           // Sec Oscillator Select (Default Secondary Oscillator (SOSC))
#pragma config WUTSEL = LEG             // Wake-up timer Select (Legacy Wake-up Timer)
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = OFF             // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config ICS = PGx1               // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

// This program measures the duty cycle (i.e., duration of a signal being 1) in
// terms of num cycles.
// INT0 pin has the external sig. T1 counts #cycles.
// Both programmed as interrupts.

volatile unsigned int overflow = 0;
volatile unsigned long int numCycles = 0;  // duty cycle in terms of # TCY

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
    } else { // I was wainting for a falling edge
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
    AD1PCFG = 0x9fff;
    LATBbits.LATB7 = 1;

    T1CON = 0;
    PR1 = 15999;
    TMR1 = 0;
    T1CONbits.TON = 1;

    IFS0bits.T1IF = 0;
    //IPC0bits.T1IP = 4;
    /* enable Timer 1 interrupt */
    IEC0bits.T1IE = 1;

    _INT0EP = 0;
    _INT0IF = 0;
    _INT0IE = 1;

}


int main(void)
{
    unsigned long int count = 0;

    setup();

    while (1) {
        count++;
    }
}
