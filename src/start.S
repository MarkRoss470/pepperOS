.code32
.section .text.prologue

.global _start
_start:
    movl $stack, %esp
    andl $-16, %esp
    movl $0xDEADBEEF, %eax
    pushl %esp
    pushl %eax
    
    cli
    call _main
    cli
    hlt

.section .text.VESA
.code16

VESA_stuff:
    
    pusha
    mov %cs, %ax
    mov %ax, %es
    mov %ax, %ds
    
    mov $0x0e, %ah
    mov $0x65, %al
    int $0x10
    
    mov $(VESA_test_str - VESA_stuff), %esi
    call print
    
    mov $0x4f00, %ax
    mov $(VESA_info_buffer - VESA_stuff), %di
    int $0x10
    
    /* video mode: 320x200 @ 16 colors */
    //movb $0x00, %ah
    //movb $0x13, %al
    //int $0x10
    
    cmp $0x004f, %ax
    jne VESA_error

VESA_info_success:
    movw $(VESA_success_str - VESA_stuff), %si
    call print
    cli
    hlt
    
    
VESA_error:
    movw $(VESA_error_str - VESA_stuff), %si
    call print
    jmp VESA_ret
    cli
    hlt
    
VESA_ret:
    //return to boot sector
    popa
    retf
    
VESA_info_buffer:
    .space 0x20
VESA_mode_buffer:
    .space 0x100
VESA_test_str:
    .asciz "VESA code reached\n\r"
VESA_success_str:
    .asciz "VESA info produced successfully\n\r"
VESA_error_str:
    .asciz "Error collecting VESA info\n\r"

/* prints string in %ds:si */
print:
    xorb %bh, %bh
    movb $0x0E, %ah

    lodsb

    /* NULL check */
    cmpb $0, %al
    je print_ret

    /* print %al to screen */
    int $0x10
    jmp print

print_ret:  ret

.section .text

.code32

.align 4

.global idt_load
.type idt_load, @function
idt_load:
    mov 4(%esp), %eax
    lidt (%eax)
    ret

.macro ISR_NO_ERR index
    .global _isr\index
    _isr\index:
        cli
        push $0
        push $\index
        jmp isr_common
.endm

.macro ISR_ERR index
    .global _isr\index
    _isr\index:
        cli
        push $\index
        jmp isr_common
.endm

ISR_NO_ERR 0
ISR_NO_ERR 1
ISR_NO_ERR 2
ISR_NO_ERR 3
ISR_NO_ERR 4
ISR_NO_ERR 5
ISR_NO_ERR 6
ISR_NO_ERR 7
ISR_ERR 8
ISR_NO_ERR 9
ISR_ERR 10
ISR_ERR 11
ISR_ERR 12
ISR_ERR 13
ISR_ERR 14
ISR_NO_ERR 15
ISR_NO_ERR 16
ISR_NO_ERR 17
ISR_NO_ERR 18
ISR_NO_ERR 19
ISR_NO_ERR 20
ISR_NO_ERR 21
ISR_NO_ERR 22
ISR_NO_ERR 23
ISR_NO_ERR 24
ISR_NO_ERR 25
ISR_NO_ERR 26
ISR_NO_ERR 27
ISR_NO_ERR 28
ISR_NO_ERR 29
ISR_NO_ERR 30
ISR_NO_ERR 31
ISR_NO_ERR 32
ISR_NO_ERR 33
ISR_NO_ERR 34
ISR_NO_ERR 35
ISR_NO_ERR 36
ISR_NO_ERR 37
ISR_NO_ERR 38
ISR_NO_ERR 39
ISR_NO_ERR 40
ISR_NO_ERR 41
ISR_NO_ERR 42
ISR_NO_ERR 43
ISR_NO_ERR 44
ISR_NO_ERR 45
ISR_NO_ERR 46
ISR_NO_ERR 47

/* defined in isr.c */
.extern isr_handler
.type isr_handler, @function

isr_common:
    pusha
    push %ds
    push %es
    push %fs
    push %gs

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    cld

    push %esp
    call isr_handler
    add $4, %esp

    pop %gs
    pop %fs
    pop %es
    pop %ds

    popa

    add $8, %esp
    iret

.section .data
.align 32
stack_begin:
    .fill 0x4000
stack:
