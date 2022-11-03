#include "numtheory.h"
#include "randstate.h"
#include "rsa.h"
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>
#define OPTIONS "b:i:n:d:s:vh"

void print_h(){
    printf("Usage: ./keygen-dist [options]\n");
    printf("  ./keygen-dist generates a public / private key pair, placing the keys into the public and private\n");
    printf("  key files as specified below. The keys have a modulus (n) whose length is specified in\n");
    printf("  the program options.\n");
    printf("    -s <seed>   : Use <seed> as the random number seed. Default: time()\n");
    printf("    -b <bits>   : Public modulus n must have at least <bits> bits. Default: 1024\n");
    printf("    -i <iters>  : Run <iters> Miller-Rabin iterations for primality testing. Default: 50\n");
    printf("    -n <pbfile> : Public key file is <pbfile>. Default: rsa.pub\n");
    printf("    -d <pvfile> : Private key file is <pvfile>. Default: rsa.priv\n");
    printf("    -v          : Enable verbose output.\n");
    printf("    -h          : Display program synopsis and usage.\n");

}

int main(int argc, char **argv) {
    int opt = 0;
    bool pb = false;
    bool pv = false;
    uint64_t bit_count = 1024;
    int iters = 50;
    FILE *pub_file;
    FILE *priv_file;
    bool verbose = false;
    uint64_t seed = time(NULL);
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'b': 
            bit_count = strtoul(optarg, NULL, 10);
            break;
        case 'i': 
            iters = strtoul(optarg, NULL, 10);
            break;
        case 'n':
            pub_file = fopen(optarg, "w");
            if(pub_file == NULL){
                printf("File empty - invalid");
                return 0;
            }
            pb = true;
            break;
        case 'd':
            priv_file = fopen(optarg,"w");
            if(priv_file == NULL){
                printf("File empty - invalid");
                return 0;
            }
            pv = true;
            break;
        case 's':
            seed = strtoul(optarg, NULL, 10);
            break;
        case 'h':
            print_h();
            break;
        case 'v':
            verbose = true;
            break;
        default:
            print_h();
            return 0;
        }
    }

    if(!pv){
        priv_file = fopen("rsa.priv", "w");
        if(priv_file == NULL){
            printf("File empty - invalid");
            return 0;
        }
    }

    if(!pb){
        pub_file = fopen("rsa.pub", "w");
        if(pub_file == NULL){
            printf("File empty - invalid");
            return 0;
        }
    }

    fchmod(fileno(priv_file),0600);

    randstate_init(seed);
    mpz_t p, q, n, e, d, s, name;
    mpz_inits(p, q, n, e, d, s, name, NULL);

    char *user;
    user = getenv("USER");

    mpz_set_str(name, user, 62);

    rsa_make_pub(p, q, n, e, bit_count, iters);
    rsa_make_priv(d, e, p, q);

    rsa_write_pub(n, e, s, user, pub_file);
    rsa_write_priv(n, d, priv_file);

    rsa_sign(s, name, d, n);

    if (verbose == true) {
        printf("username = %s\n", user);
        gmp_printf("user signature (%d bits) = %Zd\n", mpz_sizeinbase(s, 2), s);
        gmp_printf("p (%d bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q (%d bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n - modulus (%d bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("e - public exponent (%d bits) = %Zd\n", mpz_sizeinbase(e, 2), e);
        gmp_printf("d - private exponent (%d bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }

    randstate_clear();
    fclose(pub_file);
    fclose(priv_file);
    mpz_clears(p, q, n, e, d, s, name,NULL);

}


