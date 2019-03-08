// ****************************************************************************
// disc5.c -- interface to a hexadecimal keypad
//
//  Keypad interface is: RB15 - RB12 --> rows 3 - 0
//                       RA3  - RA0  --> columns 3 - 0
//  Note that we number keys pressed differently from the
// symbols on the 4x4 matrix. We assign numbers 0..3 to the 
// keys in the top row (even though the label on the actual
// keypad is 1, 2, 3, A), 4..7 to the second row and so on.
// We assume that when row 0 and column 0 are shorted, button 0 
// ("0" as in our numbering, and not the label on the key) is
//  pressed, when row 0 and column 1 are shorted, button "1" is pressed, and
//  so on. 
// To display the right figure on the 7-seg LED displays,
// we need a lookup table to translate our number 0 to the symbol
// "1" on the 7-seg display, ..., and number 15 to "d".
// ****************************************************************************
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

#define NOKEY 255

unsigned char scan(unsigned short int row)
{
    static unsigned char LUT[] = {NOKEY,NOKEY,NOKEY,NOKEY,NOKEY,NOKEY,NOKEY,3,
        NOKEY,NOKEY,NOKEY,2,NOKEY,1,0,NOKEY};
    unsigned char key;

    key = PORTA & 15;
    key = LUT[key];
    if (key == NOKEY) return key;
    key += 4*row;
    return  key;
}

// KBMASK1 is used to mask off the bits in PORTB we don't use (RB12-RB0)
#define KBMASK1 0x0fff
unsigned int keyRowEnPattern[4] = {0xe000, 0xd000, 0xb000, 0x7000};

unsigned char lookupTable[16] = {1, 2, 3, 0xA, 4, 5, 6, 0xB, 7,8,9, 0xC,
    0xe, 0, 0xf, 0xD }; // these numbers need to be further translated
                // to the actual a,b,...,g,dp pattern of the 7-seg LED
                // 0xe translates to '*' and 0xf translates to '#'

unsigned char cbuffer[16];
unsigned int front = 0;
unsigned int back = 0;

void put(unsigned char x)
{
    cbuffer[front++] = x;
    front &= 15;
}

unsigned char get(void)
{
    unsigned char x;

    if (front == back) return NOKEY;

    x = cbuffer[back++];
    back &= 15;
    return x;
}

// not to be used directly in their lab, but students can get ideas...
void waitOneMillisecondAndCheckKeypad(void)
{
    static unsigned short int row = 0;
    static unsigned char lastKey[] = {NOKEY,NOKEY,NOKEY,NOKEY};
    static unsigned short int debounce  = 0;
    unsigned char key;

    // If you haven't covered timers yet, the following two lines create a 
    // delay of 1ms
    while (!IFS0bits.T1IF) ;
    IFS0bits.T1IF = 0;
    
    if (debounce)
    {
        debounce--;
        return;
    }
    row++;
    row &= 3;
    LATB = LATB & KBMASK1;
    LATB |= keyRowEnPattern[row];

    key = scan(row);
    if (key != lastKey[row])
    {
        lastKey[row] = key;
        if (key != NOKEY)
        {
            // for nonstandard encoding: put( lookupTable[key]);
            put(key);
        }
        debounce = 40; // to give us 40ms debouncing delay (more than enough)
    }
}

void setup(void)
{
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9fff;  // For digital I/O.  If you want to use analog, you'll
                       // need to change this.

    // add your configuration commands below
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    TRISAbits.TRISA3 = 1;
    CNPU1bits.CN2PUE = 1;
    CNPU1bits.CN3PUE = 1;
    CNPU2bits.CN30PUE = 1;
    CNPU2bits.CN29PUE = 1;

    // note a different method used to set RB12..15 to 1
    // compared to what we did with TRISA0..3. Which one
    // do you like better?
    LATB |= 0xf000;
    TRISB &= 0x0fff;  // set TRISB12..15 to 0 (output)

    // if you haven't covered timers in your lecture, the following 5 lines are
    // gibberish. They setup a "timer" to give us 1ms delay.
    T1CON = 0;
    PR1 = 15999;
    TMR1 = 0;
    IFS0bits.T1IF = 0;
    T1CONbits.TON = 1;
}

int main(int argc, char *argv[])
{
    static unsigned char k;

    setup();

    while (1)
    {
        waitOneMillisecondAndCheckKeypad();
        if ((k = get()) != NOKEY)
        {
            // do something with the character, maybe
            k = k;
        }
    }

    return 0; // never reached (we hope)
}

