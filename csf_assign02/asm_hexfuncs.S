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
	cmp $126, %rdi		/* compare byteval to 127 */
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
	movq $0, %r9		/* %r12 becomes index for inserting in buf (rsi)*/
	movq %rdx, %r10 	/* move %rdx to %r10 so %rdx can be overwritten */
	movq $hexVals, %rdx		/* move pointer to hexVals into %rdx */
	movq (%rdx, %rax, 1), %r8 	/* use quotient as index in rdx to find first hex value, store to %r13 */
	movq %r8, (%rsi, %r9, 1) 	/*move %r13 to the %r12 (counter) index in buf (%rsi) */
	incl %r9d			/* increment counter */
	movq (%rdx, %r10, 1), %r8 	/* use remainder as index in rdx to get second hex value, store in %r13 */
	movq %r8, (%rsi, %r9, 1)	/*move %r13 to the %r12 (counter) index in buf (%rsi) */
	incl %r9d				/* increment counter */
	movq $0, (%rsi, %r9, 1)	/* move null terminator into final inedex of buf */
	addq $8, %rsp
	ret

.globl hex_format_offset

hex_format_offset:
	subq $8, %rsp 	/*aligtn stack */
	movq %rdi, %rax /*Moving offset into rax ? temp?*/
	/*movl $28, %esi /*Setting up index counter for offset in r11*/
	movq $0, %r8 /*Freeing up r8 to hold the char in hexVals*/
	movq $0, %r9 /*Freeing up r9 to hold loop counter*/
	movq $0, %r10 /*Temp val to to change offset*/
	movq $7, %r11 /* max shift value that will be changed incrementally */
	movq $hexVals, %rdx /*Moving hexVals pointer into rdx*/
	jmp .LhexLoop
.LhexLoop:
	/*movq %rax, %r10    /*move offset value into temp value */
	subq %r10, %rax 	/*subtract last iterations temp from offset*/
	movq %rax, %r10		/* move new offset into current temp */
	movq $0, %rcx        /*counter for shifting loop stored here */
	cmp $7, %r9		/* check if r10 needs to be shifted at all (won't on last iteration) */
	jb .LrightShift
	jae .LafterRight
.LafterRight:	
	movq (%rdx, %r10, 1), %r8	/* use quotient as index in rdx to find first hex value, store to %r18 */
	movq %r8, (%rsi, %r9, 1) 	/*move %r8 to the %r9 (counter) index in buf (%rsi) */
	movq $0, %rcx
	cmp $7, %r9			/*skip shifts on last time through loop */
	jb .LleftShift
	jae .LafterLeft
.LafterLeft:
	incl %r9d			/* increment counter */
	decl %r11d			/* decrement shift counter */
	cmp $7, %r9d  /*Check to see if r12d out of range*/
	jbe .LhexLoop
	ja .Ldone /*Jump to end code*/
.LrightShift:
	shr $4, %r10		/* right shift r10 by 4 (divide by 16) */
	incl %ecx			/*increment counter */
	cmp %r11d, %ecx		/* loop conditional */
	jb .LrightShift
	jae .LafterRight
.LleftShift:
	shl $4, %r10	/* left shift r10 by 4 (multiply by 16) */
	incl %ecx		/* increment counter */
	cmp %r11d, %ecx	/* loop conditional */
	jb .LleftShift
	jae .LafterLeft
.Ldone:
	movq $0, (%rsi, %r9, 1) /*Add null terminator to end of sbuf*/
	addq $8, %rsp		/* realign stack */
	ret

.globl hex_read

hex_read:
	subq $8, %rsp		/*align stack */
	movq %rdi, %rsi		/* get arguments in the correct registers for read function */
	movq $0, %rdi
	movq $16, %rdx
	call read
	addq $8, %rsp	/* align stack */
	ret


.globl hex_write_string

hex_write_string:
	subq $8, %rsp		/* align stack */
	movq %rdi, %r9		/* move rdi into temp value */
	call strLen
	cmp $16, %rax		/* check if string is greater than 16 */
	jbe .LFinished
	ja .L16Len
.L16Len:
	movq $16, %rax		/* if greater than 16, only write the first 16 */
	jmp .LFinished
.LFinished: 		/*otherwise, write strlen number of chars */
	movq %r9, %rsi	/* move temp into rsi */
	movq $1, %rdi	/* move one into rdi (first argument) */
	movq %rax, %rdx	/* move strlen value into rdx */
	call write
	addq $8, %rsp	/* align stack */
	ret
/* vim:ft=gas:
 */
