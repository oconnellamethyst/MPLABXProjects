#ifndef WOOLE022_LAB6_H
#define	WOOLE022_LAB6_H
#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif

void delay(unsigned long int x);
void lcd_cmd(char command);
void lcd_init(void);
void lcd_setCursor(char x, char y);
void lcd_printChar(char myChar);
void lcd_printStr(const char *s);

#ifdef	__cplusplus
}
#endif

#endif	/* NEWFILE_H */
