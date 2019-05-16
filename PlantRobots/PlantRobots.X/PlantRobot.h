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
#ifndef PlantRobot_H
#define	PlantRobot_H


#ifdef	__cplusplus
extern "C" {
#endif

// Buffer Functions
    #define N 1024
    void putVal(int newValue);
    unsigned long int getAvg();
    void initBuffer();

// Flow Control Functions
    void flowControlInit(void);
    void waterTime(uint8_t water_time_in_seconds);
    //void pause(uint8_t pause_in_seconds);

// Display Functions
    void delay(unsigned long int x);
    void lcd_cmd(char command);
    void lcd_init(void);
    void lcd_setCursor(char x, char y);
    void lcd_printChar(char myChar);
    void lcd_printStr(const char *s);

#ifdef	__cplusplus
}
#endif

#endif	/* PLANTROBOT_H */

