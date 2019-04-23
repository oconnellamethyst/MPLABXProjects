

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
    ;;calculation: 2 cycles to call + 1 cycle to set repeat + 1594 cycles to repeat + 3 cycles to return = 1600 cycles. 
    
_ms_wait:
    repeat #15993
    nop
    return
    
    ;;calculation: 2 cycles to call + 1 cycle to set repeat + 15994 cycles to repeat + 3 cycles to return = 16000 cycles
_write_0:
    inc LATA   ;;2 cycles to call, 1 cycle to inc. RA0 = Low
    repeat #4  ;;1 cycle to set repeat. RA0 is high
    nop        ;;5 cycles to repeat. RA0 = high
    clr LATA   ;;1 cycle to clear, RA0 = high
    repeat #5  ;;1 cycle to set repeat, RA0 = low
    nop        ;;6 cycles to repeat, RA0 = Low
    return     ;;3 cycles to return, RA0 = low
_write_1:
    inc LATA    ;;2 cycles to call, 1 cycle to inc. RA0 = low
    repeat #9   ;;1 cycle to set, RA0 = high
    nop         ;;repeat 10 cycles, RA0 = high
    clr LATA    ;;1 cycle to clear, RA0 = high
    repeat #2   ;;1 cycle to set, RA0 = low
    nop         ;;3 cycles to repeat. RA0 = low
    return      ;;3 cycles to return. RA0 = low
    