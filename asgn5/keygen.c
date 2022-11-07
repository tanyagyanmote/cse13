#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>
#define OPTIONS "b:i:n:d:s:vh"

// having a function for help message

void print_h() {
  fprintf(stderr, "Usage: ./keygen [options]\n");
  fprintf(stderr, "  ./keygen generates a public / private key pair, placing "
                  "the keys into the public and private\n");
  fprintf(stderr, "  key files as specified below. The keys have a modulus (n) "
                  "whose length is specified in\n");
  fprintf(stderr, "  the program options.\n");
  fprintf(stderr, "    -s <seed>   : Use <seed> as the random number seed. "
                  "Default: time()\n");
  fprintf(stderr, "    -b <bits>   : Public modulus n must have at least "
                  "<bits> bits. Default: 1024\n");
  fprintf(stderr, "    -i <iters>  : Run <iters> Miller-Rabin iterations for "
                  "primality testing. Default: 50\n");
  fprintf(stderr,
          "    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub\n");
  fprintf(
      stderr,
      "    -d <pvfile> : Private key file is <pvfile>. Default: rsa.priv\n");
  fprintf(stderr, "    -v          : Enable verbose output.\n");
  fprintf(stderr, "    -h          : Display program synopsis and usage.\n");
}

int main(int argc, char **argv) {
  // initalizing all variables
  int opt = 0;
  bool pb = false;
  bool pv = false;
  uint64_t bit_count = 1024;
  int iters = 50;
  FILE *pub_file;
  FILE *priv_file;
  bool verbose = false;
  uint64_t seed = time(NULL);
  // different cases for the different options
  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    switch (opt) {
    case 'b':
      // make sure the bit count is valid
      bit_count = strtoul(optarg, NULL, 10);
      if (bit_count > 4096 || bit_count < 50) {
        print_h();
        return -1;
      }
      break;
    case 'i':
      // make sure iters is valif
      iters = strtoul(optarg, NULL, 10);
      if (iters > 500 || iters < 1) {
        print_h();
        return -1;
      }
      break;
    case 'n':
      pub_file = fopen(optarg, "w");
      if (pub_file == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return -1;
      }
      pb = true;
      break;
    case 'd':
      priv_file = fopen(optarg, "w");
      if (priv_file == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return -1;
      }
      pv = true;
      break;
    case 's':
      seed = strtoul(optarg, NULL, 10);
      if (seed < 0) {
        print_h();
        return -1;
      }
      break;
    case 'h':
      print_h();
      return 0;
    case 'v':
      verbose = true;
      break;
    default:
      print_h();
      return -1;
    }
  }
  // open public and private keys
  if (!pv) {
    priv_file = fopen("rsa.priv", "w");
    if (priv_file == NULL) {
      fprintf(stderr, "Failed to open file.\n");
      return -1;
    }
  }

  if (!pb) {
    pub_file = fopen("rsa.pub", "w");
    if (pub_file == NULL) {
      fprintf(stderr, "Failed to open file.\n");
      return -1;
    }
  }
  // check if file permissions are set to 0600
  fchmod(fileno(priv_file), 0600);
  // initialize random state with set seed
  randstate_init(seed);
  mpz_t p, q, n, e, d, s, name;
  mpz_inits(p, q, n, e, d, s, name, NULL);

  char *user;
  // get user name
  user = getenv("USER");
  // make public and private keys
  rsa_make_pub(p, q, n, e, bit_count, iters);
  rsa_make_priv(d, e, p, q);
  // convert user to mpz_t with base 62
  mpz_set_str(name, user, 62);
  // compute the signature
  rsa_sign(s, name, d, n);
  // compute the public and private keys
  rsa_write_pub(n, e, s, user, pub_file);
  rsa_write_priv(n, d, priv_file);
  // print verbose if true
  if (verbose == true) {
    fprintf(stderr, "username = %s\n", user);
    gmp_fprintf(stderr, "user signature (%d bits) = %Zd\n",
                mpz_sizeinbase(s, 2), s);
    gmp_fprintf(stderr, "p (%d bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
    gmp_fprintf(stderr, "q (%d bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
    gmp_fprintf(stderr, "n - modulus (%d bits) = %Zd\n", mpz_sizeinbase(n, 2),
                n);
    gmp_fprintf(stderr, "e - public exponent (%d bits) = %Zd\n",
                mpz_sizeinbase(e, 2), e);
    gmp_fprintf(stderr, "d - private exponent (%d bits) = %Zd\n",
                mpz_sizeinbase(d, 2), d);
  }
  // clear random state
  // close all files
  // clear all variables
  randstate_clear();
  fclose(pub_file);
  fclose(priv_file);
  mpz_clears(p, q, n, e, d, s, name, NULL);
}
