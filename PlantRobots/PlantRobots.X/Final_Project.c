/*
Date: May 6th, 2019
Name: Junchi Feng, Roman Woolery, Scott Deyo, Amethyst O'Connell
Course number: EE2361
Term: Spring 2019
Lab/assignment number: EE2361 Final Project
Short Program Description: Plant Robot watering code, this code runs off of the
PIC24 and uses the ADC (code primarily from lab 6) and the principle of voltage 
division to turn an analog readout of the voltage into a percent moisture based 
on calibrations made with the sensor. It then takes this percent moisture readout
and prints it to an LCD screen while using the percent moisture to determine 
the duration of running a relay which runs a solenoid which waters a plant.
*/

#include "xc.h"
#include <stdio.h> //for sprintf
#include "PlantRobot.h"

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

    // TMR1 used for simple delay
    T1CON = 0;
    PR1 = 62500;
    TMR1 = 0;
    T1CONbits.TCKPS = 3;
    T1CONbits.TON = 1;

    //Timer 3 stuff
    T3CONbits.TCKPS0 = 0; //1:64
    T3CONbits.TCKPS1 = 1;
    TMR3 = 0;
    PR3 = 15625; //Once ever 1/16th of a second.
    T3CONbits.TON = 1;

    AD1CON1bits.ADON = 1;
    lcd_init();
    flowControlInit();
    return 0;
}

unsigned long int adValue; //(Analog to Digital) Value from the ADC buffer
char adStr[50]; // (Analog to Digital) String for use with sprintf below

void __attribute__((interrupt, auto_psv)) _ADC1Interrupt(){
    IFS0bits.AD1IF = 0; //read voltage from A0 for 16 times a second
    adValue = ADC1BUF0;
}

int main(void){
    setup();
    float curVal;

    while(1){
        lcd_setCursor(0,0);
        curVal = (3.3/1024)*adValue*100/2.6; //2.6 is a calibration value, 2.6 volts, the maximum voltage
        
        // If this were being sold commercially you would need to add an external timer to the PIC24
        // and create a function which would adjust that 2.6V for the corrosion of the resistive circuit
        // based on the timestamp. Alternatively, you could use a capacitive soil sensor
        // but you'd want one that would work with the PIC24 natively, i.e. not the Adafruit STEMMA.
        
        sprintf(adStr, "%6.2f %%", curVal); 
        lcd_printStr(adStr);

        lcd_setCursor(0,1);
        if(curVal >= 80)      lcd_printStr("Too wet"); // Percent values in terms of watering
        else if(curVal >= 60) lcd_printStr("Bit wet"); // These thresholds could be changed
        else if(curVal >= 40) lcd_printStr("Normal "); // For different types of plants
        else if(curVal >= 20) lcd_printStr("Bit dry");
        else if(curVal >= 0){  
            lcd_printStr("Too dry");
            waterTime(3);
        }
        putVal(getAvg());
        delay(100);
    }

    return 0;
}
