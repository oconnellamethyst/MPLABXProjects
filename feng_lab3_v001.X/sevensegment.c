// Seven Segment Library
//Functions for doing a 7-segment display

#include "xc.h"
#include "sevensegment.h"

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
     **/
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