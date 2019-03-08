

.include "xc.inc"

.text                       ;BP (put the following data in ROM(program memory))

; This is a library, thus it can *not* contain a _main function: the C file will
; deine main().  However, we
; we will need a .global statement to make available ASM functions to C code.
; All functions utilized outside of this file will need to have a leading 
; underscore (_) and be included in a comment delimited list below.

.global _write_0, _write_1, _micro_wait, _ms_wait

_micro_wait:
    repeat #1593
    nop
    return
    
    
_ms_wait:
    repeat #15993
    nop
    return
_write_0:
    inc LATA
    repeat #4
    nop;
    clr LATA
    repeat #5
    nop
    return
_write_1:
    inc LATA
    repeat #9
    nop
    clr LATA
    repeat #2
    nop
    return
    