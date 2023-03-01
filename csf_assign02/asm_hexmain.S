/* Hexdump main function, assembly language version */
#include "asm_hexfuncs.S"
	.section .rodata
	colon: .string ": "
	space: .string " "
	tripleSpace: .string "   " /*Changed this to reflect what was in c_hexmain*/
	.section .text

	.globl main
main:
	/* TODO: add your code here */
	pushq %rbp
	movq %rsp, %rbp
	subq $48, %rsp
	movq $0, %r12 /*Read variable*/
	movq $0, %r13 /*Counter variable*/
	movl $0, %r14d /*Storing for loop counter*/
	leaq -32(%rbp), %rdi /*Moving the stack pointer that corresponsds to the area we want to store data_buf*/
	call hex_read
	movq %rax, %r12 /*Storing output of read call into r12*/
	jmp .LWhileLoop
.LWhileLoop: 
	cmp $0, %r12
	jae .LEnd /*If no more data to read, end program*/
	movq %r13, %rdi
	movq -48(%rbp), %rsi /*Storing sbuf*/
	call hex_format_offset 
	call hex_write_string
	movq $colon, %rsi
	call hex_write_string
.LForLoop: /*Compares read variable to i*/
	cmp $16, %r14d /*i < 16*/
	je .LEndOfWhile
	cmp %r12d, %r14d /*Comparison for if/else statement !!! Might need to change how we use these registers*/
	jb .LIfStatement 
	jae .LElse
.LIfStatement:
	movq -32(%rbp), %rdi /*Setting up call for hex_format_byte_as_hex*/
	movq -48(%rbp), %rsi /*Setting up call for hex_format_byte_as_hex*/
	call hex_format_byte_as_hex
	movq -48(%rbp), %rdi /*Setting up call for hex_write_string*/
	call hex_write_string
	movq $space, %rdi
	call hex_write_string
	/*Need to figure out this last part of the if statement*/
	incl %r14d
	jmp .LForLoop
.LElse: 
	movq $tripleSpace, %rdi /*Calling hex_write_String*/
	call hex_write_string
	incl %r14d
	jmp .LForLoop
.LEndOfWhile: 
	movq $space, %rdi
	call hex_write_string
	/*    data_buf[read] = '\0'; */
	movq -32(%rbp), %rdi
	call hex_write_string
	/*hex_write_string("\n");*/
	movq -32(%rbp), %rdi 
	call hex_read
	movq %rax, %r12 /*Updating the read variable with next character sequence*/
	jmp .LWhileLoop
.LEnd:
	movq $0, %rax 
	addq $48, %rsp /*Reseting the stack*/
	popq %rbp /*Reseting the stack*/
	ret

/* vim:ft=gas:
 */
