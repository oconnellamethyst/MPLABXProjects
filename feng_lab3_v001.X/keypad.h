/* 
 * File:   keypad.h
 * Author: Fengj
 *
 * Created on March 5, 2019, 4:56 PM
 */

#ifndef KEYPAD_H
#define	KEYPAD_H

#include <xc.h> // include processor files - each processor file is guarded. 

#ifdef	__cplusplus
extern "C" {
#endif

void initkeypad(void);
char readKeyPadRaw(void);


#ifdef	__cplusplus
}
#endif

#endif	/* KEYPAD_H */

