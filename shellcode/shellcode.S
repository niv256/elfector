BITS 64

global _start

section .text

_start:
    push rdi
    push rsi
    push rdx
    push r15

    call get_rip
get_rip:
    pop r15

    mov rax, 0x01
    mov rdi, 0x01
    lea rsi, [r15 + message - get_rip]
    mov rdx, 0x0a
    syscall

;    mov rax, 0x3c
;    mov rdi, 0x00
;    syscall

    pop r15
    pop rdx
    pop rsi
    pop rdi

    mov rax, 0x401040
    jmp rax

    message: db "elfected!", 0ah
