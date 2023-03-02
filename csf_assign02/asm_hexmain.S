/* Hexdump main function, assembly language version */
/*#include "asm_hexfuncs.S"*/
	.section .rodata
	colon: .string ": "
	space: .string " "
	newLine: .string "\n"
	nullTerm: .string "0"
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
	call hex_format_offset
	movq %r15, %rdi 		/* move pointer to sbuf back to rdi*/
	call hex_write_string
	movq $colon, %rdi		/*move ": " into rdi */
	call hex_write_string
	movl $0, %r14d		/*initialize for loop counter to zero */
	jmp .LForLoop	
.LForLoop: /*Compares read variable to i*/
	cmp $16, %r14d /*i < 16*/
	je .LEndOfWhile
	cmp %r12d, %r14d /*Comparison for if/else statement */
	jb .LIfStatement 
	jae .LElse
.LIfStatement:
	movq $0, %rdi		/*clear rdi so subregister %dil isn't corrupted */
	movb (%rbx, %r14, 1), %dil		/*get char value from rbx (data_buf) and stor it into rdi subregister */
	movq %r15, %rsi 	/*move sbuf into rsi */
	call hex_format_byte_as_hex
	movq %r15, %rdi 	/* move sbuf into rdi */
	call hex_write_string
	movq $space, %rdi	/* move space string into rdi */
	call hex_write_string
	movq $0, %rdi		/*clear rdi so subregister %dil isn't corrupted */
	movb (%rbx, %r14, 1), %dil		/*get char value from rbx (data_buf) and stor it into rdi subregister */
	call hex_to_printable
	movb %al, (%rbx, %r14, 1) 	/*change character in data_buf to printable */
	incl %r14d			/*increment for loop counter (i) */
	incl %r13d			/*increment "counter" variable (total number of chars read) */
	jmp .LForLoop
.LElse: 
	movq $tripleSpace, %rdi /*Calling hex_write_String*/
	call hex_write_string	/*print three spaces for every empty spot until 16 */
	incl %r14d		/*increment for loop counter (i) */
	jmp .LForLoop
.LEndOfWhile: 
	movq $space, %rdi		/*print one space */
	call hex_write_string
	movq $0, %rdi		/*clear rdi so subregister %dil isn't corrupted */
	movb %dil, (%rbx, %r12, 1) 	/*get char value from rbx (data_buf) and stor it into rdi subregister */
	movq %rbx, %rdi			/* move data_buf into rdi */
	call hex_write_string 
	movq $newLine, %rdi /*Printing new line character*/
	call hex_write_string 
	movq %rbx, %rdi 	/* read next 16 chars */
	call hex_read
	movq %rax, %r12 /*Updating the read variable with next character sequence*/
	jmp .LWhileLoop
.LEnd:
	movq $0, %rax 	/* return 0 */
	addq $48, %rsp /*Resetting the stack*/
	popq %rbp /*Resetting the stack*/
	ret

/* vim:ft=gas:
 */
