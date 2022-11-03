#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <inttypes.h>
#include <string.h>
#include "numtheory.h"
#include <time.h>
#include <gmp.h>

#include "randstate.h"
#include "rsa.h"

#define OPTIONS "i:o:n:vh"

void print_h(){
    printf("Usage: ./decrypt-dist [options]\n");
    printf("  ./decrypt-dist decrypts an input file using the specified private key file,\n");
    printf("  writing the result to the specified output file.\n");
    printf("    -i <infile> : Read input from <infile>. Default: standard input.\n");
    printf("    -o <outfile>: Write output to <outfile>. Default: standard output.\n");
    printf("    -n <keyfile>: Private key is in <keyfile>. Default: rsa.priv.\n");
    printf("    -v          : Enable verbose output.\n");
    printf("    -h          : Display program synopsis and usage.\n");
}


int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false;
    FILE *inputfile = stdin;
    FILE *outputfile = stdout;
    FILE *privfile;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
            case 'i':
                inputfile = fopen(optarg, "r");
                if(inputfile == NULL){
                    printf("File empty - invalid");
                    return 0;
                }
                break;
            case 'o': 
                outputfile = fopen(optarg, "w");
                if(outputfile == NULL){
                    printf("File empty - invalid");
                    return 0;
                }
                break;
            case 'n':
                privfile = fopen(optarg, "r");
                if (privfile == NULL) {
                    printf("File empty - invalid");
                }
                break;
            case 'v':
                verbose = true;
                break; 
            case 'h':
                print_h();
                break;
            default:
                print_h();
                return 0; 
        }

        mpz_t n, d;
        mpz_inits(n, d, NULL);

        if (verbose) {
            gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
            gmp_printf("d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
        }

        rsa_read_priv(n, d, privfile);

        rsa_decrypt_file(inputfile, outputfile, n, d);

        fclose(inputfile);
        fclose(outputfile);
        fclose(privfile);

        mpz_clears(n,d, NULL);

    }

}

