#include "io.h"
#include "code.h"
#include "header.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

uint64_t bytes_read = 0; // keep track for stats
uint64_t bytes_written = 0;
// TA tutor office hours puesdocode - Lev
int read_bytes(int infile, uint8_t *buf, int nbytes) {
  // reading nbytes from the infile, storing it to current
  ssize_t current = read(infile, buf, nbytes);
  ssize_t total = current;
  // read won't read nbytes all the time, so total < nbytes
  while (current != 0 && total < nbytes) {
    // making sure they all get read
    current = read(infile, buf + total, nbytes - total);
    total += current;
  }
  // compression stats
  bytes_read += total;
  return total;
}
// TA tutor office hours puesdocode - Lev
int write_bytes(int outfile, uint8_t *buf, int nbytes) {
  // writing nbytes from the infile, storing it to current
  ssize_t current = write(outfile, buf, nbytes);
  ssize_t total = current;
  // writing won't read nbytes all the time, so total < nbytes
  while (current != 0 && total < nbytes) {
    // making sure they all get written
    current = write(outfile, buf + total, nbytes - total);
    total += current;
  }
  // compression stats
  bytes_written += total;
  return total;
}
// TA tutor office hours puesdocode - Lev
static uint8_t buf_read_bit[BLOCK];
bool read_bit(int infile, uint8_t *bit) {
  static int offset = BLOCK * 8;
  static int size;
  // edge cases
  if (offset == 0) {
    // fprintf(stderr, "got to this point offset == 0\n");
    size = read_bytes(infile, buf_read_bit, BLOCK);
  }
  if (offset == BLOCK * 8) {
    // fprintf(stderr, "got to this point offset == BLOCK * 8\n");
    size = read_bytes(infile, buf_read_bit, BLOCK);
    // fprintf(stderr, "%d\n", size);
    offset = 0;
  }
  // if end of file
  if (size == 0) {
    return false;
  }
  // gets the bit
  *bit = (buf_read_bit[offset / 8] >> (offset % 8)) & 1;
  offset += 1;
  return true;
}
// TA tutor office hours puesdocode - Lev
static uint8_t write_b[BLOCK];
static uint64_t index = 0;
void write_code(int outfile, Code *c) {
  // going through through c
  for (uint32_t i = 0; i < code_size(c); i++) {
    // checking if bit is 1 or 0
    // if its 1, add 1 to buffer if its 0 then add 0 to the buffer
    if (code_get_bit(c, i)) {
      write_b[index / 8] = write_b[index / 8] | (1 << index % 8);
    } else {
      write_b[index / 8] = write_b[index / 8] & ~((1 << (index % 8)));
    }
    index += 1;
    // fprintf(stderr, "index: %lu\n", index);
    // once you reach the end of buffer, writing the bytes to outfile
    // also reseting the index
    if (index == BLOCK * 8) {
      write_bytes(outfile, write_b, BLOCK);
      index = 0;
    }
  }
  // fprintf(stderr, "size: %u\n", code_size(c));
}
// TA office hours puesdocode - Lev
void flush_codes(int outfile) {
  //checking to see if there are any extra bits to zero out
  int top = index / 8;
  if (index % 8 != 0) {
    //keeping track
    top += 1;
  }
  //flush the bytes to the outfile
  write_bytes(outfile, write_b, top);
}
