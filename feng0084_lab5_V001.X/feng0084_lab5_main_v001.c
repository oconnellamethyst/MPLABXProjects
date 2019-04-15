/*
 * File:   feng0084_lab4_main_v001.c
 * Author: Fengj
 *
 * Created on March 12, 2019, 12:34 AM
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




void delay(int delay_in_ms){            
    int d;
    for(d=0;d<delay_in_ms;d++){
        ms_wait();                  // call the millisecond delay function delay_in_ms times
    }
}

void microDelay(int delay_in_ms){            
    int d;
    for(d=0;d<delay_in_ms;d++){
        micro_wait();                  
    }
}

void setup(){
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0xFFFF;
    
    
    I2C2CONbits.I2CEN = 0;   
    I2C2CON = 0;
    I2C2BRG = 0x9D;
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.I2CEN = 1;
}

void lcd_cmd(char package){
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b00000000;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = package;
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN);
}

void lcd_init(void){
    delay(50);
    lcd_cmd(0b00111000);
    delay(1);
    
    lcd_cmd(0b00111001);
    delay(1);
    
    lcd_cmd(0b00010100);
   delay(1);
    
   lcd_cmd(0b01110000);
   delay(1);
    
    lcd_cmd(0b01011110); 
    delay(1);
    
   lcd_cmd(0b01101100);
    delay(200);
    
   lcd_cmd(0b00111000); 
    delay(1);
    
   lcd_cmd(0b00001100); 
   delay(1);
    
    lcd_cmd(0b00000001);
   
    
    delay(2);
    
}

void lcd_setCursor(char x, char y){
    if(x<0 || y << 0 || x > 1 || y >7){
        return;
        
    }
    else{
        
        int location = 0b10000000;
        if (x == 0){
            location = location | y; 
            
        }
        
        if (x == 1){
            location = (0x40 + y) | location;
        }
        lcd_cmd(location);
    }
}

void lcd_printChar(char myChar){
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100;
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01000000;
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b10110001;
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1);
}

int main(void) {
    setup();
    lcd_init();
    lcd_cmd(0b00000001);
    while(1){
       lcd_setCursor(0,0);
       lcd_printChar('a');
  

    }
    
    return 0;
}
