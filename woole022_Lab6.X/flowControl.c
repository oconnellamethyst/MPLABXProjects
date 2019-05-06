#include "xc.h"

void flowControlInit(void)
{
    AD1PCFG &= 0x9fff; // all digital
    TRISB &= 0x3FFF;     // make sure RB0 and RB1 are outputs
    LATB &= 0x3FFF;         // make sure they start low
}

void pause(uint8_t pause_in_seconds)
{
    int i = 0;
    while(i < pause_in_seconds)
    {
        while(!_T1IF);
        _T1IF = 0;
        i++;
    }
}

void waterTime(uint8_t water_time_in_seconds)
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
