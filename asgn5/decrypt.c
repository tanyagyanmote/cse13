#include "numtheory.h"
#include <gmp.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#include "randstate.h"
#include "rsa.h"

#define OPTIONS "i:o:n:vh"

// having a function for help message
void print_h() {
  fprintf(stderr, "Usage: ./decrypt [options]\n");
  fprintf(stderr, "  ./decrypt decrypts an input file using the specified "
                  "private key file,\n");
  fprintf(stderr, "  writing the result to the specified output file.\n");
  fprintf(
      stderr,
      "    -i <infile> : Read input from <infile>. Default: standard input.\n");
  fprintf(stderr, "    -o <outfile>: Write output to <outfile>. Default: "
                  "standard output.\n");
  fprintf(
      stderr,
      "    -n <keyfile>: Private key is in <keyfile>. Default: rsa.priv.\n");
  fprintf(stderr, "    -v          : Enable verbose output.\n");
  fprintf(stderr, "    -h          : Display program synopsis and usage.\n");
}

int main(int argc, char **argv) {
  // initalizing all variables
  int opt = 0;
  bool verbose = false;
  FILE *inputfile = stdin;
  FILE *outputfile = stdout;
  FILE *privfile = NULL;
  char *priv_s = "rsa.priv";
  while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
    // different cases for the different options
    switch (opt) {
    case 'i':
      inputfile = fopen(optarg, "r");
      if (inputfile == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return -1;
      }
      break;
    case 'o':
      outputfile = fopen(optarg, "w");
      if (outputfile == NULL) {
        fprintf(stderr, "Failed to open file.\n");
        return -1;
      }
      break;
    case 'n':
      // strcpy(priv_s,optarg);
      priv_s = optarg;
      break;
    case 'v':
      verbose = true;
      break;
    case 'h':
      print_h();
      return 0;
    default:
      print_h();
      return -1;
    }
  }
  // open private key file and read from it
  privfile = fopen(priv_s, "r");
  if (privfile == NULL) {
    fprintf(stderr, "Failed to open file.\n");
    return -1;
  }
  // create mpz varibles to use
  mpz_t n, d;
  mpz_inits(n, d, NULL);

  rsa_read_priv(n, d, privfile);

  // print verbose output
  if (verbose) {
    gmp_fprintf(stderr, "n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
    gmp_fprintf(stderr, "d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
  }

  // decrypt the file
  rsa_decrypt_file(inputfile, outputfile, n, d);

  // close all files
  fclose(inputfile);
  fclose(outputfile);
  fclose(privfile);

  // clear mpz_t variables
  mpz_clears(n, d, NULL);
}
