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
#include "LCDscreen.h"
#include "feng_lab2b_asmLib_v001.h"
#define slave_address_write 0b01111100;
#define CONTRAST 0b10000000


int main(void) {
    setup();
    lcd_init();
    lcd_clear();
    while(1) {
        scroll_left("Hello World!");
        scroll_right("Hello World!");
    }
    return 0;
}
