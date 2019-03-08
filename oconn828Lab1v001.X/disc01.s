        .equ __P24FJ64GA002,1        ;required "boiler-plate" (BP)
        .include "p24Fxxxx.inc"      ;BP

#include "xc.inc"                    ;BP
        ;the next two lines set up the actual chip for operation - required
        config __CONFIG2, POSCMOD_NONE & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSECME & FNOSC_FRCPLL & SOSCSEL_LPSOSC & WUTSEL_FST & IESO_OFF
        config __CONFIG1, FWDTEN_OFF & BKBUG_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF

    .bss                        ;put the following labels in RAM
var_32_bit:  .space  4          ;declare a 32-bit variable
var_8_bit:   .space  1          ;declare an 8-bit variable
    .align   2                  ;need this so that we have the next word at even address
var_16_bit:  .space  2          ;declare a 16-bit variable
stack:
    .space  32                  ;this will be our stack area, from var_16_bit to here
    .text                       ;BP (put the following data in ROM(program memory))
    .global _main               ;BP
msec:                           ;this isn't where we start - it's a subroutine
				; it makes the microcontroller "waste" 1 ms
    push    w1                  ;save WREG1
    mov     #5331,w1		; how did we come up with this magic number?
msloop:
    sub     #1,w1		; decrement w1
    bra     NZ,msloop		; branch to msloop if the result of the last
				; operation was NotZero
    nop				; do nothing (fine-tuning our 1ms delay)
    pop     w1                  ;restore WREG1

    return
msecs:                          ;another subroutine
    push    w0                  ;save WREG0
mssloop:
    dec	    WREG0
    bra     N,mssdone		; branch if Negative
    call    msec
    bra     mssloop		; branch unconditionally to mssloop
				; did you notice that we used two different
				; methods of creating loops in msec and msecs?
				; just so that you see more examples...
mssdone:
    pop     w0                  ;restore WREG0
    return
;because we are using the C compiler to assemble our code, we need a "_main" label
;somewhere. (There's a link step that looks for it.)
_main:
    bclr    CLKDIV,#8                 ;BP
    mov     #stack,w15                ;initialize the stack pointer
    mov     #0xffff,w0	; (you can't direclty load a constant to LATB, so use w0 as the middle-man)
    mov     w0,LATB                   ;make all bits in LATB SFR 1
    mov     #3,w0
    mov     w0,TRISB                  ;make bits 15-2 of PORTB outputs (RB0 and RB1 remain inputs)
    mov     #0x9fff,w0
    mov     w0,AD1PCFG                ;PORTB will be digital, not analog!

mainloop:
    bset    LATB, #2	    ; bset stands for bit-set. Sets bit #2 of LATB
    mov     #40,w0
    call    msecs

    clr	    w0		    ; clear all bits of w0
    mov	    w0, LATB	    ; can we eliminate the prev instruction and directly say clr LATB? Try.
    mov     #40,w0
    call    msecs
    bra     mainloop
    .end

