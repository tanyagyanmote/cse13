#include "bv.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

struct BitVector {
    uint32_t length;
    uint64_t *vector;
};


BitVector *bv_create(uint32_t length){
    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));
    if(bv != NULL){
        bv->length = length;
        if (length % 64 != 0) {
            length = (length/64)+1;
        }
        else{
            length = length /64;
        }
        bv->vector = (uint64_t *) calloc(length, sizeof(uint64_t));
        if(bv->vector == NULL){
            free(bv);
            return NULL;
        }
    }
    return bv;
}


void bv_delete(BitVector **bv){
    // free vector and bv
    if(*bv != NULL){
        if (*bv != NULL){
            free((*bv)->vector);
        }
        free(*bv);
        *bv = NULL;
    }
    return;
}

uint32_t bv_length(BitVector *bv){
    return bv->length;
}

void bv_set_bit(BitVector *bv, uint32_t i){
    //bitwise operator left shift by 1 + set bit to 1
    bv -> vector[i/64] |= ((uint64_t)1 << (i%64));
}

void bv_clr_bit(BitVector *bv, uint32_t i){
    //bitwise operator to left shift by 1 + set bit to 0
    bv -> vector[i / 64] = bv -> vector[i / 64] & ~(1 << i % 64);
}

uint8_t bv_get_bit(BitVector *bv, uint32_t i){
    //checking val of bit
    return ((bv -> vector[i / 64] >> i % 64) & (uint64_t)1);
}

void bv_print(BitVector *bv){
    for (uint32_t i = 0; i < bv->length; i++) {
        printf("%d\n", bv_get_bit(bv, i));
    }
}

