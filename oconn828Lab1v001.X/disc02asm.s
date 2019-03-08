        .equ __P24FJ64GA002,1        ;required "boiler-plate" (BP)
        .include "p24Fxxxx.inc"      ;BP

#include "xc.inc"                    ;BP
	; DO NOT include config statements. We define them in the C file
        ;the next two lines set up the actual chip for operation - required
        ;config __CONFIG2, POSCMOD_NONE & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSECME & FNOSC_FRCPLL & SOSCSEL_LPSOSC & WUTSEL_FST & IESO_OFF
        ;config __CONFIG1, FWDTEN_OFF & COE_OFF & BKBUG_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF

    .bss                        ;put the following labels in RAM
_counter:  .space  2          ;declare a 32-bit variable
; you SHOULD NOT define stack, becasue this file is compiled 
; alongside a C file that contains main(). The C compiler
; will create stack space for us.
;stack:
;    .space  32                  ;this will be our stack area, from var_16_bit to here
    .text                       ;BP (put the following data in ROM(program memory))
    .global _myAsmFunc
    .global _myGlobalVar
    .global _counter

_myAsmFunc:
    push    w0
    
    inc	    _myGlobalVar
    inc	    _counter
    
    bra	    checkLoopEndCond
loopBody:
    btg    LATB, #2	    ; btg stands for bit-toggle.
    nop
    dec	    WREG0
checkLoopEndCond: 
    cp0	    w0
    bra	    nz, loopBody

    pop	    w0
    return
    .end

