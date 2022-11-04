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
    fprintf(stderr,"Usage: ./decrypt [options]\n");
    fprintf(stderr,"  ./decrypt decrypts an input file using the specified private key file,\n");
    fprintf(stderr,"  writing the result to the specified output file.\n");
    fprintf(stderr,"    -i <infile> : Read input from <infile>. Default: standard input.\n");
    fprintf(stderr,"    -o <outfile>: Write output to <outfile>. Default: standard output.\n");
    fprintf(stderr,"    -n <keyfile>: Private key is in <keyfile>. Default: rsa.priv.\n");
    fprintf(stderr,"    -v          : Enable verbose output.\n");
    fprintf(stderr,"    -h          : Display program synopsis and usage.\n");
}


int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false;
    FILE *inputfile = stdin;
    FILE *outputfile = stdout;
    FILE *privfile =NULL;
    char priv_s[] = "rsa.priv";
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
            case 'i':
                inputfile = fopen(optarg, "r");
                if(inputfile == NULL){
                    printf("File empty - invalid");
                    return -1;
                }
                break;
            case 'o': 
                outputfile = fopen(optarg, "w");
                if(outputfile == NULL){
                    fprintf(stderr,"File empty - invalid");
                    return -1;
                }
                break;
            case 'n':
                strcpy(priv_s,optarg);
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

        privfile = fopen(priv_s, "r");
        if (privfile == NULL) {
            fprintf(stderr,"File empty - invalid");
                return -1;
        }

        mpz_t n, d;
        mpz_inits(n, d, NULL);

        rsa_read_priv(n, d, privfile);

        if (verbose) {
            gmp_fprintf(stderr,"n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
            gmp_fprintf(stderr,"d (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
        }

        rsa_decrypt_file(inputfile, outputfile, n, d);

        fclose(inputfile);
        fclose(outputfile);
        fclose(privfile);

        mpz_clears(n,d, NULL);

    }

}

