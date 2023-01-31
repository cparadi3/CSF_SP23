#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "uint256.h"

// Create a UInt256 value from a single uint64_t value.
// Only the least-significant 64 bits are initialized directly,
// all other bits are set to 0.
UInt256 uint256_create_from_u64(uint64_t val) {
  UInt256 result;
  result.data[0] = val;
  for(int i = 1; i < 4; i++) {
    result.data[i] = 0U;
  }
  return result;
}

// Create a UInt256 value from an array of 4 uint64_t values.
// The element at index 0 is the least significant, and the element
// at index 3 is the most significant.
UInt256 uint256_create(const uint64_t data[4]) {
  UInt256 result;
  for (int i = 0; i < 4; i++) {
    result.data[i] = data[i];
  }
  return result;
}

// Create a UInt256 value from a string of hexadecimal digits.
UInt256 uint256_create_from_hex(const char *hex) {
  UInt256 result;
  // TODO: implement
  /*
  char hexDigits[64] = {'0'};
  for (int i = 0; i < 64; i++) {
    if (&(hex + i) == NULL) {
      break;
    }
    hexDigits[i] = &(hex + i);
  }
  string uint1, uint2, uint3, uint4 = "";
  for (int i = 0; i < 16; i++) {
    uint1 += hexDigits[i];
    uint2 += hexDigits[i + 16];
    uint3 += hexDigits[i + 32];
    uint4 += hexDigits[i + 48];
  }
  */
  //FREE THIS
  char *ptr = strdup(hex);
  ptr += 16;
  result.data[0] = strtoul((hex), &(ptr), 16);
  char* ptr2 = ptr + 16;
  result.data[1] = strtoul(ptr, &ptr2, 16);
  ptr += 32;
  result.data[2] = strtoul(ptr2, &ptr, 16);
  ptr2 += 32;
  result.data[3] = strtoul(ptr, &ptr2, 16);
  //free memory
  return result;
}

// Return a dynamically-allocated string of hex digits representing the
// given UInt256 value.
char *uint256_format_as_hex(UInt256 val) {
  char *hex = NULL;
  // TODO: implement
  return hex;
}

// Get 64 bits of data from a UInt256 value.
// Index 0 is the least significant 64 bits, index 3 is the most
// significant 64 bits.
uint64_t uint256_get_bits(UInt256 val, unsigned index) {
  uint64_t bits;
  bits = val.data[index];
  return bits;
}

// Compute the sum of two UInt256 values.
UInt256 uint256_add(UInt256 left, UInt256 right) {
  UInt256 sum;
  uint64_t carry = 0U;
  // TODO: implement
  for (int i = 0; i < 4; i++) {
    sum.data[i] = left.data[i] + right.data[i] + carry;
    if ((sum.data[i] < right.data[i]) || (sum.data[i] < left.data[i])) {
      carry = 1U;
    }
    else {
      carry = 0U;
    }
  }
  return sum;
}

// Compute the difference of two UInt256 values.
UInt256 uint256_sub(UInt256 left, UInt256 right) {
  UInt256 result;
  UInt256 temp;
  UInt256 one;
    for (int i = 0; i < 4; i++) {
      one.data[i] = (i == 0) ? 1U : 0U;
      temp.data[i] = ~right.data[i];
    }
  right = uint256_add(temp, one);
  result = uint256_add(left, right);
  return result;
}

// Compute the product of two UInt256 values.
UInt256 uint256_mul(UInt256 left, UInt256 right) {
  UInt256 product;
  // TODO: implement
  return product;
}
