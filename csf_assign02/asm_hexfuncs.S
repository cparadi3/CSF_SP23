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

.globl hex_format_byte_as_hex
/*
* Format a byte value (in the range 0-255) as string consisting
* of two hex digits.  The string is stored in sbuf.
*
* Parameters: 
*	sbuf - 
*   byteval - 
* 
* Returns:
*	nothing
*/
hex_format_byte_as_hex:
	subq $8, %rsp       /* align stack */
	movq %rdi, %rax     /* move input value into rdx */
	movq $16, %r11		/* move 16 into %r11 */
	movq $0, %rdx		/* set to zero for unsigned division */
	divq %r11           /*divide %rax by 16 - quotient stored in rax & rmdr stored in rdx */
	movq $0, %r12		/* %r12 becomes index for inserting in buf (rsi)*/
	movq %rdx, %r10 	/* move %rdx to %r10 so %rdx can be overwritten */
	movq $hexVals, %rdx		/* move pointer to hexVals into %rdx */
	movq (%rdx, %rax, 1), %r13 	/* use quotient as index in rdx to find first hex value, store to %r13 */
	movq %r13, (%rsi, %r12, 1) 	/*move %r13 to the %r12 (counter) index in buf (%rsi) */
	incl %r12d			/* increment counter */
	movq (%rdx, %r10, 1), %r13 	/* use remainder as index in rdx to get second hex value, store in %r13 */
	movq %r13, (%rsi, %r12, 1)	/*move %r13 to the %r12 (counter) index in buf (%rsi) */
	incl %r12d				/* increment counter */
	movq $0, (%rsi, %r12, 1)	/* move null terminator into final inedex of buf */
	addq $8, %rsp
	ret

.globl hex_format_offset

hex_format_offset:
	subq $8, %rsp 	/*aligtn stack */
	movq %rdi, %rax /*Moving offset into rax ? temp?*/
	/*movl $28, %esi /*Setting up index counter for offset in r11*/
	movq $0, %r13 /*Freeing up r13 to hold the char in hexVals*/
	movq $0, %r12 /*Freeing up r12 to hold loop counter*/
	movq $0, %r10 /*Temp val to to change offset*/
	movq $7, %r14 /* max shift value that will be changed incrementally */
	movq $hexVals, %rdx /*Moving hexVals pointer into rdx*/
	jmp .LhexLoop
.LhexLoop:
	/*movq %rax, %r10    /*move offset value into temp value */
	subq %r10, %rax 	/*subtract last iterations temp from offset*/
	movq %rax, %r10		/* move new offset into current temp */
	movq $0, %r15        /*counter for shifting loop stored here */
	cmp $7, %r12		/* check if r10 needs to be shifted at all (won't on last iteration) */
	jb .LrightShift
	jae .LafterRight
.LafterRight:	
	movq (%rdx, %r10, 1), %r13	/* use quotient as index in rdx to find first hex value, store to %r13 */
	movq %r13, (%rsi, %r12, 1) 	/*move %r13 to the %r12 (counter) index in buf (%rsi) */
	movq $0, %r15
	cmp $7, %r12			/*skip shifts on last time through loop */
	jb .LleftShift
	jae .LafterLeft
	/*shr $8, %r10 /*right shift offset by index*4 , getting index of hexvals to put in sbuf, then decrement index and run loop again */
	/*shl $4, %r14 */
.LafterLeft:
	/* movq %r10, %r14 	/* move current temp into "last iteration's" temp */
	incl %r12d			/* increment counter */
	/*subl $4, %esi /*Decrement the shift*/
	decl %r14d
	cmp $7, %r12d  /*Check to see if r12d out of range*/
	jbe .LhexLoop
	ja .Ldone /*Jump to end code*/
.LrightShift:
	shr $4, %r10		/* right shift r10 by 4 (divide by 16) */
	incl %r15d			/*increment counter */
	cmp %r14d, %r15d
	jb .LrightShift
	jae .LafterRight
.LleftShift:
	shl $4, %r10
	incl %r15d
	cmp %r14d, %r15d
	jb .LleftShift
	jae .LafterLeft
.Ldone:
	/*incl %r12d */
	movq $0, (%rsi, %r12, 1) /*Add null terminator to end of sbuf*/
	addq $8, %rsp
	ret

.globl hex_read

hex_read:
	subq $8, %rsp
	movq %rdi, %rsi
	movq $0, %rdi
	movq $16, %rdx
	call read
	addq $8, %rsp
	ret


.globl hex_write_string

hex_write_string:
	subq $8, %rsp
	call strLen
	cmp $16, %rax
	jbe .LFinished
	ja .L16Len
.L16Len:
	movq $16, %rax
	jmp .LFinished
.LFinished: 
	movq %rdi, %rsi
	movq $1, %rdi
	movq %rax, %rdx
	call write
	addq $8, %rsp
	ret
/* vim:ft=gas:
 */
