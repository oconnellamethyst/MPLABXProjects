        .equ __P24FJ64GA002,1        ;required "boiler-plate" (BP)
        .include "p24Fxxxx.inc"      ;BP

#include "xc.inc"                    ;BP
        ;the next two lines set up the actual chip for operation - required
        config __CONFIG2, POSCMOD_NONE & I2C1SEL_PRI & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSECME & FNOSC_FRCPLL & SOSCSEL_LPSOSC & WUTSEL_FST & IESO_OFF
        config __CONFIG1, FWDTEN_OFF & BKBUG_OFF & GWRP_OFF & GCP_OFF & JTAGEN_OFF

        .global ALLVar
        .global BLLVar
        .global CLLVar
        .global op1
    .bss                        ;put the following labels in RAM
ALLVar:  .space  8          ;declare a 64-bit variable
BLLVar:  .space  8          ;declare a 64-bit variable
CLLVar:  .space  8          ;declare a 64-bit variable
stack:
    .space  32                  ;this will be our stack area
    .text                       ;BP (put the following data in ROM(program memory))
    .global _main               ;BP
add64_version1:                           ;this isn't where we start - it's a subroutine
    add     w4,w0,w0 ; mov ALL, w0, mov ALL+2 w1, mov BLL w4, mov BLL+2, w5, mov BLL+4, w6...
    addc    w5,w1,w1
    addc    w6,w2,w2
    addc    w7,w3,w3
    return
add64_version2:
    mov     [w1++],w3
    add     w3,[w2++],[w0++]
    mov     [w1++],w3
    addc    w3,[w2++],[w0++]
    mov     [w1++],w3
    addc    w3,[w2++],[w0++]
    mov     [w1++],w3
    addc    w3,[w2++],[w0++]

    return

getPC:		    ; ADVANCED. you can safely skip
    mov     PCL,w0  ; ADVANCED. you can safely skip
    return          ; ADVANCED. you can safely skip


test1:
        .WORD   0xAAAA, 0xBBBB, 0xCCCC, 0xDDDD    ; represents the 64-bit value
						  ; DDDD CCCC BBBB AAAA
        .WORD   0xEEEE, 0xFFFF, 0x1111, 0x3456
test2:  
        .WORD   0xDDDD, 0xAAAA, 0x1234, 0xffff
        .WORD   0x1111, 0x1111, 0x3432, 0x6576
test3:  
        .word   0x1a2b, 0xffff, 0xeeee, 0xaaaa
        .word   0x8888, 0x9732, 0x2a2a, 0xabcd
aSampleString:
        .ascii "Hello World!\0"
;because we are using the C compiler to assemble our code, we need a "_main" label
;somewhere. (There's a link step that looks for it.)
_main:
    bclr    CLKDIV,#8                 ;BP
    mov     #stack,w15                ;initialize the stack pointer
    mov     #0xffff,w0
    mov     w0,LATB                   ;make all bits in LATB SFR 1
    mov     #3,w0
    mov     w0,TRISB                  ;make bits 15-2 of PORTB outputs (RB0 and RB1 remain inputs)
    mov     #0x9fff,w0
    mov     w0,AD1PCFG                ;PORTB will be digital, not analog!
    bset    TRISA,#0                  ;PORTA, bit 0, will be an input (safety first!)
    bset    CNPU1,#2                  ;turns on internal Pull UP on CN01 (RA0)

    clr     PSVPAG
    bset    CORCON,#PSV

    mov     #psvoffset(test1), w8
                    ; psvoffset is a compiler directive that takes the address
                    ; of the op1 label, and sets the most significant bit so that
                    ; the uController understands you are referring to prog mem
                    ; and not data mem.


    mov     #ALLVar,w0              ; make sure you understand the difference between
                                    ; ALLVar (in data mem, currently has val 0) and
                                    ; w8 (mapped to prog space because its msb=1)
                                    ; which currently holds the constants
    mov	    [w0], w10
    mov	    [w8], w11
copyData:
    mov     #ALLVar,w9
    repeat  #7		    ; repeat next instruction 8 times
    mov     [w8++],[w9++]   ; results in 8 words to be copied from prog to data
			    ; memory

testFunctions:
    mov     #ALLVar,w1
    mov     #BLLVar,w2
    mov     #CLLVar,w0
    rcall   add64_version2

    ; now test add64_version1
    mov	    #ALLVar, w10
    mov	    [w10++], w0
    mov	    [w10++], w1
    mov	    [w10++], w2
    mov	    [w10++], w3
    ; now w10 is at BLLVar
    mov	    [w10++], w4
    mov	    [w10++], w5
    mov	    [w10++], w6
    mov	    [w10++], w7
    
    rcall   add64_version1
    
    mov     #CLLVar,w4
    mov	    w0, [w4++]
    mov	    w1, [w4++]
    mov	    w2, [w4++]
    mov	    w3, [w4++]
    
lpbk:
    bra     lpbk	; ensure we won't go past this point
    
    
; ADVANCED: an alternative method for getting the address of test1
; and reading from prog mem.
; All students: you can safely ignore the lines below... Read only if you are
;    comfortable with everything else and would like to learn more about reading
;    from program memory and how it works.
; the lines below try to explaing how "mov #psvoffset(test1), w8" by mimicking
; its fetching the address in program memory, and by settting the most sig bit
; in the address.
    rcall   getPC    ; has the prog addr of "return" after that mov pcl inst.
    mov [w0], w1     ; reads the value from DATA memory address 0x2B6
    bset w0, #15
    mov [w0], w1     ; reads the value from PROG mem address 0x2B6, which is the opcode for return
    inc2 WREG0       ; skip the "return" inst opcode and point to op1
    mov [w0], w1     ; reads PROG mem value from address 0x2B8, which is our 0xAAAA
    mov w0, w8       ; if you want to access other rows of data, add an offset value to w8
    
    .end


