[BITS 16]
[ORG 0x7C00]


CODE_OFFSET equ 0x8
DATA_OFFSET equ 0x10
start:
    cli         ; Clear interrupts
    xor ax, ax  ; Set AX to 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti         ; Enable interrupts

load_PM:
    cli
    lgdt[gdt_descriptor]
    mov eax, cr0
    or al, 1
    mov cr0, eax
    jmp CODE_OFFSET:PModeMain

gdt_start:
    dd 0x0
    dd 0x0

    ;Code segment Descriptor
    dw 0xFFFF ; Limit
    dw 0x0000 ; Base
    db 0x00   ; Base
    db 10011010b ; Access Byte
    db 11001111b ; Flags 

    ; Data Segment Descriptor
    dw 0xFFFF ; Limit
    dw 0x0000 ; Base
    db 0x00   ; Base
    db 10010010b ; Access Byte
    db 11001111b ; Flags 

gdt_end:


gdt_descriptor:
    dw gdt_end - gdt_start ; Size of GDT - 1
    dd gdt_start

[BITS 32]
PModeMain:
    mov ax, DATA_OFFSET
    mov ex, ax
    mov fs, ax
    mov ss, ax
    mov gs, ax
    mov ebp, 0x9C00
    mov esp, ebp

    in al, 0x92
    or al, 2
    out 0x92, al

    jmp $

times 510 - ($ - $$) db 0
dw 0xAA55  ; Boot sector signature
