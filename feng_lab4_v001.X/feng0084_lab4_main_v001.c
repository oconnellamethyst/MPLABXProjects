/*
 * File:   feng0084_lab4_main_v001.c
 * Author: Fengj
 *
 * Created on March 12, 2019, 12:34 AM
 */


#include <p24FJ64GA002.h>

#include "xc.h"

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

volatile int rollover = 0;

void __attribute__((interrupt,auto_psv)) _T2Interrupt(){
    IFS0bits.T2IF = 0;
    rollover ++;
}


//Starting from Discussion 6 code
void __attribute__((__interrupt__,__auto_psv__)) _IC1Interrupt(void) //Takes care of what we do
{
    _INT0IF = 0;

    if (_INT0EP == 0){   // I was waiting for a rising edge
        TMR1 = 0;
        overflow = 0;
    } else { // I was waiting for a falling edge
        numCycles = TMR1 + 16000L * overflow; // What is this? how many cycles? 16000 is one ms, number of cycles in ms. That L is long int, which we need cause that multiplication is going to be bigger than 65535. 
//        TMR1 = 0;         // uncomment these two lines if you want the period
//        overflow = 0;     //       and not just active duty cycle. also
                            //       delete the if part, and the last line
                            //       that changes polarity of INT0EP
    }
    
    if (numCycles>=2){
        //do debouncing things?
    }

    _INT0EP = 1 - _INT0EP; //BTG... or XOR = 1, basically we're flipping this.
}


void setup(void)
{
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0xFFFF;
    
}


void initServo(void) {
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
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
void setServo(int Val) {
    OC1RS = Val;
}


void delay(int delay_in_ms){            
    int d;
    for(d=0;d<delay_in_ms;d++){
        ms_wait();                  // call the millisecond delay function delay_in_ms times
    }
}

void initPushButton(void) {
    /*
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    RPINR7bits.IC1R = 8; // Use Pin RP8 = "8", for Input Capture 1 (Table 10-2)
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock PPS
    
    
    T2CON = 0;
    T2CONbits.TCKPS =0b11;
    TMR2 = 0; // Initialize to zero (also best practice)
    PR2 = 62499;
    
   
    IC1CON = 0; // Turn off and reset internal state of IC1
    IC1CONbits.ICTMR = 0b001; // Use Timer 2 for capture source
    IC1CONbits.ICM = 0b011; // Capture raising edges
    IC1CONbits.ICI = 0b00; //Interrupt every capture event
    IFS0bits.IC1IF = 0;
    
    */
    TRISBbits.TRISB8 = 1;
    CNPU2bits.CN22PUE = 1;
    
    
    //set flag to zero, enable interrupt
    //IEC0bits.IC1IE = 0;
    
   
    
     
    T2CON = 0;
    T2CONbits.TCKPS =0b11;
    TMR2 = 0; // Initialize to zero (also best practice)
    PR2 = 62499;
    
    IFS0bits.T2IF = 0;
    IEC0bits.T2IE = 1;
    T2CONbits.TON = 1; // Restart 16-bit Timer2
    

}

int main(void) {
    long int currentTime = 0;
    long int prev = 0;
    int press = 0; //count the how many times the button has been pressed, for testing button purpose only
    setup();
    initServo();
    initPushButton();
   setServo(1000);//initial several to the right
    while(1){
        //while(IFS0bits.IC1IF == 0);
        //IFS0bits.IC1IF = 0;
        while(PORTBbits.RB8 == 1);// idle value of RB8 is 1, when button pressed, it will go 0. 
        press++; // increment press.
      
    
           prev = currentTime;//update previous button pressed time
            currentTime =(TMR2 + rollover*62500)/1000; //record current button pressed time
            TMR2 = 0;
            rollover = 0;
            if((currentTime-prev)>10){ //if this is not double click, set currentTime to -1, so that it won't trigger next if loop. 
            currentTime = -1;
            }
            
            
        
        
        if((currentTime-prev) < 10 && currentTime>0){//if it is double click. do following. 
            setServo(4500);
            delay(2000);
            setServo(1000);
        }
       
        
        
        delay(2);
        
        
    }
    return 0;
}
