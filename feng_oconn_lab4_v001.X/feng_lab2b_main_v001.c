/*
 * File:   feng_lab2b_main_v001.c
 * Author: Fengj
 *
 * Created on February 17, 2019, 3:19 PM
 */


#include "xc.h"
#include "feng_lab2b_asmLib_v001.h"
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


#define PERIOD 5
void setup(void)
{
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    AD1PCFG = 0x9fff;
    TRISA = 0b1111111111111110;  //set port RA0 to input, 
    LATA = 0x0000;              //set pin RA0 high

}

void writeColor(int r, int g, int b){
    /**
    write_0();
    write_0();
    write_0();
    write_0();
 
    write_1();
    write_1();
    write_1();
    write_1();
   **/
   
    int i, j,k;
    
        
    for(i = 0; i< 8; i++){
        if((r & 0x0080) == 0x0080 ){
            write_1();
        }
        
        else{
            write_0();
    }
     r = r << 1;   
    
    }
    
    
   
    
      
    for( j = 0; j< 8; j++){
        if((g & 0x0080) == 0x0080 ){
            write_1();
        }
        else{
            write_0();
    }
     g = g << 1;   
    }
    
    
    for(k = 0; k< 8; k++){
        if((b & 0x0080) == 0x0080 ){
            write_1();
        }
        else{
            write_0();
    }
     b = b << 1;   
    }
    
    

    
    
   
        
   
     
    
}


void delay(int delay_in_ms){
    int d;
    for(d=0;d<delay_in_ms;d++){
        ms_wait();
    }
}





uint32_t packColor(unsigned char Red, unsigned char Grn, unsigned char Blu){
    unsigned long RGBval = ((long) Red << 16) | ((long) Grn << 8) | ((long) Blu);
    return RGBval;
}

unsigned char getR(uint32_t RGBval){
    
    unsigned char Red    = 0;
    unsigned char Green  = 0;
    unsigned char Blue   = 0;
    Red   = (unsigned char) (RGBval >> 16);
    Green = (unsigned char) (RGBval >> 8 );
    Blue  = (unsigned char) (RGBval >> 0 );
    return Red;


}
unsigned char getG(uint32_t RGBval){
    
       
    unsigned char Red    = 0;
    unsigned char Green  = 0;
    unsigned char Blue   = 0;
    Red   = (unsigned char) (RGBval >> 16);
    Green = (unsigned char) (RGBval >> 8 );
    Blue  = (unsigned char) (RGBval >> 0 );
    return Green;


    
}
unsigned char getB(uint32_t RGBval){
       
    unsigned char Red    = 0;
    unsigned char Green  = 0;
    unsigned char Blue   = 0;
    Red   = (unsigned char) (RGBval >> 16);
    Green = (unsigned char) (RGBval >> 8 );
    Blue  = (unsigned char) (RGBval >> 0 );
    return Blue;



}


void writePacCol(uint32_t PackedColor){
    writeColor(getR(PackedColor),getG(PackedColor),getB(PackedColor));
}


uint32_t Wheel(unsigned char WheelPos){
    
    WheelPos = 255 - WheelPos;
    if(WheelPos < 85) {
        
            writeColor(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    if(WheelPos < 170) {
        WheelPos -= 85;
        
            writeColor(0, WheelPos * 3, 255 - WheelPos * 3);
    }
    
    
    WheelPos -= 170;
        writeColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}

 



int main(void) {
    setup();
    int m;
    for(m =0; m<50; m++ ){
        ms_wait();
    }  
    
    
    
    while(1){
        int n;
        for(n =0; n<255;n++){
            delay(PERIOD);
            Wheel(n);
        }
        
        
        for(n = 253; n>0; n--){
             delay(PERIOD);
             Wheel(n);
        
        }
        
       
    }
    
    return 0;
}
