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

/*
 This program generates 50ms delays between LATB0 toggles.
 * Does it using four different ways:
 * 1- Timer 1 counts 50 iterations of 1ms delays
 * 2- Timer 2 uses a prescalar of 64 to get 50ms directly
 * 3- Timer 1 uses interrupts and counts 50 occurances of the int
 * 4- Timer 2 uses an interrupt that is triggered every 50ms

 * Main by default first does polling of T1IF inside a loop of 50,
 * then does a polling of T2IF, and then enables T1IE.
 * You can comment out T1IE=1 and uncomment the line that enables T2IE
 * and simulate the code agian.
 * 
 * IF you want to measure how long the delay is between LATB0 flippings,
 * put breakpoints on where LATB0 changes (e.g., LATB=0, LATB=1,
 * asm("btg  LATB, #0);
 *  
 */

void __attribute__((__interrupt__,__auto_psv__)) _T1Interrupt(void)
{
    static int count = 0;
    
    _T1IF = 0;
    count++;
    if (count == 50) {
        asm ("btg  LATB, #0");
        count = 0;
    }
}


void __attribute__((__interrupt__,__auto_psv__)) _T2Interrupt(void)
{
    IFS0bits.T2IF = 0;  // could have used _T2IF
    asm ("btg  LATB, #0");
}


void setup(void) {
    CLKDIVbits.RCDIV = 0;

    AD1PCFG = 0x9fff; // all digital
    TRISB = 0;
    LATB = 0;
    T1CON = 0; // turn off timer, prescalar 1:1, use Tcy as clk source
    PR1 = 15999;
    TMR1 = 0;
    _T1IF = 0; // equivalent to IFS0bits.T1IF = 0;
    T1CONbits.TON = 1; // now turn on the timer
    
    T2CON = 0x0020; // turn off, 1:64 pre, Tcy clock source
    PR2 = 12499;  // combined with 1:64 ==> 800,000, which is 50ms
    TMR2 = 0;
    _T2IF = 0;
    T2CONbits.TON = 1;
}

int main(void) {
    int i, j;
    setup();

    // test polling T1
    LATB = 0;
    TMR1 = 0;
    for (j=0 ; j<5 ; j++) {
        for (i=0 ; i<50 ; i++) {
            while (!_T1IF);
            _T1IF = 0;
        }
        LATB = 1;
    }
    
    // test polling T2
    LATB = 0;
    TMR2 = 0;
    _T2IF = 0;
    for (j=0 ; j<5 ; j++) {
        while (_T2IF == 0);
        _T2IF = 0;
        LATB = 1;
    }
    
    // Enable only one of the T1 / T2 interrupts below. Comment out
    // the enabling of the other one.
    
    // now enable T1 interrupt
  //  _T1IE = 1; // equiv to IEC0.T1IE=1;
    
    // enable T2 interrupt
   IEC0bits.T2IE = 1;  // equiv to _T2IE = 1
    
    while (1) {
   }
}