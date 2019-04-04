/* 
 * File:   oconn_assem_5.h
 * Author: amethyst
 *
 * Created on February 19, 2019, 3:51 PM
 */

#ifndef OCONN_ASSEM_5_H
#define	OCONN_ASSEM_5_H

#ifdef	__cplusplus
extern "C" {
#endif

    void ado_wait_100us(void);
    void ado_wait_1ms(void);
    void write_0(void);
    void write_1(void);
    void setup(void);
    int main(void);
    void minColor(int r, int g, int b);
    void delay(int delay_in_ms);
    
    void Wheel(unsigned long int WheelPos);
    void writeColor(unsigned long int color);
    void writeColorNum(int r, int g, int b);
    struct redgreenblue unpacker(unsigned long int RGBval);
    unsigned long int packer(unsigned char r, unsigned char g, unsigned char b);
    unsigned long int packerStruct(struct redgreenblue rgb);
    
    
#ifdef	__cplusplus
}
#endif

#endif	/**/

