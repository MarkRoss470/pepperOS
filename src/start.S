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
    push %es
    push %ds
    mov %cs, %ax
    mov %ax, %es
    mov %ax, %ds
    
    mov $(VESA_arrived_str - VESA_stuff), %esi
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
    
    movw VESA_info_buffer - VESA_stuff, %ax
    //0x56455341 = "VESA"
    //bytes reversed due to little endian nature of x86
    cmpw $0x4556, %ax
    jne VESA_invalid_signature
    
    movw VESA_info_buffer - VESA_stuff + 2, %ax
    cmpw $0x4153, %ax
    jne VESA_invalid_signature
    
    movw $(VESA_valid_signature_str - VESA_stuff), %si
    call print
    
    //version
    movw VESA_info_buffer - VESA_stuff + 4, %ax
    call print_hex
    movw $(VESA_space_str - VESA_stuff), %si
    call print
    //modes segment
    movw VESA_info_buffer - VESA_stuff + 14, %ax
    call print_hex
    movw $(VESA_space_str - VESA_stuff), %si
    call print
    //modes offset
    movw VESA_info_buffer - VESA_stuff + 16, %ax
    call print_hex
    movw $(VESA_ret_str - VESA_stuff), %si
    call print

VESA_print_current_mode:
    pusha
    push %es
    push %di
    
    mov $0x4f03, %ax
    int $0x10
    push %ax
    mov %bx, %ax
    call print_hex
    pop %ax
    cmp $0x004f, %ax
    jne VESA_error
    mov %bx, %cx
    mov $0x1100, %ax
    mov %ax, %es
    mov $0x4f01, %ax
    mov $(VESA_mode_buffer - VESA_stuff), %di
VESA_print_current_mode_int:
    int $0x10
    cmp $0x004f, %ax
    jne VESA_error
VESA_current_mode_info_get_success:
    call VESA_print_chosen_mode_info
    
    pop %di
    pop %es
    popa
VESA_loop_init:
    movw $(VESA_ret_str - VESA_stuff), %si
    call print
    
    movw VESA_info_buffer - VESA_stuff + 14, %bx
    mov VESA_info_buffer - VESA_stuff + 16, %fs
    
#define width_offset $16

VESA_modes_loop:
    mov %fs:(%bx), %ax
    cmp $0xffff, %ax
    je VESA_modes_loop_done
    
    push %ax
    
    mov %ax, %cx
    mov $0x1100, %ax
    mov %ax, %es
    mov $0x4f01, %ax
    mov $(VESA_mode_buffer - VESA_stuff), %di
    
    int $0x10
    
    cmp $0x004f, %ax
    jne VESA_modes_error
    
VESA_compare_modes:
    //check if width greater than width of currently chosen mode
    mov VESA_mode_buffer - VESA_stuff + 18, %ax
    cmp %ax, VESA_chosen_mode_buffer - VESA_stuff + 18
    jg VESA_modes_loop_end
    //TODO:change here to support higher widths
    cmp $0x640, %ax
    jg VESA_modes_loop_end
VESA_width_not_lesser:

    //check if height greater than width of currently chosen mode
    mov VESA_mode_buffer - VESA_stuff + 20, %ax
    cmp %ax, VESA_chosen_mode_buffer - VESA_stuff + 20
    //TODO:change here to support taller hights
    cmp $0x2d0, %ax
    jg VESA_modes_loop_end

    jg VESA_modes_loop_end
VESA_height_not_lesser:
    //check if number of bits equal to 32
    mov VESA_mode_buffer - VESA_stuff + 25, %al
    cmp $32, %al
    
    jne VESA_modes_loop_end

VESA_number_bits_correct:
    //check if linear frame buffer is supported
    mov VESA_mode_buffer - VESA_stuff, %ax
    and $0x90, %ax
    cmp $0x90, %ax
    
    jne VESA_modes_loop_end

VESA_linear_frame_buffer_supported:

    //check if memory model is correct
    mov VESA_mode_buffer - VESA_stuff + 27, %al
    cmp $4, %al
    je VESA_appropriate_memory_model
    cmp $6, %al
    je VESA_appropriate_memory_model
    jmp VESA_modes_loop_end

VESA_appropriate_memory_model:

VESA_select_mode:
    mov %cx, VESA_chosen_mode - VESA_stuff
    call VESA_prefer_mode

    jg VESA_modes_loop_end
    
VESA_modes_loop_end:
    movw %cx, VESA_current_mode - VESA_stuff
    
    movw VESA_current_mode - VESA_stuff + 20, %ax
    cmpw $0x640, %ax
    //jl VESA_dont_print_info
    
    mov VESA_mode_buffer - VESA_stuff + 25, %al
    cmp $16, %al
    jne VESA_dont_print_info
    
    call VESA_print_chosen_mode_info
VESA_dont_print_info:
    pop %ax
    add $2, %bx
    jmp VESA_modes_loop
VESA_modes_loop_done:

    movw $(VESA_ret_str - VESA_stuff), %si
    call print

    call VESA_copy_back_mode
    call VESA_print_chosen_mode_info

    mov VESA_chosen_mode - VESA_stuff, %ax
    cmpw $0xffff, %ax
    je VESA_no_mode_found

    //cli
    //hlt

VESA_set_mode:
    mov $0x4f02, %ax
    mov VESA_chosen_mode - VESA_stuff, %bx
    or $0x4000, %bx
    int $0x10
    
    jmp VESA_ret

VESA_print_chosen_mode_info:
    pusha
    push %si
    push %di
    //mode number
    mov VESA_current_mode - VESA_stuff, %ax
    call print_hex
    movw $(VESA_space_str - VESA_stuff), %si
    call print
    
    //width
    movw VESA_mode_buffer - VESA_stuff + 18, %ax
    call print_hex
    call print
    
    //height
    movw VESA_mode_buffer - VESA_stuff + 20, %ax
    call print_hex
    call print
    
    //memory model
    movw VESA_mode_buffer - VESA_stuff + 27, %ax
    call print_hex
    call print
    
    //bit depth
    movb VESA_mode_buffer - VESA_stuff + 25, %al
    movb $0, %ah
    call print_hex

    movw $(VESA_ret_str - VESA_stuff), %si
    call print
    
    pop %di
    pop %si
    popa
    ret

VESA_no_mode_found:
    movw $(VESA_no_mode_str - VESA_stuff), %si
    call print
    cli
    hlt
    
VESA_prefer_mode:
    pusha
    mov $0, %bx
VESA_prefer_mode_loop:
    
    movw (VESA_mode_buffer - VESA_stuff)(%bx), %ax
    movw %ax, (VESA_chosen_mode_buffer - VESA_stuff)(%bx)
    add $2, %bx
    cmp $0x100, %bx
    jl VESA_prefer_mode_loop
    
    popa
    ret

VESA_copy_back_mode:
    pusha
    
    movw (VESA_chosen_mode - VESA_stuff), %ax
    movw %ax, (VESA_current_mode - VESA_stuff)
    
    mov $0, %bx
VESA_copy_back_mode_loop:
    
    movw (VESA_chosen_mode_buffer - VESA_stuff)(%bx), %ax
    movw %ax, (VESA_mode_buffer - VESA_stuff)(%bx)
    add $2, %bx
    cmp $0x100, %bx
    jl VESA_copy_back_mode_loop
    
    popa
    ret

VESA_modes_error:
    add $2, %sp
    jmp VESA_error

VESA_invalid_signature:
    movw $(VESA_invalid_signature_str - VESA_stuff), %si
    call print
    jmp VESA_ret

VESA_error:
    movw $(VESA_error_str - VESA_stuff), %si
    call print
    cli
    hlt
    
VESA_ret:
    //return to boot sector
    pop %ds
    pop %es
    popa
    
    retf

.global VESA_chosen_mode
.global VESA_chosen_mode_buffer
.global VESA_mode_buffer


VESA_info_buffer:
    .space 0x200
VESA_current_mode:
    .space 2
VESA_mode_buffer:
    .space 0x100
VESA_chosen_mode:
    .word 0xffff
VESA_chosen_mode_buffer:
    .space 0x100


VESA_dot_str:
    .asciz "."
VESA_ret_str:
    .asciz "\n\r"
VESA_space_str:
    .asciz " "
VESA_arrived_str:
    .asciz "VESA code reached\n\r"
VESA_success_str:
    .asciz "VESA info collected successfully\n\r"
VESA_error_str:
    .asciz "Error collecting VESA info\n\r"
VESA_invalid_signature_str:
    .asciz "Invalid signature\n\r"
VESA_valid_signature_str:
    .asciz "Valid signature\n\r"
VESA_test_str:
    .asciz "VESA test string\n\r"
VESA_no_mode_str:
    .asciz "No supported VESA mode found\n\r"

/* prints string in %ds:si */
print:
    pusha
print_loop:
    xorb %bh, %bh
    movb $0x0E, %ah

    lodsb

    /* NULL check */
    cmpb $0, %al
    je print_ret

    /* print %al to screen */
    int $0x10
    jmp print_loop

print_ret:
    popa  
    ret

/*prints hex value from ax*/
print_hex:
    pusha
    push %ds
    
    push %ax
    mov %cs, %ax
    mov %ax, %ds
    pop %ax
    
    mov %ax, %cx
    mov $0x0e, %ah
    
    mov %cx, %dx
    shr $12, %dx
    and $0xf, %dx
    mov $(VESA_hex_str - VESA_stuff), %bx
    add %dx, %bx
    mov (%bx), %al
    int $0x10
    
    mov %cx, %dx
    shr $8, %dx
    and $0xf, %dx
    mov $(VESA_hex_str - VESA_stuff), %bx
    add %dx, %bx
    mov (%bx), %al
    int $0x10
    
    mov %cx, %dx
    shr $4, %dx
    and $0xf, %dx
    mov $(VESA_hex_str - VESA_stuff), %bx
    add %dx, %bx
    mov (%bx), %al
    int $0x10
    
    mov %cx, %dx
    shr $0, %dx
    and $0xf, %dx
    mov $(VESA_hex_str - VESA_stuff), %bx
    add %dx, %bx
    mov (%bx), %al
    int $0x10
    
    pop %ds
    popa
    ret
VESA_hex_str: .asciz "0123456789ABCDEF"


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
