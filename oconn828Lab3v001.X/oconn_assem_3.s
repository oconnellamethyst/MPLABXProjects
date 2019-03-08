.include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))

; This is a library, thus it can *not* contain a _main function: the C file will
; deine main().  However, we
; we will need a .global statement to make available ASM functions to C code.
; All functions utilized outside of this file will need to have a leading 
; underscore (_) and be included in a comment delimited list below.
.global _write_0, _write_1, _ado_wait_100us, _ado_wait_1ms
    
_ado_wait_100us:
                     ; 2 cycles for function call, this takes 1600 cycles
    repeat #1593     ; 1 cycle to load and prep
    nop              ; X+1 cycles to execute NOP X+1 times, 1600-7=1593
    return           ; 3 cycles for the return
    
_ado_wait_1ms:
                     ; 2 cycles for function call
    repeat #15993 ; 1 cycle to load and prep
    nop              ; X+1 cycles to execute NOP X+1 times
    return           ; 3 cycles for the return 
    
_write_0:
    inc     LATA     ; set pin RA0 low = 1 cycle
    repeat #3       ; THESE ARE UNDER, 6
    nop
    
    clr     LATA     ; set pin RA0 high = 1 cycle
    repeat #4       ; THESE ARE UNDER, 14, Measured 18, sub 4
    nop
                     ; Total = 24 cycles high, 32 cycles low
    return 
    
_write_1:
    inc     LATA     ; set pin RA0 low = 1 cycle
    repeat #9       ; THESE ARE UNDER, 6
    nop
    
    clr     LATA     ; set pin RA0 high = 1 cycle
                     ; Total = 24 cycles high, 32 cycles low
    return 

