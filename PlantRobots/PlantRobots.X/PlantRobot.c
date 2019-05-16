/*
Date: May 6th, 2019
Name: Junchi Feng, Roman Woolery, Scott Deyo, Amethyst O'Connell
Course number: EE2361
Term: Spring 2019
Lab/assignment number: EE2361 Final Project
Short Program Description: Plant Robot watering code, this code runs off of the
PIC24 and uses the ADC and the principle of voltage division to turn an analog
readout of the voltage into a percent moisture based on calibrations made with
the sensor. It then takes this percent moisture readout and prints it to an LCD
screen while using the percent moisture to determine the duration of running
a relay which runs a solenoid which waters a plant.
*/


#include "xc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "PlantRobot.h"


//Buffer definitions
#define N 1024 // Maximum number for ADC things
volatile int Buffer[N]; // Creates the buffer array
volatile int count = -1; // "Stack pointer" of sorts for the buffer array

// Buffer Functions
void putVal(int newValue){
// Function: putVal
//  ---------------------------
// Adds a new value (int newValue) to the buffer array

    if(count<1023){
        Buffer[count++] = newValue;
    }
    else{
        count = 0;        
        Buffer[count] = newValue;
    }
}

unsigned long int getAvg(){
// Function: getAvg
//  ---------------------------
// Gets an average value from the information in the buffer array
    int x;
    unsigned long int avg = 0;
    for(x=0; x<N; x++)
        avg += Buffer[x];
    return avg/N;
}

void initBuffer(){
// Function: initBuffer
//  ---------------------------
// Initializes an empty buffer array
    int x;
    for(x=0; x<N; x++)
        Buffer[x] = 0;
}


// Flow Control Functions
void flowControlInit(void)
// Function: flowControlInit
//  ---------------------------
// Initializes the pins used in flow control to work the relay for the solenoid
{
    AD1PCFG &= 0x9fff; // all digital
    TRISB &= 0x3FFF;     // make sure RB0 and RB1 are outputs
    LATB &= 0x3FFF;         // make sure they start low
}

void pause(uint8_t pause_in_seconds)
// Function: pause
//  ---------------------------
// Delays the function for a number of seconds (pause_in_seconds). Used in the waterTime function.
{
    int i = 0;
    while(i < pause_in_seconds)
    {
        delay(1000); // there are 1000 milliseconds in a second
    }
}

void waterTime(uint8_t water_time_in_seconds)
// Function: waterTime
//  ---------------------------
// Opens the solenoid valve to water the plant for a number of seconds (water_time_in_seconds).
{
    uint32_t i;

    // Latch 'SET' on relay to turn flow solenoid ON
    LATB = 0x8000;         // RB0 latches solenoid ON
    for(i = 0; i < 160000; i++)asm("nop");   // keep high >10ms to latch
    LATB = 0;         // Turn RB0 OFF to conserve power; solenoid remains latched ON

    pause(water_time_in_seconds);   // Water keeps flowing

    // Latch 'UNSET' on relay to turn flow solenoid OFF
    LATB = 0x4000;         // RB1 latches solenoid OFF
    for(i = 0; i < 160000; i++)asm("nop");
    LATB = 0;         // ... RB1 is turned off to conserve power; solenoid remains latched OFF
}

// Display Functions
void delay(unsigned long int x){
// Function: delay
//  ---------------------------
// Delays the function for a number of milliseconds (x) using assembly code. Used in lcd_init.
    unsigned long int y; //x is # of ms
    for(y=0; y<x; y++){
        asm("repeat #15998");
        asm("nop");
    }
}

void lcd_cmd(char command) {
// Function: lcd_cmd
//  ---------------------------
// Sends a command to the LCD screen
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
// Function: lcd_init
//  ---------------------------
// Initializes the LCD screen to be used in displaying information
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
// Function: lcd_setCursor
//  ---------------------------
// Sets the position for the cursor to write new letters based on the numbers in x and y.
    int location = 0x40*y+x; //0x40 * row + column
    int coords = location + 0x80;
    lcd_cmd(coords);
}

void lcd_printChar(char myChar){
// Function: lcd_printChar
//  ---------------------------
// Prints a character (myChar) onto the LCD screen

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
// Function: lcd_printStr
//  ---------------------------
// Prints a string onto the LCD screen. Takes in a string pointer.
    int i;
    for(i=0; i<strlen(s); i++){
        if(i>7){
            break;
        }
        lcd_printChar(s[i]);
    }
}  

