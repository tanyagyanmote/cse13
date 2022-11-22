#define N_HASHES 5
#include "bf.h"
#include "bv.h"
#include "city.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

struct BloomFilter {
  // given
  uint64_t salts[N_HASHES];
  uint32_t n_keys;
  uint32_t n_hits;
  uint32_t n_misses;
  uint32_t n_bits_examined;
  BitVector *filter;
};

static uint64_t default_salts[] = {
    // given values
    0x5adf08ae86d36f21, 0x419d292ea2ffd49e, 0x50d8bb08de3818df,
    0x272347aea4045dd5, 0x7c8e16f768811a21};

BloomFilter *bf_create(uint32_t size) {
  // given to us
  BloomFilter *bf = (BloomFilter *)malloc(sizeof(BloomFilter));
  if (bf) {
    bf->n_keys = bf->n_hits = 0;
    bf->n_misses = bf->n_bits_examined = 0;
    for (int i = 0; i < N_HASHES; i++) {
      bf->salts[i] = default_salts[i];
    }
    bf->filter = bv_create(size);
    if (bf->filter == NULL) {
      free(bf);
      bf = NULL;
    }
  }
  return bf;
}

void bf_delete(BloomFilter **bf) {
  // deleting the bloom filter
  // null check
  if (*bf != NULL) {
    // null check for the filter
    if ((*bf)->filter != NULL) {
      // using bit vector delete
      bv_delete(&(*bf)->filter);
    }
    // free the bloom filter
    free(*bf);
    // set to NULL
    *bf = NULL;
  }
}

uint32_t bf_size(BloomFilter *bf) { return bv_length(bf->filter); }

void bf_insert(BloomFilter *bf, char *oldspeak) {
  // inserting the bf using bv set bit
  bv_set_bit(bf->filter, hash(bf->salts[0], oldspeak) % bf_size(bf));
  bv_set_bit(bf->filter, hash(bf->salts[1], oldspeak) % bf_size(bf));
  bv_set_bit(bf->filter, hash(bf->salts[2], oldspeak) % bf_size(bf));
  bv_set_bit(bf->filter, hash(bf->salts[3], oldspeak) % bf_size(bf));
  bv_set_bit(bf->filter, hash(bf->salts[4], oldspeak) % bf_size(bf));
  bf->n_keys += 1;
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {
  // Check if oldspeak was hashed into the bloomfilter
  // with all 5 salts and if they all equal 1 return true
  // if not then return false
  uint32_t one = hash(bf->salts[0], oldspeak) % bf_size(bf);
  uint32_t two = hash(bf->salts[1], oldspeak) % bf_size(bf);
  uint32_t three = hash(bf->salts[2], oldspeak) % bf_size(bf);
  uint32_t four = hash(bf->salts[3], oldspeak) % bf_size(bf);
  uint32_t five = hash(bf->salts[4], oldspeak) % bf_size(bf);
  bf->n_bits_examined += 1;
  // if its false add one to misses and return false
  if (bv_get_bit(bf->filter, one) == 0) {
    bf->n_misses += 1;
    return false;
  }
  bf->n_bits_examined += 1;
  if (bv_get_bit(bf->filter, two) == 0) {
    bf->n_misses += 1;
    return false;
  }
  bf->n_bits_examined += 1;
  if (bv_get_bit(bf->filter, three) == 0) {
    bf->n_misses += 1;
    return false;
  }
  bf->n_bits_examined += 1;
  if (bv_get_bit(bf->filter, four) == 0) {
    bf->n_misses += 1;
    return false;
  }
  bf->n_bits_examined += 1;
  if (bv_get_bit(bf->filter, five) == 0) {
    bf->n_misses += 1;
    return false;
  }
  // if all true then add one to hits
  bf->n_hits += 1;
  return true;
}

uint32_t bf_count(BloomFilter *bf) {
  uint32_t count = 0;
  for (uint32_t i = 0; i < bf_size(bf); i += 1) {
    // if its true add one to count
    if (bv_get_bit(bf->filter, i)) {
      count += 1;
    }
  }
  return count;
}

void bf_print(BloomFilter *bf) {
  if (bf != NULL) {
    bv_print(bf->filter);
  }
}

void bf_stats(BloomFilter *bf, uint32_t *nk, uint32_t *nh, uint32_t *nm,
              uint32_t *ne) {
  *nk = bf->n_keys;
  *nh = bf->n_hits;
  *nm = bf->n_misses;
  *ne = bf->n_bits_examined;
}
