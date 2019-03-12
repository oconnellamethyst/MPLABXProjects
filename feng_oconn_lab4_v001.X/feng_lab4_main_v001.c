/*
 * File:   feng_lab3_main_v001.c
 * Author: Fengj
 *
 * Created on February 25, 2019, 8:14 PM
 */


#include <p24FJ64GA002.h>

#include "xc.h"
#include "feng_lab2b_asmLib_v001.h"

#include "servo.h"
#include "sevensegment.h"


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

void setup(void)
{
    CLKDIVbits.RCDIV = 0;  //Set RCDIV=1:1 (default 2:1) 32MHz or FCY/2=16M
    
}



/*
 * 
 * The following functions should have been moved to the libraries, if the libraries are not working check here
void init7seg(void) {
    AD1PCFG = 0x9FFF;
    TRISB = 0x0000;
    LATB = 0xFFFF;
    LATA = 0xFFFF;
    /**
    LATBbits.LATB10 =1; //left
     
    LATBbits.LATB9 = 0; //A
     LATBbits.LATB8 = 0; //B
     LATBbits.LATB7 = 0; //C
     LATBbits.LATB6 = 0;//D
     LATBbits.LATB5 = 0;//E
     LATBbits.LATB4 = 0;//F
     LATBbits.LATB3 = 0;//G
      LATBbits.LATB2 = 0;//DP
     
}

void showChar7seg(char myChar, int myDigit){
    //where myDigit indicates LEFT (1) or RIGHT (0)
   LATB = 0xFFFF ;
    if(myDigit == 1){
        LATBbits.LATB10 =1;
        LATBbits.LATB11 = 0;
        
    }
    else if(myDigit == 0){
        LATBbits.LATB10 =0;
        LATBbits.LATB11 = 1;
        
    }
    
    if(myChar == '1'){
        LATBbits.LATB8 = 0; //B
        LATBbits.LATB7 = 0; //C
    }
    if (myChar == '2'){
        LATBbits.LATB9 = 0; //A
         LATBbits.LATB8 = 0; //B
          LATBbits.LATB3 = 0;//G
          LATBbits.LATB5 = 0;//E
         LATBbits.LATB6 = 0;//D
    }
    
    if(myChar == '3'){
        LATBbits.LATB9 = 0; //A
     LATBbits.LATB8 = 0; //B
     LATBbits.LATB7 = 0; //C
     LATBbits.LATB6 = 0;//D
     LATBbits.LATB3 = 0;//G
    }
    if(myChar == '4'){
        LATBbits.LATB8 = 0; //B
        LATBbits.LATB7 = 0; //C
        LATBbits.LATB4 = 0;//F
        LATBbits.LATB3 = 0;//G
    }
    
    if(myChar =='5'){
        LATBbits.LATB9 = 0; //A
        LATBbits.LATB4 = 0;//F
        LATBbits.LATB3 = 0;//G
        LATBbits.LATB7 = 0; //C
        LATBbits.LATB6 = 0;//D
    }
    
    if(myChar =='6'){
     LATBbits.LATB9 = 0; //A
     LATBbits.LATB7 = 0; //C
     LATBbits.LATB6 = 0;//D
     LATBbits.LATB5 = 0;//E
     LATBbits.LATB4 = 0;//F
     LATBbits.LATB3 = 0;//G
        
    }
    
    if(myChar =='7'){
     LATBbits.LATB9 = 0; //A
     LATBbits.LATB8 = 0; //B
     LATBbits.LATB7 = 0; //C
     
        
    }
    
    if(myChar=='8'){
     LATBbits.LATB9 = 0; //A
     LATBbits.LATB8 = 0; //B
     LATBbits.LATB7 = 0; //C
     LATBbits.LATB6 = 0;//D
     LATBbits.LATB5 = 0;//E
     LATBbits.LATB4 = 0;//F
     LATBbits.LATB3 = 0;//G
    }
    
    if(myChar=='9'){
     LATBbits.LATB9 = 0; //A
     LATBbits.LATB8 = 0; //B
     LATBbits.LATB7 = 0; //C
     LATBbits.LATB6 = 0;//D
     LATBbits.LATB4 = 0;//F
     LATBbits.LATB3 = 0;//G
    }
    
    if(myChar=='0'){
     LATBbits.LATB9 = 0; //A
     LATBbits.LATB8 = 0; //B
     LATBbits.LATB7 = 0; //C
     LATBbits.LATB6 = 0;//D
     LATBbits.LATB5 = 0;//E
     LATBbits.LATB4 = 0;//F
    
    }
    
    if(myChar=='A'){
       
     LATBbits.LATB9 = 0; //A
     LATBbits.LATB8 = 0; //B
     LATBbits.LATB7 = 0; //C
    LATBbits.LATB3 = 0;//G
     LATBbits.LATB5 = 0;//E
     LATBbits.LATB4 = 0;//F
     
    
    }
    
    if(myChar=='b'){
     LATBbits.LATB7 = 0; //C
     LATBbits.LATB6 = 0;//D
     LATBbits.LATB5 = 0;//E
     LATBbits.LATB4 = 0;//F
     LATBbits.LATB3 = 0;//G
    }
    
     if(myChar=='C'){
     LATBbits.LATB9 = 0; //A
     LATBbits.LATB6 = 0;//D
     LATBbits.LATB5 = 0;//E
     LATBbits.LATB4 = 0;//F
 
    }
    
    if(myChar=='d'){
    
     LATBbits.LATB8 = 0; //B
     LATBbits.LATB7 = 0; //C
     LATBbits.LATB6 = 0;//D
     LATBbits.LATB5 = 0;//E
     LATBbits.LATB3 = 0;//G
    }
    
     if(myChar=='E'){
     LATBbits.LATB9 = 0; //A
     LATBbits.LATB6 = 0;//D
     LATBbits.LATB5 = 0;//E
     LATBbits.LATB4 = 0;//F
     LATBbits.LATB3 = 0;//G
    }
     if(myChar=='F'){
     LATBbits.LATB9 = 0; //A
     LATBbits.LATB5 = 0;//E
     LATBbits.LATB4 = 0;//F
     LATBbits.LATB3 = 0;//G
    }
}



void test7seg(void){
    showChar7seg('0', 0);
    delay(250);
    showChar7seg('1', 0);
    delay(250);
    showChar7seg('2', 0);
    delay(250);
    showChar7seg('3', 0);
    delay(250);
    showChar7seg('4', 0);
    delay(250);
    showChar7seg('5', 0);
    delay(250);
    showChar7seg('6', 0);
    delay(250);
    showChar7seg('7', 0);
    delay(250);
    showChar7seg('8', 0);
    delay(250);
    showChar7seg('9', 0);
    delay(250);
    showChar7seg('A', 0);
    delay(250);
    showChar7seg('b', 0);
    delay(250);
    showChar7seg('C', 0);
    delay(250);
    showChar7seg('d', 0);
    delay(250);
    showChar7seg('E', 0);
    delay(250);
    showChar7seg('F', 0);
    delay(250);
    showChar7seg('0', 1);
    delay(250);
    showChar7seg('1', 1);
    delay(250);
    showChar7seg('2', 1);
    delay(250);
    showChar7seg('3', 1);
    delay(250);
    showChar7seg('4', 1);
    delay(250);
    showChar7seg('5', 1);
    delay(250);
    showChar7seg('6', 1);
    delay(250);
    showChar7seg('7', 1);
    delay(250);
    showChar7seg('8', 1);
    delay(250);
    showChar7seg('9', 1);
    delay(250);
    showChar7seg('A', 1);
    delay(250);
    showChar7seg('b', 1);
    delay(250);
    showChar7seg('C', 1);
    delay(250);
    showChar7seg('d', 1);
    delay(250);
    showChar7seg('E', 1);
    delay(250);
    showChar7seg('F', 1);
    delay(250);
}
void initkeypad(void){
    AD1PCFG = 0x9FFF;
    LATA = 0xFFFF;
    TRISA = 0xFFFF;
    CNPU1bits.CN2PUE =1;
    CNPU1bits.CN3PUE = 1;
    CNPU2bits.CN30PUE =1;
    CNPU2bits.CN29PUE  =1;
}

char readKeyPadRaw(void){
    
    LATBbits.LATB12 = 0;
    LATBbits.LATB13 = 1;
    LATBbits.LATB14 = 1;
    LATBbits.LATB15 = 1;
    ms_wait();
    if(PORTAbits.RA3 ^ 1){
        return '1';
    }
    
    if(PORTAbits.RA2 ^ 1){
        return '2';
    }
    
    if(PORTAbits.RA1 ^ 1){
        return '3';
    }
    if(PORTAbits.RA0 ^ 1){
        return 'A';
    }
    
    
    LATBbits.LATB12 = 1;
    LATBbits.LATB13 = 0;
    LATBbits.LATB14 = 1;
    LATBbits.LATB15 = 1;
    ms_wait();
    if(PORTAbits.RA3 ^ 1){
        return '4';
    }
    
    if(PORTAbits.RA2 ^ 1){
        return '5';
    }
    
    if(PORTAbits.RA1 ^ 1){
        return '6';
    }
    if(PORTAbits.RA0 ^ 1){
        return 'b';
    }
    
    
     LATBbits.LATB12 = 1;
    LATBbits.LATB13 = 1;
    LATBbits.LATB14 = 0;
    LATBbits.LATB15 = 1;
    ms_wait();
    if(PORTAbits.RA3 ^ 1){
        return '7';
    }
    
    if(PORTAbits.RA2 ^ 1){
        return '8';
    }
    
    if(PORTAbits.RA1 ^ 1){
        return '9';
    }
    if(PORTAbits.RA0 ^ 1){
        return 'C';
    }
    
     LATBbits.LATB12 = 1;
    LATBbits.LATB13 = 1;
    LATBbits.LATB14 = 1;
    LATBbits.LATB15 = 0;
    ms_wait();
    if(PORTAbits.RA3 ^ 1){
        return 'E';
    }
    
    if(PORTAbits.RA2 ^ 1){
        return '0';
    }
    
    if(PORTAbits.RA1 ^ 1){
        return 'F';
    }
    if(PORTAbits.RA0 ^ 1){
        return 'd';
    }
    return 'x';
} */

void delay(int delay_in_ms){            
    int d;
    for(d=0;d<delay_in_ms;d++){
        ms_wait();                  // call the millisecond delay function delay_in_ms times
    }
}

int main(void) {
    setup();
    init7seg(); // sets the bits right for the seven segment display to run
    initkeypad(); // does likewise for the keypad
    char Value[2] = {'x','x'}; // sets up an array for use, with x's so that we know when it hasn't been used
    char nextDig[2] = {'x','x'}; // does likewise
    
    while(1) {
        char newDig = readKeyPadRaw(); // sets up char, new dig. Runs function readKeyPad, which returns char
        nextDig[0] = nextDig[1]; // slides the number
        nextDig[1] = newDig; // places in the new number
        
        if (nextDig[0] == 'x' && nextDig[1] != 'x') { //similar code to above
            Value[0] = Value[1];
            Value[1] = newDig;
        }
         
        showChar7seg(Value[1],0); // The second bit determines which character is displaying, and the first what is displayed
        delay(5); // Delay so that the number shows up
        showChar7seg(Value[0],1); // Display the other number
        delay(5); // Likewise
    }
    return 0;
}
