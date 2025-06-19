
; Simple kernel entry point
[BITS 32]
global _start
extern kernel_main

section .text
_start:
    ; Set up stack
    mov esp, stack_top
    
    ; Call C kernel
    call kernel_main
    
    ; Hang if we return
    cli
    hlt
    jmp $

section .bss
align 4
stack_bottom:
    resb 16384 ; 16 KiB
stack_top:
