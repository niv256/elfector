BITS 64

global _start

section .text

_start:
    call get_rip
get_rip:
    pop r15

    push rdi
    push rsi
    push rdx

    mov rax, 0x01
    mov rdi, 0x01
    lea rsi, [r15 + message - get_rip]
    mov rdx, 0x0a
    syscall

    pop rdx
    pop rsi
    pop rdi

    mov rax, r15
    sub rax, get_rip
    sub rax, _start
    sub rax, 0xdeadbeef
    jmp rax

    message: db "elfected!", 0ah
