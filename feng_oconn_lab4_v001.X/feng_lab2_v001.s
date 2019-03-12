

.include "xc.inc"          ; required "boiler-plate" (BP)

;the next two lines set up the actual chip for operation - required
config __CONFIG2, POSCMOD_EC & I2C1SEL_SEC & IOL1WAY_OFF & OSCIOFNC_ON & FCKSM_CSECME & FNOSC_FRCPLL & SOSCSEL_LPSOSC & WUTSEL_FST & IESO_OFF
config __CONFIG1, WDTPS_PS1 & FWPSA_PR32 & WINDIS_OFF & FWDTEN_OFF & BKBUG_ON & GWRP_ON & GCP_ON & JTAGEN_OFF

    .bss        ; put the following labels in RAM
counter:
    .space 2    ; a variable that takes two bytes (we won?t use
                ; it for now, but put here to make this a generic
                ; template to be used later).
stack:
    .space  32  ; this will be our stack area, needed for func calls

.text           ; BP (put the following data in ROM(program memory))

;because we are using the C compiler to assemble our code, we need a "_main" label
;somewhere. (There's a link step that looks for it.)
.global _main               ;BP
_main:

    bclr    CLKDIV,#8                 ;BP
    nop
    ;; --- Begin your program below here ---

    mov     #0x9fff,w0  
    mov     w0,AD1PCFG            ; Set all pins to digital mode
    mov     #0b1111111111111110,w0  
    mov     w0,TRISA            ; set pin RA0 to output
    mov     #0x0001,w0  
    mov     w0,LATA            ; set pin RA0 high
    
_micro_wait:
    repeat #1593;
    nop;
    return;
_ms_wait:
    repeat #15993;
    nop;
    return;
    
_write_0:
    bset LATA, #0
    repeat #3
    nop;
    bclr LATA, #0
    repeat #6
    nop
    return
_write_1:
    bset LATA, #0
    repeat #8
    nop
    bclr LATA, #0
    repeat #6
    nop
    return
    
write_bit_stream:
    repeat #19
    nop
    clr LATA
    nop
    nop
    repeat #27
    nop
    inc LATA
 
	
    call _write_1
    call _write_1
    call _write_1
    call _write_1
    call _write_1
    call _write_1
    call _write_1
    call _write_1
    
    call _write_1
    call _write_1
    call _write_1
    call _write_1
    call _write_1
    call _write_1
    call _write_1
    call _write_1
    
    call _write_0
    call _write_0
    call _write_0
    call _write_0
    call _write_0
    call _write_0
    call _write_0
    call _write_0
    call _ms_wait
foreverLoop:
    
    bra     foreverLoop
 
    .end    ; this doesn?t actually end anything. Does not translate to assembly
            ; code. Just a way to tell the compiler we are done with this file.

