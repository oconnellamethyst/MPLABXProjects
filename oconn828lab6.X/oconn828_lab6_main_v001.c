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
#include "stdio.h"
#include "LCDscreen.h"
#include "feng_lab2b_asmLib_v001.h"
#define slave_address_write 0b01111100;
#define CONTRAST 0b10000000

int createLatest(void) {
	int adValue;
	char adStr[20];
    adValue = ADC1BUF0;
    sprintf(adStr, "%6.4f", (3.3/1024)*adValue);  // ?x.xxxx V?
                       // 6.4 in the format string ?%6.4f? means 6 placeholders for the whole
                       // floating point number, 4 of which are for the fractional part.
    return float(adStr);
}

int main(void) {
    setup();
    lcd_init();
    lcd_clear();
    float lastInputs[16]; //relies on pushing and popping new values from this array in the form of a float, i.e. pushing and popping new values of (3.3/1024)*ADC1BUF0
    int init = 0;
    float avgInput = 0;
    float avgInputCreator = 0;
    char adStr[15];
    int count;
    
    while(1) {
        if (init < 15){ //15 because 16 array
            avgInput = (lastInputs[init] + avgInput*(init))/16; //the average is the former average times the former average number plus the new number
            init++;
        }
        else{
            for (count = 0; count > init; count++ ) {
                avgInputCreator = avgInputCreator + lastInputs[count]; // goes through and ads each in the lastInputs together into a things
            }
            avgInput = avgInputCreator/16;
        }
        
        sprintf(adStr, "%6.4f V", avgInput);
        
        if (avgInput != 0){
            scroll_left(adStr);
            scroll_right(adStr);
        }
        
    }
    return 0;
}
