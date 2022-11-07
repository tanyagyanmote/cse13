#include "numtheory.h"
#include "randstate.h"
#include <gmp.h>
#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void pow_mod(mpz_t o, mpz_t a, mpz_t d, mpz_t n) {
  // using the puesdocode given
  mpz_t temp_v, temp_p, temp_d;
  mpz_inits(temp_v, temp_p, temp_d, NULL);
  mpz_set_ui(temp_v, 1);
  mpz_set(temp_p, a);
  mpz_set(temp_d, d);
  // check if d > 0
  while (mpz_cmp_ui(temp_d, 0) > 0) {
    // check if d is odd
    if (mpz_odd_p(temp_d)) {
      // multiply v times p mod n
      mpz_mul(temp_v, temp_v, temp_p);
      mpz_mod(temp_v, temp_v, n);
    }
    // p times p mod n
    mpz_mul(temp_p, temp_p, temp_p);
    mpz_mod(temp_p, temp_p, n);
    // d divided by 2
    mpz_fdiv_q_ui(temp_d, temp_d, 2);
  }
  mpz_set(o, temp_v);
  mpz_clears(temp_v, temp_p, temp_d, NULL);
}

void gcd(mpz_t d, mpz_t a, mpz_t b) {
  // using given puesdocode
  mpz_t temp_a, temp_b;
  mpz_inits(temp_a, temp_b, NULL);
  mpz_set(temp_a, a);
  mpz_set(temp_b, b);
  // while b isn't 0
  while (mpz_cmp_ui(temp_b, 0) != 0) {
    mpz_t t;
    // set t to b
    mpz_init_set(t, temp_b);
    // b is a mod b
    mpz_mod(temp_b, temp_a, temp_b);
    // set a to t
    mpz_set(temp_a, t);
    mpz_clear(t);
  }
  mpz_set(d, temp_a);
  mpz_clears(temp_a, temp_b, NULL);
}

void mod_inverse(mpz_t o, mpz_t a, mpz_t n) {
  // using given puesdocode
  mpz_t r_prime, r, t_prime, t, temp_r, temp_r_prime, temp_t, temp_t_prime, q,
      temp_qr_prime, temp_qt_prime;
  mpz_inits(r, r_prime, t, t_prime, temp_r, temp_r_prime, temp_t, temp_t_prime,
            q, temp_qr_prime, temp_qt_prime, NULL);
  mpz_set(r_prime, a);                  // setting r' to a
  mpz_set(r, n);                        // setting r to n
  mpz_set_ui(t_prime, 1);               // setting t' to 1
  mpz_set_ui(t, 0);                     // setting t to 0
  while (mpz_cmp_ui(r_prime, 0) != 0) { // checking while r' isn't 0
    mpz_fdiv_q(q, r, r_prime);          // q is r divided by r'
    mpz_set(temp_r_prime, r_prime);
    mpz_set(temp_r, r);
    mpz_mul(temp_qr_prime, q, temp_r_prime); // multiply q times r'
    mpz_sub(r_prime, temp_r, temp_qr_prime); // subtract r from that ^
    mpz_set(r, temp_r_prime);
    mpz_set(temp_t_prime, t_prime);
    mpz_set(temp_t, t);
    mpz_mul(temp_qt_prime, q, temp_t_prime); // multiply q times t prime
    mpz_sub(t_prime, temp_t, temp_qt_prime); // subtract t from that ^
    mpz_set(t, temp_t_prime);
  }
  // if r is greater than 1, has no inverse so set out to 0
  if (mpz_cmp_ui(r, 1) > 0) {
    mpz_set_ui(o, 0);
  }
  // if t is less than 0, then add n to t
  if (mpz_cmp_ui(t, 0) < 0) {
    mpz_add(t, t, n);
  }
  mpz_set(o, t);
  mpz_clears(r_prime, r, t_prime, t, temp_r, temp_r_prime, temp_t, temp_t_prime,
             q, temp_qr_prime, temp_qt_prime, NULL);
}

// find totient fucntion for isprime
// write n-1= 2^srsuch that r is odd
void find_totient(mpz_t s, mpz_t r, mpz_t n) {
  mpz_t temp_n, temp_mod;
  mpz_inits(temp_n, temp_mod, NULL);
  mpz_set(temp_n, n);
  mpz_mod_ui(temp_mod, temp_n, 2);
  while (mpz_cmp_ui(temp_mod, 0) == 0) {
    mpz_add_ui(s, s, 1);
    mpz_fdiv_q_ui(temp_n, temp_n, 2);
    mpz_mod_ui(temp_mod, temp_n, 2);
  }
  mpz_set(r, temp_n);
  mpz_clears(temp_n, temp_mod, NULL);
}

bool is_prime(mpz_t n, uint64_t iters) {
  mpz_t a, s, r, temp_n, two;
  mpz_inits(a, s, r, temp_n, two, NULL);
  mpz_set_ui(two, 2);
  mpz_sub_ui(temp_n, n, 1);
  // if n is less than 4 it's prime, return true
  if (mpz_cmp_ui(n, 4) < 0) {
    return true;
  }
  // if n is even it's not prime, return false
  if (mpz_even_p(n)) {
    return false;
  }
  find_totient(s, r, temp_n);
  // start at 1 that goes to iters
  for (uint64_t i = 1; i < iters; i++) {
    mpz_t range, y, j;
    mpz_inits(range, y, j, NULL);
    mpz_sub_ui(range, n, 3);
    // choosing random number
    mpz_urandomm(a, state, range);
    mpz_add_ui(a, a, 2);
    // power moding r^n
    pow_mod(y, a, r, n);
    // if y isn't 1, and y isn't n-1
    if (mpz_cmp_ui(y, 1) != 0 && mpz_cmp(y, temp_n) != 0) {
      // j = 1
      mpz_set_ui(j, 1);
      // if j is less than s-1 and y isn't n-1
      while (mpz_cmp(j, s) < 0 && mpz_cmp(y, temp_n) != 0) {
        // power mod 2^n
        pow_mod(y, y, two, n);
        // if y is 1 return false
        if (mpz_cmp_ui(y, 1) == 0) {
          return false;
        }
        // j = j+1
        mpz_add_ui(j, j, 1);
      }
      // if y isn't n-1 return false
      if (mpz_cmp(y, temp_n) != 0) {
        return false;
      }
    }
    mpz_clears(range, y, j, NULL);
  }
  mpz_clears(a, s, r, temp_n, two, NULL);
  return true;
}

void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
  mpz_t temp, min, rand;
  mpz_inits(temp, rand, min, NULL);
  mpz_ui_pow_ui(min, 2, bits);
  do {
    mpz_set(temp, min);
    mpz_urandomb(rand, state, bits);
    mpz_add(temp, temp, rand);
  }
  // while it isn't prime, generate a prime num
  while (!is_prime(temp, iters));
  mpz_set(p, temp);
  mpz_clears(temp, min, rand, NULL);
  return;
}
