/*
 * File:   feng_lab1_main_v001.c
 * Author: Fengj
 *
 * Created on January 28, 2019, 7:10 PM
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
     CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
     
     AD1PCFG = 0x9fff;            //sets all pins to digital I/O
     TRISA = 0b0000000000011111;  //set port A to inputs, 
     TRISB = 0b0000000000000011;  //and port B to outputs
     LATA = 0xffff;               //Set all of port A to HIGH
     LATB = 0x0fff;               //and all of port B to HIGH


}

void delay(void) {
    int curCount = 0;
    int countFlow = 0;
    while(countFlow<20)
    {
        curCount++;
        if(curCount > 20000){
            countFlow++;
            curCount = 0;
        }
        
    }
}



int main(void) {
    setup();
    LATB = 0x7FFF; //This will turn on the most significant LED
    
    int count = 0;
    while(1) {
        
        // Execute repeatedly forever and ever and ever and ever ...
        if (count == 4){
            int k = 0;
            while(k<4){
                delay();
                 LATB = LATB << 1;
                 LATB |= 0x0001;
                 k++;
                 
            }
            
            count=0;
        }
        
        else{
           delay();
        LATB = LATB >> 1;
        LATB |= 0x8000;
        count++;
        }
    }
    return 0;
}
