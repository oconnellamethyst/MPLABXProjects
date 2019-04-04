// Library
//Functions for doing a Library

#include "xc.h"
#include "library.h"


void lcd_cmd(char command){
/*This function should take a single byte/char command and write it out the I2C bus.  The complete packet 
 * should consist of a START bit, address with R/nW byte, control byte, command/data byte, and STOP bit.  
 * It is probably a good idea to use blocking code to implement this at first, but you are encouraged 
 * to use polling or interrupts in your final library.*/
    
/*???????? is hello world in Japanese
 
 1011 1010
 1101 1101
 1100 0110
 1100 0001
 1101 1100
 1011 1110
 1011 0110
 1011 0010
 
 
 
 
 
 */
    
    int begining = 0b0;
    int bitey = 0b0;
    if(command == 'a'){
        bitey = 0b01100001;
    }
    else if(command == 'b'){
        bitey = 0b01100010;
    }
    else if(command == 'c'){
        bitey = 0b0110;
    }
    else if(command == 'd'){
        bitey = 0b0110;
    }
    else if(command == 'e'){
        bitey = 0b0110;
    }
    else if(command == 'f'){
        bitey = 0b0110;
    }
    else if(command == 'g'){
        bitey = 0b0110;
    }
    else if(command == 'h'){
        bitey = 0b0110;
    }
    else if(command == 'i'){
        bitey = 0b0110;
    }
    else if(command == 'j'){
        bitey = 0b0110;
    }
    else if(command == 'k'){
        bitey = 0b0110;
    }
    else if(command == 'l'){
        bitey = 0b0110;
    }
    else if(command == 'm'){
        bitey = 0b0110;
    }
    else if(command == 'n'){
        bitey = 0b0110;
    }
    else if(command == 'o'){
        bitey = 0b0110;
    }
    else if(command == 'p'){
        bitey = 0b0111;
    }
    else if(command == 'q'){
        bitey = 0b0111;
    }
    else if(command == 'r'){
        bitey = 0b0111;
    }
    else if(command == 's'){
        bitey = 0b0111;
    }
    else if(command == 't'){
        bitey = 0b0111;
    }
    else if(command == 'u'){
        bitey = 0b0111;
    }
    else if(command == 'v'){
        bitey = 0b0111;
    }
    else if(command == 'w'){
        bitey = 0b0111;
    }
    else if(command == 'x'){
        bitey = 0b0111;
    }
    else if(command == 'y'){
        bitey = 0b0111;
    }
    else if(command == 'z'){
        bitey = 0b0111;
    }
    else if(command == 'A'){
        bitey = 0b0100;
    }
    else if(command == 'B'){
        bitey = 0b0100;
    }
    else if(command == 'C'){
        bitey = 0b0100;
    }
    else if(command == 'D'){
        bitey = 0b0100;
    }
    else if(command == 'E'){
        bitey = 0b0100;
    }
    else if(command == 'F'){
        bitey = 0b0100;
    }
    else if(command == 'G'){
        bitey = 0b0100;
    }
    else if(command == 'H'){
        bitey = 0b0100;
    }
    else if(command == 'I'){
        bitey = 0b0100;
    }
    else if(command == 'J'){
        bitey = 0b0100;
    }
    else if(command == 'K'){
        bitey = 0b0100;
    }
    else if(command == 'L'){
        bitey = 0b0100;
    }
    else if(command == 'M'){
        bitey = 0b0100;
    }
    else if(command == 'N'){
        bitey = 0b0100;
    }
    else if(command == 'O'){
        bitey = 0b0100;
    }
    else if(command == 'P'){
        bitey = 0b0101;
    }
    else if(command == 'Q'){
        bitey = 0b0101;
    }
    else if(command == 'R'){
        bitey = 0b0101;
    }
    else if(command == 'S'){
        bitey = 0b0101;
    }
    else if(command == 'T'){
        bitey = 0b0101;
    }
    else if(command == 'U'){
        bitey = 0b0101;
    }
    else if(command == 'V'){
        bitey = 0b0101;
    }
    else if(command == 'W'){
        bitey = 0b0101;
    }
    else if(command == 'X'){
        bitey = 0b0101;
    }
    else if(command == 'Y'){
        bitey = 0b0101;
    }
    else if(command == 'Z'){
        bitey = 0b0101;
    }
    else{
        bitey = 0b00100001;
    }
}
