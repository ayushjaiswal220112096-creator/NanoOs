
; Simple bootloader that enters protected mode and jumps to kernel
[BITS 16]
[ORG 0x7C00]

; Constants
CODE_SEG equ 0x08
DATA_SEG equ 0x10
KERNEL_OFFSET equ 0x1000

start:
    ; Set up segments and stack
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    
    ; Print 'B' to show bootloader is running
    mov ah, 0x0E
    mov al, 'B'
    int 0x10
    
    ; Load kernel from disk
    mov [BOOT_DRIVE], dl    ; Save boot drive
    call load_kernel
    
    ; Print 'K' to show kernel loaded
    mov ah, 0x0E
    mov al, 'K'
    int 0x10
    
    ; Enter protected mode
    call switch_to_pm
    
    ; We never return from switch_to_pm
    jmp $

; Load kernel from disk
load_kernel:
    pusha
    ; Set up disk read
    mov ah, 0x02        ; BIOS read function
    mov al, 20          ; Number of sectors to read
    mov ch, 0           ; Cylinder 0
    mov dh, 0           ; Head 0
    mov cl, 2           ; Start from sector 2
    mov dl, [BOOT_DRIVE]; Drive number
    
    ; Set up memory location to read to
    mov bx, KERNEL_OFFSET
    
    ; Perform the read
    int 0x13
    jc disk_error       ; Jump if error (carry flag set)
    
    ; Check if we read the right number of sectors
    cmp al, 20
    jne disk_error
    
    popa
    ret

disk_error:
    ; Print 'D' for disk error
    mov ah, 0x0E
    mov al, 'D'
    int 0x10
    jmp $               ; Hang

; GDT
gdt_start:
    ; Null descriptor
    dd 0x0
    dd 0x0
    
    ; Code segment descriptor
    dw 0xFFFF           ; Limit (bits 0-15)
    dw 0x0              ; Base (bits 0-15)
    db 0x0              ; Base (bits 16-23)
    db 10011010b        ; Access byte
    db 11001111b        ; Flags + Limit (bits 16-19)
    db 0x0              ; Base (bits 24-31)
    
    ; Data segment descriptor
    dw 0xFFFF           ; Limit (bits 0-15)
    dw 0x0              ; Base (bits 0-15)
    db 0x0              ; Base (bits 16-23)
    db 10010010b        ; Access byte
    db 11001111b        ; Flags + Limit (bits 16-19)
    db 0x0              ; Base (bits 24-31)
gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1  ; Size of GDT
    dd gdt_start                ; Address of GDT

; Switch to protected mode
switch_to_pm:
    cli                     ; Disable interrupts
    lgdt [gdt_descriptor]   ; Load GDT
    
    ; Enable protected mode
    mov eax, cr0
    or eax, 0x1
    mov cr0, eax
    
    ; Far jump to flush pipeline
    jmp CODE_SEG:init_pm
    
; Variables
BOOT_DRIVE db 0

; Protected mode initialization
[BITS 32]
init_pm:
    ; Set up segment registers
    mov ax, DATA_SEG
    mov ds, ax
    mov ss, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ebp, 0x90000
    mov esp, ebp
    
    ; Print 'P' to show we're in protected mode (position 160 = row 2, col 0)
    mov ebx, 0xB8000 + 160
    mov al, 'P'
    mov ah, 0x0F
    mov [ebx], ax
    
    ; Jump to kernel
    call KERNEL_OFFSET
    
    ; We should never get here
    jmp $


; Padding and boot signature
times 510-($-$$) db 0
dw 0xAA55
