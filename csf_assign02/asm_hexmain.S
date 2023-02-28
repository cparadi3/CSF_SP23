/* Hexdump main function, assembly language version */
/*#include "asm_hexfuncs.S" */
	.section .rodata
	colon: .string ": "
	space: .string " "
	doubleSpace: .string "  "
	.section .text

	.globl main
main:
	/* TODO: add your code here */
	pushq %rbp
	movq %rsp, %rbp
	subq $48, %rsp
	movq $0, %r12 /*Read variable*/
	movq $0, %r13 /*Counter variable*/
	leaq -32(%rbp), %rdi /*Moving the stack pointer that corresponsds to the area we want to store data_buf*/
	call hex_read
	movq %rax, %r12 /*Storing output of read call into r12*/
	jmp .LWhileLoop
.LWhileLoop: 
	cmp $0, %r12
	jae .LEnd /*If no more data to read, end program*/
	movq %r13, %rdi
	movq -48(%rbp), %rsi
	call hex_format_offset 
	call hex_write_string
	movq $colon, %rsi
	call hex_write_string
	
.LEnd:
	movq $0, %rax
	ret

/* vim:ft=gas:
 */
