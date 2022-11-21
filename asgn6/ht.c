#include "ht.h"
#include <stdio.h>
#include <stdlib.h>
#include "city.h"
#include "ll.h"

struct HashTable {
    //given to us
    uint64_t salt;
    uint32_t size;
    uint32_t n_keys;
    uint32_t n_hits;
    uint32_t n_misses;
    uint32_t n_examined;
    bool mtf;
    LinkedList **lists;
};

HashTable *ht_create(uint32_t size , bool mtf) {
    // given to us 
    HashTable *ht = (HashTable *) malloc(sizeof(HashTable));
    if (ht != NULL) {
        ht->mtf = mtf;
        ht->salt = 0x9846e4f157fe8840;
        ht->n_hits = ht->n_misses = ht->n_examined = 0;
        ht->n_keys = 0;
        ht->size = size;
        ht->lists = (LinkedList **) calloc(size , sizeof(LinkedList *));
        if (!ht->lists) {
            free(ht);
            ht = NULL;
        }
    }
    return ht;
}

void ht_delete(HashTable **ht){
    if(*ht){
        for(uint32_t i =0; i< (*ht) -> size; i++){
            if((*ht) -> lists[i]){
                ll_delete(&(*ht)->lists[i]);
            }
        }
        free((*ht)->lists);
        (*ht) -> lists = NULL;  
        free(*ht);
        *ht = NULL;
    }
    return;

}

uint32_t ht_size(HashTable *ht){
    return ht->size;
}

Node *ht_lookup(HashTable *ht, char *oldspeak){
    uint32_t link1 = 0;
    uint32_t link2 = 0;
    uint32_t link3 = 0;
    uint32_t link4 = 0;
    uint32_t index = hash(ht->salt,oldspeak)%ht_size(ht);
    ll_stats(&link1,&link2);
    if(ht->lists[index]== NULL){
        ht->n_misses += 1;
        return NULL;
    }
    Node *lookup = ll_lookup(ht->lists[index], oldspeak);
    if (lookup != NULL){
        ht->n_hits += 1;
    }
    else{
        ht->n_misses += 1;
    }
    ll_stats(&link3,&link4);
    ht -> n_examined += link4 - link2;
    return ll_lookup(ht->lists[index],oldspeak);    
}

void ht_insert(HashTable *ht, char *oldspeak, char *newspeak){
    uint32_t index = hash(ht->salt,oldspeak) % ht->size;
    //ht -> n_keys += 1;
    if(ll_lookup(ht->lists[index],oldspeak) == NULL){
        if(ht->lists[index] == NULL){
            ht->lists[index] = ll_create(ht -> mtf);
        }
        ht -> n_keys += 1;
        ll_insert(ht->lists[index],oldspeak,newspeak);
    }

}

uint32_t ht_count(HashTable *ht){
    uint32_t counting = 0;
    for (uint32_t i = 0; i < ht->size; i ++) {
        if (ht->lists[i]) {
            counting ++;
        }
    }
    return counting;
}

void ht_print(HashTable *ht){
    for (uint32_t i = 0; i < ht->size; i += 1) {
        if(ht->lists[i] != NULL){
            ll_print(ht->lists[i]);
        }
    }
    return;

}

void ht_stats(HashTable *ht, uint32_t *nk, uint32_t *nh, uint32_t *nm, uint32_t *ne){
    //setting the stats to the corresponding struct variables
    *nk = ht -> n_keys;
    *nh = ht -> n_hits;
    *nm = ht -> n_misses;
    *ne = ht -> n_examined;
}
