/* 
 * File:   sevensegment.h
 * Author: Fengj
 *
 * Created on March 5, 2019, 5:00 PM
 */

#ifndef SEVENSEGMENT_H
#define	SEVENSEGMENT_H

#include <xc.h> // include processor files - each processor file is guarded. 

#ifdef	__cplusplus
extern "C" {
#endif

void init7seg(void);
void showChar7seg(char myChar, int myDigit);
void test7seg(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SEVENSEGMENT_H */

