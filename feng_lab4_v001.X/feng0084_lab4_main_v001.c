/*
Date: March 12, 2019
Name: Junchi Feng and Amethyst O'Connell
Student ID number: [insert] and 5225858
Course number: EE 2361
Term: Spring 2019
Lab/assignment number:
Short Program Description: Program for lab 4, moves a servo one direction when a button is pressed and the other direction when the button is pressed twice quickly 

 */


#include <p24FJ64GA002.h>

#include "xc.h"
#include<stdbool.h>
#include "feng_lab2b_asmLib_v001.h"

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


void setServo(int Val) {
// Function: setServo
//  ---------------------------
// This function does one thing, it sets the output compare one register to a value, this value represents the location/angle that the servo motor will move to. It will probably break if too big of an angle is put in, but it might also spin multiple times, I also believe it will break if a negative value is placed in it. Since it writes to OC1RS, it has the limitations of that register.
    OC1RS = Val;
}


void delay(int delay_in_ms){            
    int d;
    for(d=0;d<delay_in_ms;d++){
        ms_wait();                  // call the millisecond delay function delay_in_ms times
    }
}




volatile int rollover = 0;

unsigned long int duration[2] = {0,0};
unsigned long int period = 0;
unsigned long int garbage = 0;

unsigned long int start = 0;
unsigned long int stop = 0;


void __attribute__((interrupt,auto_psv)) _IC1Interrupt(){
        IFS0bits.IC1IF = 0; 
        
        
        start = stop;
        stop = IC1BUF + rollover*62500;
        period = ((stop - start) )/1000; // divide 1000, convert to us. 
        rollover = 0;
        TMR2 = 0;
        IFS0bits.T2IF = 0;
        while(_ICBNE){
            garbage = IC1BUF;
        }
        
         duration[1] = duration[0];
         duration[0] = period;
        
         
            if(period < 25){
                setServo(4500);
                delay(1000);
                
                setServo(1000);
            
            }
     
}

    
   

void __attribute__((interrupt,auto_psv)) _T2Interrupt(){
    IFS0bits.T2IF = 0;
    rollover ++;
}
void setup(void)
{
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0xFFFF;
    
}


void initServo(void) {
// Function: initServo
//  ---------------------------
// There should not be any arguments. This function requires the use of the Timer 3 peripheral. This function sets up the servo motor by setting up the timer for use in PWM for the servo motor. It does so by setting up the pins, pin 6 to send out the PWM needed for the motor, and pin 8 to take in the button and set up timer 3.
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR7bits.IC1R = 8; // Use Pin RP8 = "8", for Input Capture 1 (Table 10-2)
    RPOR3bits.RP6R = 18; // Use Pin RP6 for Output Compare 1 = "18" (Table 10-3)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
    
     
     TRISBbits.TRISB6 = 0;
    
    
    //Configure Timer 3 (500ns/count, 25ms max).
    // note that resolution = 500ns = 8 x 62.5ns = 0.0005ms, max period = 25ms = Tcy*8*50,000
    T3CON = 0x0010; //Stop Timer, Tcy clk source, PRE 1:8
    TMR3 = 0; // Initialize to zero (also best practice)
    PR3 = 39999; // Set period to be larger than max external sig duration
    T3CONbits.TON = 1; // Restart 16-bit Timer3
    
    OC1CON = 0; // turn off OC1 for now
    OC1R = 3000;
    OC1RS = 3000;
    OC1CONbits.OCTSEL = 1; // Use Timer 3 for compare source
    OC1CONbits.OCM = 0b110; // Output compare PWM w/o faults
   
}

void initPushButton(void) {
// Function: initPushButton
//  ---------------------------
// This function sets up the button and takes no arguments. This sets up both timer 1 and timer 2 to be used with interrupts in order to be able to sense when the button is pressed. 


   
    CNPU2bits.CN22PUE = 1;
    
    
     
    T2CON = 0;
    T2CONbits.TCKPS =0b11;
    TMR2 = 0; // Initialize to zero (also best practice)
    PR2 = 62499;
    
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    T2CONbits.TON = 1; // Restart 16-bit Timer2
    
    T1CON = 0x0010; //Stop Timer, Tcy clk source, PRE 1:8
    TMR1 = 0; // Initialize to zero (also best practice)
    PR1 = 39999; // Set period to be larger than max external sig duration
    T1CONbits.TON = 1; // Restart 16-bit Timer1
    
    
    IC1CON = 0; // Turn off and reset internal state of IC1
    IC1CONbits.ICTMR = 1; // Use Timer 2 for capture source
    IC1CONbits.ICM = 0b001; // Turn on and capture every rising edge
    
    IC1CONbits.ICI = 0;
    
    AD1PCFG = 0xFFFF;
    TRISBbits.TRISB8 = 1;
    //set flag to zero, enable interrupt
    IEC0bits.IC1IE = 1;
    IFS0bits.IC1IF = 0;
    CNPU2bits.CN22PUE = 1;
    IEC0bits.T2IE = 1;
    IFS0bits.T2IF = 0;
    IFS0bits.T1IF = 0;

}

int main(void) {
  
    setup();
    initServo();
    initPushButton();
   setServo(1000);//initial several to the right
   TRISBbits.TRISB5 = 0;
    while(1){
        LATBbits.LATB5 = 1; 
    }
    
        
     
        
    
    return 0;
}
