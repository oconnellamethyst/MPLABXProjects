/* 
 * File:   LCDscreen.h
 * Author: amethyst
 *
 * Created on April 16, 2019, 2:47 PM
 */

#ifndef LCDSCREEN_H
#define	LCDSCREEN_H

#ifdef	__cplusplus
extern "C" {
#endif

void setup(void);
void delay(int delay_in_ms);
void microDelay(int delay_in_ms);
void lcd_cmd(char command);
void lcd_init(void);
void lcd_printChar(char myChar);
void lcd_setCursor(char x, char y);
char getCoordinates(char x, char y);
void lcd_printStr(const char *s);
void scroll_left(const char *s);
void scroll_right(const char *s);
void second_row_scroll_left(const char *s);
void second_row_scroll_right(const char *s);
void lcd_clear(void);


#ifdef	__cplusplus
}
#endif

#endif	/* LCDSCREEN_H */

