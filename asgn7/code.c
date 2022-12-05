#include "code.h"
#include <stdio.h>

// setting top and bits to 0
Code code_init(void) {
  Code c;
  c.top = 0;
  for (uint8_t i = 0; i < MAX_CODE_SIZE; i++) {
    c.bits[i] = 0;
  }
  return c;
}
// code size is top
uint32_t code_size(Code *c) { return c->top; }
// if the top is empty then return true, else false
bool code_empty(Code *c) {
  if (c->top == 0) {
    return true;
  }
  return false;
}
// if the code is full then return true, else false
bool code_full(Code *c) {
  if (c->top == ALPHABET) {
    return true;
  }
  return false;
}
// if i is in range then set bit
bool code_set_bit(Code *c, uint32_t i) {
  if (i < ALPHABET && i >= 0) {
    c->bits[i / 8] |= ((uint64_t)1 << (i % 8));
    return true;
  }
  return false;
}
// check if i is in range and clear bit
bool code_clr_bit(Code *c, uint32_t i) {
  if (i < ALPHABET && i >= 0) {
    c->bits[i / 8] = c->bits[i / 8] & ~(1 << i % 8);
    return true;
  }
  return false;
}
// check if i is in range and get bit at position
bool code_get_bit(Code *c, uint32_t i) {
  if (i < ALPHABET && i >= 0) {
    return ((c->bits[i / 8] >> i % 8) & (uint64_t)1);
  }
  return false;
}
// check if code is full
// if not then push bit
bool code_push_bit(Code *c, uint8_t bit) {
  if (!code_full(c)) {
    // fprintf(stderr, "got to code push\n");
    if (bit) {
      code_set_bit(c, c->top);
      c->top++;
    } else {
      code_clr_bit(c, c->top);
      c->top++;
    }
    return true;
  }
  return false;
}
// make sure code isn't empty when popping off the bit
bool code_pop_bit(Code *c, uint8_t *bit) {
  if (!code_empty(c)) {
    // fprintf(stderr, "got to code pop\n");
    *bit = code_get_bit(c, c->top - 1);
    code_clr_bit(c, c->top - 1);
    c->top -= 1;
    return true;
  }
  return false;
}

// printing for debugging
void code_print(Code *c) {
  for (uint32_t i = 0; i < (code_size(c)); i++) {
    printf("%u\n", c->bits[i]);
  }
}
