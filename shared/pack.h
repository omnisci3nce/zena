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
  uint32_t val = ((**buf) << 24);
  (*buf)++;
  val |= ((**buf) << 16);
  (*buf)++;
  val |= ((**buf) << 8);
  (*buf)++;
  val |= (**buf);
  (*buf)++;
  return val;
}

int pack_string(uint8_t **buf, char *val) {
  size_t string_len = strlen(val);
  string_len++;  // add one for \0
  // pack string length
  pack_u32(buf, string_len);
  // pack string
  strcpy(*buf, val);
  (*buf) += string_len;

  return 4 + string_len;
}

uint32_t unpack_string(const uint8_t **buf, char **output) {
  // unpack length
  uint32_t string_len = unpack_u32(buf);

  printf("contents len: %d\n", string_len);
  // unpack string

  char *str = malloc((string_len) * sizeof(char));  // + 1 for '\0' terminator
  strcpy(str, (char *)(*buf));
  *output = str;
  (*buf) += string_len;

  return 4 + string_len;
}

#endif
