// pack / unpack helpers
#ifndef PACK_H
#define PACK_H

#include <stdint.h>

// for now while we're in PoC stage lets just memcpy data into the buffer

void pack_u32(uint8_t *buf, uint32_t val) {
  *buf++ = val >> 24;
  *buf++ = val >> 16;
  *buf++ = val >> 8;
  *buf++ = val;
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

#endif
