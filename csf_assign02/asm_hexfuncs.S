/* Assembly language implementation of hexdump functions */

	.section .rodata
	hexVals: .string "0123456789abcdef"

	/* TODO: add read-only data such as strings here */

	.section .text
/*
 * Determine the length of specified character string.
 *
 * Parameters:
 *   s - pointer to a NUL-terminated character string
 *
 * Returns:
 *    number of characters in the string
 */
	.globl strLen
strLen:
	subq $8, %rsp                 /* adjust stack pointer */
	movq $0, %r10                 /* initial count is 0 */

.LstrLenLoop:
	cmpb $0, (%rdi)               /* found NUL terminator? */
	jz .LstrLenDone               /* if so, done */
	inc %r10                      /* increment count */
	inc %rdi                      /* advance to next character */
	jmp .LstrLenLoop              /* continue loop */

.LstrLenDone:
	movq %r10, %rax               /* return count */
	addq $8, %rsp                 /* restore stack pointer */
	ret

/*
* Determine if character is a printable value.
*
* Parameters: 
*	byteval - value of ASCII value of character
*
* Returns:
*	bytevalue if it is printable, 46 if not (default value)
*/
	.globl hex_to_printable
hex_to_printable: 
	subq $8, %rsp		/* adjust stack pointer */
	cmp $127, %rdi		/* compare byteval to 127 */
	ja .Lperiod			/* if byteval is above 127, jump to func that sets byteval to default*/
	cmp $32, %rdi		/* compare byteval to 32*/
	jb .Lperiod			/* if byteval is below 32, jump to func that sets byteval to default*/
	jmp .LfuncDone		/* jump to end of function*/
.Lperiod:
	movq $46, %rdi		/* puts 46 into rdi*/
.LfuncDone:
	movq %rdi, %rax		/* moves byteval to rax to return*/
	addq $8, %rsp		/* restore stack pointer*/
	ret

.globl hex_format_offset
hex_format_offset:
	/* TODO: implement */
	ret

.globl hex_format_byte_as_hex
hex_format_byte_as_hex:
	/* TODO: implement */
	ret

/* vim:ft=gas:
 */
