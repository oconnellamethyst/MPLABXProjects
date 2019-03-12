// Keypad
//Functions for doing a keypad

#include "xc.h"
#include "keypad.h"


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
    
    //Sets the row bits that are being checked
    LATBbits.LATB12 = 0;
    LATBbits.LATB13 = 1;
    LATBbits.LATB14 = 1;
    LATBbits.LATB15 = 1;
    ms_wait();
    //checks the column bits in that row
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
}