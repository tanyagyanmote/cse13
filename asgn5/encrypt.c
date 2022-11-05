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
    fprintf(stderr,"Usage: ./encrypt [options]\n");
    fprintf(stderr,"  ./encrypt encrypts an input file using the specified public key file,\n");
    fprintf(stderr,"  writing the result to the specified output file.\n");
    fprintf(stderr,"    -i <infile> : Read input from <infile>. Default: standard input.\n");
    fprintf(stderr,"    -o <outfile>: Write output to <outfile>. Default: standard output.\n");
    fprintf(stderr,"    -n <keyfile>: Public key is in <keyfile>. Default: rsa.pub.\n");
    fprintf(stderr,"    -v          : Enable verbose output.\n");
    fprintf(stderr,"    -h          : Display program synopsis and usage.\n");
}


int main(int argc, char **argv) {
    int opt = 0;
    bool verbose = false;
    FILE *inputfile = stdin;
    FILE *outputfile = stdout;
    char *pub_s = "rsa.pub";
    FILE *pub_key = NULL;
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
                //strcpy(pub_s,optarg);
                pub_s = optarg;
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
    
    pub_key = fopen(pub_s, "r");
    if (pub_key == NULL) {
        printf("File empty - invalid");
        return -1;
    }

    char user[1024] = {0};
    mpz_t n, e, s, m, temp,temp1;
    mpz_inits(n, e, s, m, temp, temp1, NULL);
    rsa_read_pub(n, e, s, user, pub_key);

    if (verbose) {
        fprintf(stderr,"user = %s\n", user);
        gmp_fprintf(stderr,"s (%d bits) = %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_fprintf(stderr,"n (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_fprintf(stderr,"e (%d bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
    }
    
    mpz_set_str(m, user, 62);
    
    if (!rsa_verify(m, s, e, n)) {
        fprintf(stderr,"signature couldn't be verified");
        return 0;
    }

    rsa_encrypt_file(inputfile, outputfile, n, e);

    fclose(inputfile);
    fclose(outputfile);
    fclose(pub_key);

    mpz_clears(n, e, s, m, temp,NULL);

}
