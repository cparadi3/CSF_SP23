// C implementation of hexdump main function

#include "hexfuncs.h"  // this is the only header file which may be included!

int main(void) {
  // TODO:

  //read 16 bytes from stdin (maybe do a while loop like while(hex_read == 16))
    //hex_read
  char data_buf[16];
  char sbuf[16];
  unsigned read = hex_read(data_buf);
  unsigned counter = 0;
  while(read > 0) {
    hex_format_offset(counter, sbuf);
    hex_write_string(sbuf);
    hex_write_string(": ");
    for (unsigned i = 0; i < 16; i++) {
      if(i < read) {
      hex_format_byte_as_hex(data_buf[i], (sbuf));
      hex_write_string(sbuf);
      hex_write_string(" ");
      data_buf[i] = hex_to_printable(data_buf[i]);
      counter++;
      }
      else if(i >= read) {
        hex_write_string("   ");
      }
    }
    hex_write_string(" ");
    data_buf[read] = '\0';
    hex_write_string(data_buf);
    hex_write_string("\n");
    read = hex_read(data_buf);
  }
  //print the "position" of the string in the input (should start at zero + go up by 16 per line)
    //hex_format_offset and hex_write_string should be called (and hex_to_printable)
  //for each 16 char's in the line, format as hex
    //call format_byte_as_hex, hex_to_printable, and hex_write_string
  //separate values appropriately by spaces, colons, and newlines.
  

}
