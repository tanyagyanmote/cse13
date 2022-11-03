 #include <stdio.h>
#include <stdlib.h>
#include "rsa.h"
#include "numtheory.h"
#include "randstate.h"

void rsa_make_pub(mpz_t p, mpz_t q, mpz_t n, mpz_t e, uint64_t nbits, uint64_t iters){
    mpz_t p1,q1,mul,abs_val,greatest,least,temp1;
    bool check = true;
    mpz_inits(p1,q1,mul,abs_val,greatest,least,temp1,NULL);
    uint64_t p_bits = (random() % ((3 * nbits) / 4)) + (nbits / 4);
    uint64_t q_bits = nbits - p_bits;
    make_prime(p, p_bits, iters); 
    make_prime(q, q_bits, iters); 
    mpz_mul(n, q, p);
    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(mul,p1,q1);
    mpz_abs(abs_val, mul);
    gcd(greatest,p1,q1);
    mpz_fdiv_q(least,abs_val,greatest);
    mpz_urandomb(e, state, nbits);
    while(check){
        mpz_urandomb(e,state,nbits);
        gcd(temp1,e,least);
        if(mpz_cmp_ui(temp1, 1) != 0){
            check = false;
        }
    }
    mpz_clears(p1,q1,mul,abs_val,greatest,least);
    
}


void rsa_write_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile){
    gmp_fprintf(pbfile, "%Zx\n", n);
    gmp_fprintf(pbfile, "%Zx\n", e);
    gmp_fprintf(pbfile, "%Zx\n", s);
    gmp_fprintf(pbfile, "%s\n", username);
}



void rsa_read_pub(mpz_t n, mpz_t e, mpz_t s, char username[], FILE *pbfile){
    gmp_fscanf(pbfile, "%Zx\n", n);
    gmp_fscanf(pbfile, "%Zx\n", e);
    gmp_fscanf(pbfile, "%Zx\n", s);
    gmp_fscanf(pbfile, "%s\n", username);
}


void rsa_make_priv(mpz_t d, mpz_t e, mpz_t p, mpz_t q){
    mpz_t totient, temp_q, temp_p, gcd1, temp1;
    mpz_inits(totient, temp_q, temp_p, gcd1, temp1, NULL);
    mpz_sub_ui(temp_p, p, 1);
    mpz_sub_ui(temp_q, q, 1);
    mpz_mul(totient, temp_p, temp_q);
    gcd(gcd1,temp_p,temp_q);
    mpz_fdiv_q(temp1,totient,gcd1);
    mod_inverse(d, e, temp1);
    mpz_clears(totient, temp_q, temp_p, gcd1, temp1,NULL);
}


void rsa_write_priv(mpz_t n, mpz_t d, FILE *pvfile){
    gmp_fprintf(pvfile, "%Zx\n", n);
    gmp_fprintf(pvfile, "%Zx\n", d);
}


void rsa_read_priv(mpz_t n, mpz_t d, FILE *pvfile){
    gmp_fscanf(pvfile, "%Zx\n", n); 
    gmp_fscanf(pvfile, "%Zx\n", d);
}


void rsa_encrypt(mpz_t c, mpz_t m, mpz_t e, mpz_t n){
    pow_mod(c, m, e, n);
}


void rsa_encrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t e){
    uint64_t k = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *block = (uint8_t *) calloc(k, sizeof(uint8_t));
    block[0] = 0xFF;
    size_t j;
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    while ((j = fread(&block[1], sizeof(uint8_t), k - 1, infile)) > 0) {
        mpz_import(m, j + 1, 1, sizeof(uint8_t), 1, 0, &block[0]);
        rsa_encrypt(c, m, e, n);
        gmp_fprintf(outfile, "%Zx\n", c);
    }
    mpz_clears(m, c, NULL);
    free(block);
    block = NULL;

}

void rsa_decrypt(mpz_t m, mpz_t c, mpz_t d, mpz_t n){
    pow_mod(m, c, d, n);
}


void rsa_decrypt_file(FILE *infile, FILE *outfile, mpz_t n, mpz_t d){
    uint64_t size = (mpz_sizeinbase(n, 2) - 1) / 8;
    uint8_t *block = (uint8_t *) calloc(size, sizeof(uint8_t));
    size_t j;
    mpz_t m, c;
    mpz_inits(m, c, NULL);
    while((gmp_fscanf(infile, "%Zx\n", c)) != EOF){
        rsa_decrypt(m, c, d, n);
        mpz_export(block, &j, 1, sizeof(uint8_t), 1, 0, m);
        fwrite(&block[1], sizeof(uint8_t), j - 1, outfile);
    }
    mpz_clears(c, m, NULL);
    free(block);

}


void rsa_sign(mpz_t s, mpz_t m, mpz_t d, mpz_t n){
    pow_mod(s, m, d, n);
}


bool rsa_verify(mpz_t m, mpz_t s, mpz_t e, mpz_t n){
    mpz_t t;
    mpz_init(t);
    pow_mod(t, s, e, n);
    if (mpz_cmp(t, m) == 0) {
        mpz_clear(t);
        return true;
    }
    mpz_clear(t);
    return false;
}
