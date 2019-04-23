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

volatile unsigned char buffer[64];
volatile unsigned char front = 0;
volatile unsigned char back = 0;

void __attribute__((__interrupt__,__auto_psv__)) _U1RXInterrupt(void)
{
    IFS0bits.U1RXIF = 0;
    buffer[front++] = U1RXREG;
    front &= 63;
}

void PPutch(const unsigned char c)
{
    while (!IFS0bits.U1TXIF) ; // I'm free to use, there is room in my buffer to be used, Always, before writing to the TX reg, make sure it's empty
    IFS0bits.U1TXIF = 0;
    U1TXREG = c;
}

unsigned char PGetch(void)
{
    unsigned char x;

    while (front == back) ;
    x = buffer[back++];
    back &= 63;
    return x;
}

void setup(void)
{
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0x9fff;  // For digital I/O.
    
    // I think the following two lines are irrelevant. The UART doc
    // Page 3, when describing the UARTEN bit in UxMODE says that when UARTEN
    // is set, TRISx are ignored and instead UEN and UTXEN control pins.
    _TRISB6 = 0;  // U1TX output
    _TRISB10 = 1; // U1RX input

    U1MODE = 0;  // UEN<1:0> bits control the pins
    // U1BRG = 34; // 115200 baud,
    // U1MODEbits.BRGH = 1;
    U1MODEbits.BRGH = 0;
    U1BRG = 25; // 38400 baud (check the calculation with the datasheet)
    U1MODEbits.UEN = 0;
    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN = 1;

    // Peripheral Pin Select 
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    _RP6R = 0x0003;   //RB6->UART1:U1TX; See Table 10-3 on P109 of the datasheet
    _U1RXR = 10;   //RB10->UART1:U1RX;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS

    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;
}

int main(int argc, char *argv[])
{

    setup();

    int i;

    // waste some time
    for (i = 0; i < 30000; i++) i = i;

    // transmit data on UART
    //for (i = ' '; i < 0x7f; i++)
    //    PPutch(i);
    //PPutch(0x0D);  // new line characters
    //PPutch(0x0A);

    while (1)
    {
        unsigned int x;

        x = PGetch();
        if(( 'A' <= x) && ('W' >= x )){ //If it's between these 23 letters it rolls forward three
            // If you wanted any number, you'd do something like ('A' <= x ) && (('A' + 26 - n) >= x)
            x += 3;
        }
        else if((x >= 'X') && (x <= 'Z')){ //But for these 3 it has to roll backwards b/c ASCII is not a circle
            x -= 23;
        }
        PPutch(x);
    }

    return 0; // never reached (we hope)
}
