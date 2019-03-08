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
    
    ;Void Setup
    
    mov     #0x9fff,w0  
    mov     w0,AD1PCFG            ; Set all pins to digital mode
    mov     #0b1111111111111110,w0  
    mov     w0,TRISA            ; set pin RA0 to output
    mov     #0x0001,w0  
    mov     w0,LATA            ; set pin RA0 high
    
    call    foreverLoop
    
wait_100ns:
                     ; 2 cycles for function call, this takes 1600 cycles
    repeat #1593     ; 1 cycle to load and prep
    nop              ; X+1 cycles to execute NOP X+1 times, 1600-7=1593
    return           ; 3 cycles for the return
    
wait_1ms:
                     ; 2 cycles for function call
    repeat #15993 ; 1 cycle to load and prep
    nop              ; X+1 cycles to execute NOP X+1 times
    return           ; 3 cycles for the return
    
write_bit_stream:
    repeat #14       ; 24=14 minus two for ftn call, minus two again because... reasons
    nop
    clr     LATA     ; set pin RA0 low = 1 cycle

    repeat #29       ; 32=29
    nop
    inc     LATA     ; set pin RA0 high = 1 cycle
                     ; Total = 24 cycles high, 32 cycles low
    return  
    
write_0:
    inc     LATA     ; set pin RA0 low = 1 cycle
    repeat #3       ; THESE ARE UNDER, 6
    nop
    
    clr     LATA     ; set pin RA0 high = 1 cycle
    repeat #4       ; THESE ARE UNDER, 14, Measured 18, sub 4
    nop
                     ; Total = 24 cycles high, 32 cycles low
    return 
    
write_1:
    inc     LATA     ; set pin RA0 low = 1 cycle
    repeat #9       ; THESE ARE UNDER, 6
    nop
    
    clr     LATA     ; set pin RA0 high = 1 cycle
                     ; Total = 24 cycles high, 32 cycles low
    return 

foreverLoop:
    
    ;r
    call    write_1
    call    write_1
    call    write_1
    call    write_1
    
    call    write_0
    call    write_0
    call    write_0
    call    write_0
    
    ;g
    call    write_1
    call    write_1
    call    write_0
    call    write_0
    
    call    write_0
    call    write_0
    call    write_0
    call    write_0
    
    ;b
    call    write_0
    call    write_0
    call    write_0
    call    write_0
    
    call    write_0
    call    write_0
    call    write_0
    call    write_0    
    
    
    nop
    .end    ; this doesn?t actually end anything. Does not translate to assembly
            ; code. Just a way to tell the compiler we are done with this file.



    
    