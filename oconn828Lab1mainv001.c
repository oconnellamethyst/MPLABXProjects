/*
 * File:   oconn828Lab1mainv001.c
 * Author: amethyst
 *
 * Created on January 22, 2019, 3:56 PM
 */


#include "xc.h"

// CW1: FLASH CONFIGURATION WORD 1 (see PIC24 Family Reference Manual 24.1)
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
    // Execute once code goes here
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0x9fff;            //sets all pins to digital I/O
    TRISA = 0b0000000000011111;  //set port A to inputs, 
    TRISB = 0b0000000000000011;  //and port B to outputs
    LATA = 0xffff;               //Set all of port A to HIGH
    LATB = 0xffff;               //and all of port B to HIGH

}

/* int main(void)
{
    setup();
    
    LATB = 0x0FFF; //OR FTN
    
    while(1) {
        // Execute repeatedly forever and ever and ever and ever ...
        //LATB = LATB & 0x7FFF; //0x7FFF == 0111 1111 1111 1111 pin 15 AND FTN
        //LATB = LATB & 0xAFFF; // 0xAFFF == 1010 1111 1111 1111 pin 14 and 12, AND FTN
        
        
        //LATB = LATB | 0x8000; //0x8000 == 1000 0000 0000 0000 pin 15 OR FTN
        LATB = LATB | 0x3000; //0x3000 == 0011 0000 0000 0000 pin 12 and 13 OR FTN
    }
    return 0;
}
 */

/*void delay(void) {
    long int curCount = 0;   
 
    while(curCount < 3200000) //int data type has only one byte
    {
        curCount++;
    }
}

int main(void) {
    setup(); 
    LATB = 0x7FFF; //This will turn on the most significant LED
                  
    while(1) {
        // Execute repeatedly forever and ever and ever and ever ...
        
        delay(); //This pauses execution so the shifts are visible 
        LATB = LATB >> 1; //This shifts the LATB register
        LATB |= 0x8000;         
    }
    return 0;
}
*/

void delay(void) {
    long int curCount = 0;   
 
    while(curCount < 3200000) //int data type has only one byte
    {
        curCount++;
    }
}

int main(void) {
    setup(); 
    LATB = 0x7FFF; //This will turn on the most significant LED #initialcondition
    int counting = 0;
    
    while(1){
        // Executes forever
        while(counting < 3) {
            // Executes three times
            delay(); //This pauses execution so the shifts are visible 
            LATB = LATB >> 1; //This shifts the LATB register
            LATB |= 0x8000;   
            counting++;
        }
        counting = 0;
        while(counting < 2) {
            // Executes twice as the last one acts as an initial condition
            delay(); //This pauses execution so the shifts are visible 
            LATB = LATB << 1; //This shifts the LATB register
            LATB |= 0x8000;   
            counting++;
        }
        counting = 0; //Resetting to initial condition
        delay();
        LATB = 0x7FFF; 
    }
    return 0;
}
