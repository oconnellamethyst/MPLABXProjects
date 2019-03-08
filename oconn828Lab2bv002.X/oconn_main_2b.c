/*
 * File:   oconn_main_2b.c
 * Author: amethyst
 *
 * Created on January 22, 2019, 3:56 PM
 */

#include "xc.h"
#include "oconn_assem_2b.h"

#define PERIOD 5

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


struct redgreenblue {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

void setup(void)
{
    // Execute once code goes here
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0x9fff;            //sets all pins to digital I/O
    TRISA = 0b0000000000011110;  //set port A to inputs, 
    TRISB = 0b0000000000000000;  //and port B to outputs
    LATA = 0x0000;               //Set all of port A to HIGH
    LATB = 0xffff;               //and all of port B to HIGH

}

/*void delay(int delay_in_ms){
    for( delay_in_ms; delay_in_ms != 0; delay_in_ms-- ){
        ado_wait_1ms();
   }
}*/


unsigned long int packer(unsigned char r, unsigned char g, unsigned char b)
{
    unsigned long int RGBval = 0;
    RGBval = ((long)r << 16) | ((long)g << 8) | ((long)b);
    return RGBval;
}

unsigned long int packerStruct(struct redgreenblue rgb)
{
    unsigned long int RGBval = 0;
    RGBval = ((long)(int)rgb.r << 16) | ((long)(int)rgb.g << 8) | ((long)(int)rgb.b);
    return RGBval;
}

struct redgreenblue unpacker(unsigned long int RGBval){
    unsigned char Red    = 0;
    unsigned char Green  = 0;
    unsigned char Blue   = 0;
    Red   = (unsigned char) (RGBval >> 16);
    Green = (unsigned char) (RGBval >> 8 );
    Blue  = (unsigned char) (RGBval >> 0 );
    struct redgreenblue rgb = {Red, Green, Blue};
    return rgb;

}

void minColor(int r, int g, int b){
    ado_wait_100us();
    if((r & 0b10000000) == 0b10000000){
        write_1();
    }
    else{
        write_0();
    }
    
    if((r & 0b01000000) == 0b01000000){
        write_1();
    }
    else{
        write_0();
    }
    
    if((r & 0b00100000) == 0b00100000){
        write_1();
    }
    else{
        write_0();
    }
    
    if((r & 0b00010000) == 0b00010000){
        write_1();
    }
    else{
        write_0();
    }
    
    if((r & 0b00001000) == 0b00001000){
        write_1();
    }
    else{
        write_0();
    }
    
    if((r & 0b00000100) == 0b00000100){
        write_1();
    }
    else{
        write_0();
    }
    
    if((r & 0b00000010) == 0b00000010){
        write_1();
    }
    else{
        write_0();
    }
    
    if((r & 0b00000001) == 0b00000001){
        write_1();
    }
    else{
        write_0();
    }
    //g
    if((g & 0b10000000) == 0b10000000){
        write_1();
    }
    else{
        write_0();
    }
    
    if((g & 0b01000000) == 0b01000000){
        write_1();
    }
    else{
        write_0();
    }
    
    if((g & 0b00100000) == 0b00100000){
        write_1();
    }
    else{
        write_0();
    }
    
    if((g & 0b00010000) == 0b00010000){
        write_1();
    }
    else{
        write_0();
    }
    
    if((g & 0b00001000) == 0b00001000){
        write_1();
    }
    else{
        write_0();
    }
    
    if((g & 0b00000100) == 0b00000100){
        write_1();
    }
    else{
        write_0();
    }
    
    if((g & 0b00000010) == 0b00000010){
        write_1();
    }
    else{
        write_0();
    }
    
    if((g & 0b00000001) == 0b00000001){
        write_1();
    }
    else{
        write_0();
    }
    //b
    if((b & 0b10000000) == 0b10000000){
        write_1();
    }
    else{
        write_0();
    }
    
    if((b & 0b01000000) == 0b01000000){
        write_1();
    }
    else{
        write_0();
    }
    
    if((b & 0b00100000) == 0b00100000){
        write_1();
    }
    else{
        write_0();
    }
    
    if((b & 0b00010000) == 0b00010000){
        write_1();
    }
    else{
        write_0();
    }
    
    if((b & 0b00001000) == 0b00001000){
        write_1();
    }
    else{
        write_0();
    }
    
    if((b & 0b00000100) == 0b00000100){
        write_1();
    }
    else{
        write_0();
    }
    
    if((b & 0b00000010) == 0b00000010){
        write_1();
    }
    else{
        write_0();
    }
    
    if((b & 0b00000001) == 0b00000001){
        write_1();
    }
    else{
        write_0();
    }
    
    }

/*void writeColor(unsigned long int color){
    struct redgreenblue rgb = unpacker(color);
    minColor((int)rgb.r);
    minColor((int)rgb.g);
    minColor((int)rgb.b);
    }

void writeColorNum(int r, int g, int b){
    minColor(r);
    minColor(g);
    minColor(b);
}*/

void wheel(unsigned long int WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    minColor(255 - WheelPos * 3, 0, WheelPos * 3);
    //struct redgreenblue rgb = {255 - WheelPos * 3, 0, WheelPos * 3};
    //return rgb;
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    minColor(0, WheelPos * 3, 255 - WheelPos * 3);
    //struct redgreenblue rgb = {0, WheelPos * 3, 255 - WheelPos * 3};
    //return rgb;
  }
  WheelPos -= 170;
  minColor(WheelPos * 3, 255 - WheelPos * 3, 0);
  //struct redgreenblue rgb = {WheelPos * 3, 255 - WheelPos * 3, 0};
  //return rgb;
}

int main(void)
{
    setup();
    unsigned long int cont = 0;
    
    while(1) {
        
        //How do I reset RA0 and to what? is this what we want
        /*
        
        
        cont = wheel(cont);
        writeColor(cont);
         */
        
        //wheel(cont);
        while(cont<255){
            ado_wait_1ms();
            ado_wait_1ms();
            ado_wait_1ms();
            ado_wait_1ms();
            ado_wait_1ms();
            wheel(cont);
            cont++;
            
        }
        cont = 0;
        
        
        /*
        int r = 255;
        int b = 0;
        int l = 1;
        
        if (b==255){
            l=1;
        }
        else if (r==255){
            l=0;
        }
        else if (l == 1){
            r++;
            b--;
        }
        else if (l==0){
            r--;
            b++;
        }
        
        cont = packer((unsigned char)r,0,(unsigned char)b);
        writeColor(cont);*/
       
        /*
        write_0();
        write_0();
        write_0();
        write_0();
        
        write_0();
        write_0();
        write_0();
        write_0();
        
        write_1();
        write_1();
        write_1();
        write_1();
        
        write_1();
        write_1();
        write_1();
        write_1();
        
        write_0();
        write_0();
        write_0();
        write_0();
        
        write_0();
        write_0();
        write_0();
        write_0();*/
        
    }
    return 0;
}