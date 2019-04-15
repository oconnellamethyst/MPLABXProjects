/*
 * File:   feng0084_lab4_main_v001.c
 * Author: Fengj
 *
 * Created on March 12, 2019, 12:34 AM
 */

#include <p24FJ64GA002.h>
#include "xc.h"
#include "string.h"
#include "stdbool.h"
#include "feng_lab2b_asmLib_v001.h"
#define slave_address_write 0b01111100;
#define CONTRAST 0b10000000

void setup(void) {
    CLKDIVbits.RCDIV = 0;
    AD1PCFG = 0xFFFF;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    I2C2CON = 0;
    I2C2BRG = 0x9D;
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.I2CEN = 1;
    T1CON = 0;
    PR1 = 432;
    IFS0bits.T1IF = 0;
    T1CONbits.TON = 1;
}


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


void lcd_cmd(char command) {
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = slave_address_write;
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b00000000;
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = command;
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1);
}



void lcd_init(void) {
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




void lcd_printChar(char myChar) {
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN == 1);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = slave_address_write;
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = 0b11000000;
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = myChar;
    while(!IFS3bits.MI2C2IF);
    IFS3bits.MI2C2IF = 0;
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1);
}

void lcd_setCursor(char x, char y) {
    if (x < 0 || x > 1 || y < 0 || y > 7) {
        return;
    }
    else {
        char cursorLocation = 0b10000000;
        cursorLocation |= y;
        if (x == 1) {
            cursorLocation |= 0b01000000;
        }
        lcd_cmd(cursorLocation);
    }
}

char getCoordinates(char x, char y) {
    if (x < 0 || x > 1 || y < 0 || y > 7) {
        return ' ';
    }
    else {
        char cursorLocation = 0b10000000;
        cursorLocation |= y;
        if (x == 1) {
            cursorLocation |= 0b01000000;
        }
        return cursorLocation;
    }
}

void lcd_printStr(const char *s) {
    I2C2CONbits.SEN = 1;
    while (I2C2CONbits.SEN == 1); //start bit
    IFS3bits.MI2C2IF = 0;
    I2C2TRN = slave_address_write;
    while(!IFS3bits.MI2C2IF); // address
    IFS3bits.MI2C2IF = 0;
    
    
    int i = 0;
    int row = 0;
    int column = 0;
    for (i = 0; i < 8; i++) {
        if (i == 7) { //last char in string
           I2C2TRN = 0b10000000;   
           while(!IFS3bits.MI2C2IF);
           IFS3bits.MI2C2IF = 0;
            char location = getCoordinates(row,column);
            I2C2TRN = location;
            while(!IFS3bits.MI2C2IF);
            IFS3bits.MI2C2IF = 0;
            I2C2TRN = 0b01000000;   // Control Byte: CO = 0 RS =1 ?last byte?
            while(!IFS3bits.MI2C2IF);
            IFS3bits.MI2C2IF = 0;
            I2C2TRN = s[i];  //data byte
            while(!IFS3bits.MI2C2IF);
            IFS3bits.MI2C2IF = 0;   
        }
        else { //else char in string
           I2C2TRN = 0b10000000;
           while(!IFS3bits.MI2C2IF);
           IFS3bits.MI2C2IF = 0;
            char location = getCoordinates(row,column);
            I2C2TRN = location;
            while(!IFS3bits.MI2C2IF);
            IFS3bits.MI2C2IF = 0;
            I2C2TRN = 0b11000000;   // Control Byte: CO = 1 RS =1
            while(!IFS3bits.MI2C2IF);
            IFS3bits.MI2C2IF = 0;
            I2C2TRN = s[i];  //data byte
            while(!IFS3bits.MI2C2IF);
            IFS3bits.MI2C2IF = 0;
        }
        column += 1;
        if (column == 8) {
            column = 0;
            row = 1;
        }
    }
    I2C2CONbits.PEN = 1;
    while(I2C2CONbits.PEN == 1); // end bit
    
}



void scroll_left(const char *s) {
    if (strlen(s) < 9) {
        lcd_printStr(s);
    }
    else {
        int i = 0;
        for (i = 0; i <= strlen(s)-8; i++) {
            char temp[8];
            int k = 0;
            int tempPos = 0;
            for (k = i; k < i+8; k++) {
                temp[tempPos] = s[k];
                tempPos++;
            }
            lcd_printStr(temp);
            int j = 0;
            for (j = 0; j < 500; j++) {
                ms_wait();
            }
        }
    }
}

void scroll_right(const char *s) {
    if (strlen(s) < 9) {
        lcd_printStr(s);
    }
    else {
        int i = 0;
        for (i = strlen(s)-1; i >= 7; i--) {
            char temp[8];
            int k = 0;
            int tempPos = 7;
            for (k = i; k > i-8; k--) {
                temp[tempPos] = s[k];
                tempPos--;
            }
            lcd_printStr(temp);
            int j = 0;
            for (j = 0; j < 500; j++) {
                ms_wait();
            }
        }
    }
}


void lcd_clear(void) {
    lcd_printStr("                ");
}

int main(void) {
    setup();
    lcd_init();
    lcd_clear();
    while(1) {
        scroll_left("Hello World!");
        scroll_right("Hello World!");
    }
    return 0;
}
