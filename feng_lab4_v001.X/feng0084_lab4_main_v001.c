/*
 * File:   feng0084_lab4_main_v001.c
 * Author: Fengj
 *
 * Created on March 12, 2019, 12:34 AM
 */


#include <p24FJ64GA002.h>

#include "xc.h"



#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)


// CW2: FLASH CONFIGURATION WORD 2 (see PIC24 Family Reference Manual 24.1)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
                                       // Fail-Safe Clock Monitor is enabled)
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))

void setup(void)
{
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
   
    
}


void initServo(void) {
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPOR3bits.RP6R = 18; // Use Pin RP6 for Output Compare 1 = "18" (Table 10-3)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
    
     AD1PCFG = 0xFFFF;
     TRISBbits.TRISB6 = 0;
    
    
    //Configure Timer 3 (500ns/count, 25ms max).
    // note that resolution = 500ns = 8 x 62.5ns = 0.0005ms, max period = 25ms = Tcy*8*50,000
    T3CON = 0x0010; //Stop Timer, Tcy clk source, PRE 1:8
    TMR3 = 0; // Initialize to zero (also best practice)
    PR3 = 39999; // Set period to be larger than max external sig duration
    T3CONbits.TON = 1; // Restart 16-bit Timer3
   
    
    OC1CON = 0; // turn off OC1 for now
    OC1R = 3000; //changed to 3K
    OC1RS =3000; //change to 3k
    OC1CONbits.OCTSEL = 1; // Use Timer 3 for compare source
    OC1CONbits.OCM = 0b110; // Output compare PWM w/o faults
   
}
void setServo(int Val) {
    OC1RS = Val;
}


void delay(int delay_in_ms){            
    int d;
    for(d=0;d<delay_in_ms;d++){
        ms_wait();                  // call the millisecond delay function delay_in_ms times
    }
}

int main(void) {
    setup();
    initServo();
    while(1){
    setServo(3600); // changed to 3600
    delay(1000);
    setServo(1000); // change to 2400
    delay(1000);
    }
    return 0;
}
