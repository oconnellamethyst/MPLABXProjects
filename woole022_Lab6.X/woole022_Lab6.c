#include "xc.h"
#include <stdio.h> //for sprintf
#include "Display6.h"
#include "woole022_Lab6.h"

#pragma config ICS = PGx1          // Comm Channel Select (Emulator EMUC1/EMUD1 pins are shared with PGC1/PGD1)
#pragma config FWDTEN = OFF        // Watchdog Timer Enable (Watchdog Timer is disabled)
#pragma config GWRP = OFF          // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF           // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF        // JTAG Port Enable (JTAG port is disabled)
#pragma config I2C1SEL = PRI       // I2C1 Pin Location Select (Use default SCL1/SDA1 pins)
#pragma config IOL1WAY = OFF       // IOLOCK Protection (IOLOCK may be changed via unlocking seq)
#pragma config OSCIOFNC = ON       // Primary Oscillator I/O Function (CLKO/RC15 functions as I/O pin)
#pragma config FCKSM = CSECME      // Clock Switching and Monitor (Clock switching is enabled, 
#pragma config FNOSC = FRCPLL      // Oscillator Select (Fast RC Oscillator with PLL module (FRCPLL))


int setup(void){
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0xFFFE; //everything digital except AN0
    
    //LCD stuff
    TRISA = 0b1;  //set RA0 for output to LED
    TRISB = 0;  //set RB8 to output
    I2C2CON = 0; //clears
    I2C2CONbits.I2CEN = 0;
    I2C2BRG = 157; //Baud rate generator
    _MI2C2IF = 0;
    I2C2CONbits.I2CEN = 1;
    
    //Analog Stuff
    initBuffer();   //Sets all values to 0
    AD1CON1bits.SSRC = 0b010; //sets to timer 3
    AD1CON1bits.ASAM = 1; // turns on auto sampling
    AD1CON2 = 0x0000; //read every one (1st buffer))
    AD1CON3 = 0x0100;
    AD1CON3bits.ADCS = 1;
    IFS0bits.AD1IF = 0;
    IEC0bits.AD1IE = 1;  
    PORTB = 0xffff;     
    
    //Timer 3 stuff 
    T3CONbits.TCKPS0 = 0; //1:64
    T3CONbits.TCKPS1 = 1;
    TMR3 = 0;
    PR3 = 15625; //Once ever 1/16th of a second.
    T3CONbits.TON = 1;
   
    AD1CON1bits.ADON = 1;
    lcd_init();
    return 0;
}

unsigned long int adValue;
char adStr[50];

void __attribute__((interrupt, auto_psv)) _ADC1Interrupt(){
    IFS0bits.AD1IF = 0; //read voltage from A0 for 16 times a second
    adValue = ADC1BUF0;
}
    
int main(void){
    setup();
    float curVal;
    
    while(1){
        lcd_setCursor(0,0);
        curVal = (3.3/1024)*adValue*100/2.6;
        sprintf(adStr, "%6.2f %%", curVal);
        lcd_printStr(adStr);
          
        lcd_setCursor(0,1);        
        if(curVal >= 80)      lcd_printStr("Too wet");
        else if(curVal >= 60) lcd_printStr("Bit wet");
        else if(curVal >= 40) lcd_printStr("Normal");
        else if(curVal >= 20) lcd_printStr("Bit dry");
        else if(curVal >= 0)  lcd_printStr("Too dry");

        putVal(getAvg());
        delay(100);
    }
    
    return 0;
}
