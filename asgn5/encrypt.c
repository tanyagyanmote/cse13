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
    printf("Usage: ./encrypt-dist [options]\n");
    printf("  ./encrypt-dist encrypts an input file using the specified public key file,\n");
    printf("  writing the result to the specified output file.\n");
    printf("    -i <infile> : Read input from <infile>. Default: standard input.\n");
    printf("    -o <outfile>: Write output to <outfile>. Default: standard output.\n");
    printf("    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\n");
    printf("    -v          : Enable verbose output.\n");
    printf("    -h          : Display program synopsis and usage.\n");
}


int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false;
    FILE *inputfile = stdin;
    FILE *outputfile = stdout;
    FILE *pub_key;
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
                pub_key = fopen(optarg, "r");
                if (pub_key == NULL) {
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
                return -1;

        }

        pub_key = fopen(optarg, "r");
        if (pub_key == NULL) {
            printf("File empty - invalid");
            return -1;
        }

        char user[1024] = {0};
        mpz_t n, e, s, m;
        mpz_inits(n, e, s, m, NULL);
        rsa_read_pub(n, e, s, user, pub_key);

        if (verbose) {
            printf("user = %s\n", user);
            gmp_printf("s (%d bits) = %Zd\n", mpz_sizeinbase(s, 2), s);
            gmp_printf("n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
            gmp_printf("e (%d bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
        }
        
        mpz_set_str(m, user, 62);


        if (!rsa_verify(m, s, e, n)) {
            printf("Signature couldn't be verified");
            return 0;
        }

        rsa_encrypt_file(inputfile, outputfile, n, e);

        fclose(inputfile);
        fclose(outputfile);
        fclose(pub_key);

        mpz_clears(n, e, s, m, NULL);


    }

}
