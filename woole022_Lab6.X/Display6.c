#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "woole022_Lab6.h"

void delay(unsigned long int x){
    unsigned long int y; //x is # of ms
    for(y=0; y<x; y++){
        asm("repeat #15998");
        asm("nop");
    }
}

void lcd_cmd(char command) {
    I2C2CONbits.SEN = 1;
    while(I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b1111100; 
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0;
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = command;
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1);
    IFS3bits.MI2C2IF = 0;
}

void lcd_init(void){
    delay(50);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00111001); // function set, extend0000); // contrast C3-C0
    lcd_cmd(0b01011110); // Ion, Bon, C5-C4ed instruction mode
    lcd_cmd(0b00010100); // interval osc
    lcd_cmd(0b01110000); // contrast C3-C0
    lcd_cmd(0b01011110); // Ion, Bon, C5-C4
    lcd_cmd(0b01101100); // follower control
    delay(200);
    lcd_cmd(0b00111000); // function set, normal instruction mode
    lcd_cmd(0b00001100); // Display On
    lcd_cmd(0b00000001); // Clear Display
    delay(2);
}

void lcd_setCursor(char x, char y){
    int location = 0x40*y+x; //0x40 * row + column
    int coords = location + 0x80;
    lcd_cmd(coords);
}

void lcd_printChar(char myChar){

    I2C2CONbits.SEN = 1;    //Initiate Start condition
    while(I2C2CONbits.SEN == 1);  // SEN will clear when Start Bit is complete
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01111100; // 8-bits consisting of the slave address and the R/nW bit
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b01000000; // 8-bits consisting of control byte /w RS=1
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = myChar; // 8-bits consisting of the data byte
    
    while(IFS3bits.MI2C2IF == 0);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1); // PEN will clear when Stop bit is complete
}

void lcd_printStr(const char *s){
    int i;
    for(i=0; i<strlen(s); i++){
        if(i>7){
            break;
        }
        lcd_printChar(s[i]);
    }
}  

