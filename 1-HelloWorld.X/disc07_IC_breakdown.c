#include <p24Fxxxx.h>
#include <xc.h>
#include <stdlib.h>

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
 This code shows how polling IC1 and T2 overflows can fail.
IC1 is setup to react on any edge. Base timer is T2. The code is supposed to
calculate the width of a low-going pulse that goes low around 20K Tcy and high
around 35K (hence pulse width = 15K). T2 is setup to rollover in 10K.

Works for:
   pulse: @ 80 IC1=1, @ 20,100 IC1=0, @ 35,100 IC1=1

Fails: (not always. depends on compiler version)
  @ 80 IC1=1, @ 20,086 IC1=0, @ 35,086 IC1=1       gives a pulse width of 25k
                                                   instead of 15k

The reason is that in the first while loop, the T2 rollover happens just barely
after the IC1 event, so the "if T2IF" condition is not triggered (hence no
rollover++), and TMR2 holds a small value (e.g., 8 as opposed to ~9.9K).
The second while loop waiting for the rising edge then catches that T2IF (that
really didn't match the start=TMR2 value after the loop), and another T2IF
event, hence assuming > 20K was spent during the low pulse.

 To better understand what's going on, consider the following examples:
 1) after the first while loop, overflow=1, start=IC1BUF=9.8K. This means that
    T2IF has not happened for a second time at the time the code reaches start=
    IC1BUF. The second while loop is going to catch a rollover early on,
    and another one around 30K, ending with IC1BUF ~= 4.8K.
    So start stamp = 10K + 9.8K, end time stamp = 4.8K + 30K and a difference
    of 15K.
 
 2) After the first while loop, overflow=2, start=IC1BUF=200. This means that
    T2 overflow happened long enough before the IC1IF event that the "if" 
    statement inside the while loop had time to ++ rollover for a second time
    well before the IC1 edge that ended the while loop. Start time stamp = 200 +
    20k. The second while loop is only going to do one rollover at around 30K.
    It emerges with end time stamp = 5.2K + 30K. The subtraction is 15K.
 
 3) This scenario is similar to scenario #1, except that the T2 rollover happens
    right before the IC1IF event, but after the if statement from the previous
    iteration of the while loop had fetched the value of the IFS0.
 *        Remember, checking to see if T2IF is set requires copying the value
 *        of IFS0 to a register, say, W5, and then masking it to get the T2IF
 *        bit, checking to see if the masked val is zero, and then doing a
 *        conditional branch. So if T2IF changes right after the move
 *        instruction IFS0 --> W5, then the if statement will not catch it in
 *        that while iteration.
    Going back to Scenario 3, the "if" condition inside the while loop has not
    had a chance to ++ the rollover variable for a second time, and when the
    code emerges from the first while loop, start=IC1BUF=5.
    Hence the start time stamp = 10K + 5    (but 5 happened because T2 did a
    second rollover, but we didn't ++ the rollover var).
    The second while loop is going to immediately catch the T2IF event, incrementing
    the rollover variable to 2. Another rollover increment is going to happen
    around 30K. The code emerges from the second while loop with IC1BUF=5005.
    The end time stamp = 5005 + 30K, and the difference is 25K.

 */

void setup(void)
{
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9fff;  // For digital I/O.  If you want to use analog, you'll
                       // need to change this.

    // add your configuration commands below
    PORTB = 0xffff;
    TRISB = 0x7fff; // bit 15 is an output

    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    _IC1R = 2 ;   //IC! <- RB2; See Table 10-2 on P108 of the datasheet
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
    TMR2 = 0;

    T2CON = 0;
    PR2 = 9999;
    T2CONbits.TON = 1;
    TMR2 = 0;
    
    IC1CON = 0;
    IC1CONbits.ICTMR = 1; // timer 2
    IC1CONbits.ICM = 1;   // capture on every edge

    PR1=65000;      // Timer 1 setup for debugging purposes.
    T1CON=0;
    TMR1 = 104;
    T1CONbits.TON = 1;
    _T1IF = 0;
}

int main(int argc, char *argv[])
{
    unsigned short int rollovers, rolloversStart;
    unsigned short int start;
    unsigned short int stop;
    unsigned long int interval;
    int putABreakPointHere=0;
    
    setup();

    while (1)
    {
        IFS0bits.IC1IF = 0;
        rollovers = 0;
        while (!IFS0bits.IC1IF)
        {
            if (IFS0bits.T2IF)
            {
                rollovers++;
                IFS0bits.T2IF = 0;
            }
        }
        start = IC1BUF; // takes a few cycles

        if (IFS0bits.T2IF)
        {
            rollovers++;
            IFS0bits.T2IF = 0;
        }

        rolloversStart = rollovers; //give the start time  ----____----____, rollovers increment roll, capture stop, tmr value, simulus in IDE,
        IFS0bits.IC1IF = 0;
        while (!IFS0bits.IC1IF)
        {
            if (IFS0bits.T2IF)
            {
                rollovers++;
                IFS0bits.T2IF = 0;
            }
        }
        stop = IC1BUF;
        interval = rollovers - rolloversStart;
        interval *= 10000;
        interval += stop;
        interval -= start;
        putABreakPointHere++;             // interval is supposed to be 15,000
    }

    return 0; // never reached (we hope)
}
