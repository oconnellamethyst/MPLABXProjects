/*
 * File:   feng0084_lab4_main_v001.c
 * Author: Fengj
 *
 * Created on March 12, 2019, 12:34 AM
 */

#include <p24FJ64GA002.h>
#include <stdio.h>   
#include "xc.h"
#include "string.h"
#include "stdbool.h"
#include "LCDscreen.h"
#include "feng_lab2b_asmLib_v001.h"
#define slave_address_write 0b01111100;
#define CONTRAST 0b10000000;




#define BUFFER_SIZE 1024
 int index = 0;
volatile unsigned int buffer[BUFFER_SIZE];



void putVal(int newValue){
    if(index < BUFFER_SIZE ){
        buffer[index] = newValue;
        index++;
    }
    else{
        index = 0;
        buffer[index] = newValue;
        index++;
    }
}
int getAvg(){
    unsigned long int sum = 0;
     int i = 0;
    for(i = 0; i<BUFFER_SIZE ; i++){
        sum += buffer[i];
    }
     
     return sum/BUFFER_SIZE;
}
int initBuffer(){
    int i = 0;
    for(i = 0; i<BUFFER_SIZE ; i++){
        buffer[i] = 0;
    }
    return 0;
}




void __attribute__ ((__interrupt__,auto_psv)) _ADC1Interrupt(void)
{
    IFS0bits.AD1IF = 0;
    
    putVal(ADC1BUF0);
}

void __attribute__ ((__interrupt__,auto_psv)) _T2Interrupt(void)
{
    IFS0bits.T2IF = 0;
    
 int average = getAvg();
	char adStr[50];
    sprintf(adStr, "%6.4f V", (3.3/1024)*average);  
   scroll_left(adStr);  
 
    
}
 
   
    
    





void setup(void) {
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0b1111111111111110;   // AN0 as analog, all other pins are digital
    TRISAbits.TRISA0 = 1;              //AN0 as input
   
    //Set up LCD: 
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    I2C2CON = 0;
    I2C2BRG = 0x9D;
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.I2CEN = 1;
    
    
    AD1CON1 = 0;
    AD1CON2bits.VCFG = 0b000;
    AD1CON1bits.SSRC = 0b010;          //use Timer 3 as the trigger for A/D conversion
    AD1CHS = 0x0000;                 // Connect RA0/AN0 as CH0 input..
    AD1CSSL = 0;
    AD1CON3 = 0x0000; // Sample time is TMR3, Tad = Tcy
    AD1CON2 = 0x0004; // Set AD1IF after 2 conversions
    
    
    
    TMR3 =0; // set TMR3 to time out every 
    T2CONbits.TCKPS =0b10; //1:64 pre scale
    PR3 = 15600; //0.0264s
    T3CONbits.TON = 1;
    AD1CON1bits.ASAM = 1; // start auto sampling every  0.0624 s. 16 samples per second
    AD1CON1bits.ADON = 1;                  // turn ADC ON
    
    
    
    IEC0bits.AD1IE = 1;                    //enable AD1 interrupt  
    IFS0bits.AD1IF = 0;  

   
    
    T2CON = 0;
    T2CONbits.TCKPS =0b10;
    PR2 =25000; //trigger timer2 every 
    IFS0bits.T2IF = 0;
    T2CONbits.TON = 1;
    IEC0bits.T2IE = 1; //Enable Timer2 interrupts
    
}


int main(void) {
    setup();
    lcd_init();
    lcd_clear();
    initBuffer();
    while(1) {
        //scroll_left("Hello World!");
        //scroll_right("Hello World!");
    }
    return 0;
}
