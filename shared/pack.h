// pack / unpack helpers
#ifndef PACK_H
#define PACK_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO:
 * - pack/unpack u16
 * - pack/unpack u64
 * - pack/unpack string
*/

int pack_u32(uint8_t **buf, uint32_t val) {
  **buf = val >> 24;
  (*buf)++;
  **buf = val >> 16;
  (*buf)++;
  **buf = val >> 8;
  (*buf)++;
  **buf = val;
  (*buf)++;
  return 4;
}

uint32_t unpack_u32(const uint8_t **buf) {
  uint32_t val = **buf;
  (*buf)++;
  val |= ((**buf) << 8); 
  (*buf)++;
  val |= ((**buf) << 16); 
  (*buf)++;
  val |= ((**buf) << 24); 
  (*buf)++;
  return val;
}

int pack_string(uint8_t **buf, char *val) {
  size_t len = strlen(val);
  len++; // add one for \0
  // pack string length
  pack_u32(buf, len);
  // pack string
  strcpy(*buf, val);
  (*buf) += len;

  return 4 + len;
}

// uint32_t unpack_string(const uint8_t **buf) {
// }

#endif
