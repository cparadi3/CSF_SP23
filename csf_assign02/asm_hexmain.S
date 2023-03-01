/* Hexdump main function, assembly language version */
/*#include "asm_hexfuncs.S"*/
	.section .rodata
	colon: .string ": "
	space: .string " "
	newLine: .string "\n"
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
	leaq -32(%rbp), %rbx /*Moving the stack pointer that corresponsds to the area we want to store data_buf*/
	leaq -48(%rbp), %r15 	/*r15 is sbuf */
	movq %rbx, %rdi
	/*leaq -48(%rbp), %r15 /* moving stack pointer to area corresponding to buf (r15) */
	/* movq %rbx, %rdi		/* move data buf pointer to argument register */
	call hex_read
	movq %rax, %r12 /*Storing output of read call into r12*/
	jmp .LWhileLoop
.LWhileLoop: 
	cmp $0, %r12
	jbe .LEnd /*If no more data to read, end program*/
	movq %r13, %rdi		/*counter is first argument in format offset */
	/*leaq -48(%rbp), %rsi /*Storing sbuf*/
	movq %r15, %rsi		/*move sbuf into rsi */
	/*movq %rax, %rsi */
	call hex_format_offset
	movq %r15, %rdi 		/* move pointer to sbuf back to rdi*/
	call hex_write_string
	movq $colon, %rdi
	call hex_write_string
	movl $0, %r14d
	jmp .LForLoop
.LForLoop: /*Compares read variable to i*/
	cmp $16, %r14d /*i < 16*/
	je .LEndOfWhile
	cmp %r12d, %r14d /*Comparison for if/else statement !!! Might need to change how we use these registers*/
	jb .LIfStatement 
	jae .LElse
.LIfStatement:
	/*leaq -32(%rbp), %rax /*Setting up call for hex_format_byte_as_hex (rdi is data_buf[i])*/
	/*leaq -48(%rbp), %rsi /*Setting up call for hex_format_byte_as_hex (rsi is buf)*/
	/*movq %rbx, %rax*/
	/*addq %r14, %rax 	/* increment index of data_buf by for loop counter*/
	/*movq (%rax), %rdi 	/* dereference pointer to data_buf[i] */
	movq $0, %rdi
	movb (%rbx, %r14, 1), %dil
	movq %r15, %rsi 	/*move sbuf into rsi */
	call hex_format_byte_as_hex
	/*leaq -48(%rbp), %rdi /*Setting up call for hex_write_string*/
	movq %r15, %rdi 	/* move sbuf into rdi */
	call hex_write_string
	movq $space, %rdi
	call hex_write_string
	/*Need to figure out this last part of the if statement*/
	/*data_buf[i] = hex_to_printable(data_buf[i]) */
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
	movq $newLine, %rdi /*Printing new line character*/
	call hex_write_string
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
