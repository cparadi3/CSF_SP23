// C implementation of hexdump functions

#include <unistd.h>  // this is the only system header file you may include!
#include "hexfuncs.h"

// TODO: add function implementations here

// Read up to 16 bytes from standard input into data_buf.
// Returns the number of characters read.
unsigned hex_read(char data_buf[]) {
    unsigned charsRead = read(0, data_buf, 16);
    return charsRead;
}

// Write given nul-terminated string to standard output.
void hex_write_string(const char s[]) {
    int length = 0;
    char tempChar;
    for (int i = 0; i < 32; i++) {
        tempChar = s[i];
        if (tempChar == '\0') {
            break;
        }
        length++;
    }
    write(1,s,length);
}

// Format an unsigned value as an offset string consisting of exactly 8
// hex digits.  The formatted offset is stored in sbuf, which must
// have enough room for a string of length 8.
void hex_format_offset(unsigned offset, char sbuf[]) {
    char hexVals[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    unsigned powersOfSixteen[8] = {0,16,256,4096,65536,1048576,16777216,268435456};
    int index = 0;
    unsigned tempVal = 0;
    for (int i = 7; i > 0; i--) {
        offset = offset - tempVal;
        sbuf[index] = hexVals[offset / powersOfSixteen[i]];
        tempVal = powersOfSixteen[i] * (offset/powersOfSixteen[i]);
        index++;
    }
    sbuf[7] = hexVals[offset % 16]; 
    sbuf[8] = 0;
}

// Format a byte value (in the range 0-255) as string consisting
// of two hex digits.  The string is stored in sbuf.
void hex_format_byte_as_hex(unsigned char byteval, char sbuf[]) {
    char hexVals[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    sbuf[0] = hexVals[byteval / 16];
    sbuf[1] = hexVals[byteval % 16];
}

// Convert a byte value (in the range 0-255) to a printable character
// value.  If byteval is already a printable character, it is returned
// unmodified.  If byteval is not a printable character, then the
// ASCII code for '.' should be returned.
char hex_to_printable(unsigned char byteval) {
    if (byteval > 127 || byteval < 32) {
        return 46;
    }
    return byteval;
}