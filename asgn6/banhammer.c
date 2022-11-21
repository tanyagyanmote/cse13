#include <stdio.h>
#include <getopt.h>
#include <stdint.h>
#include "bv.h"
#include "bf.h"
#include "ht.h"
#include "ll.h"
#include "node.h"
#include "parser.h"
#include "city.h"
#include "messages.h"

#define OPTIONS "ht:f:sm"

void print_h() {
    fprintf(stderr, "Usage: ./banhammer [options]\n");
    fprintf(stderr, "  ./banhammer will read in words from stdin, identify any badspeak or old speak and output an\n");
    fprintf(stderr, "  appropriate punishment message. The badspeak and oldspeak (with the newspeak translation) \n");
    fprintf(stderr, "  that caused the punishment will be printed after the message. If statistics are enabled\n");
    fprintf(stderr, "  punishment messages are suppressed and only statistics will be printed.\n");
    fprintf(stderr, "    -t <ht_size>: Hash table size set to <ht_size>. (default: 10000)\n");
    fprintf(stderr, "    -f <bf_size>: Bloom filter size set to <bf_size>. (default 2^19)\n");
    fprintf(stderr, "    -s          : Enables the printing of statistics.\n");
    fprintf(stderr, "    -m          : Enables move-to-front rule.\n");
    fprintf(stderr, "    -h          : Display program synopsis and usage.\n");
}


int main(int argc, char **argv) {
    int opt = 0;
    bool statistics = false;
    uint32_t table_size = 10000;
    uint32_t f_size = 1<<19;
    bool mtf = false;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
            case 's':
                statistics = true;
                break;
            case 't':
                table_size = strtoul(optarg,NULL,10);
                if (table_size < 1) {
                    fprintf(stderr, "Invalid hash table size.\n");
                    return -1;
                }
                break; //check if u need to print help
            case 'f':
                f_size = strtoul(optarg,NULL,10);
                if (f_size < 1) {
                    fprintf(stderr, "Invalid Bloom filter size.\n");
                    return -1;
                }
                break; //check if u need to print help
            case 'm':
                mtf = true;
                break;
            case 'h':
                print_h();
                return 0;
            default:
                print_h();
                return -1;
            }
    }
    char newspeak[1024]; //check on size
    char badspeak[1024];
    char oldspeak[1024];
    char possible[1024];

    BloomFilter *bf = bf_create(f_size);
    HashTable *ht = ht_create(table_size,mtf);

    if (bf == NULL) {
        fprintf(stderr, "Failed to create Bloom filter.\n");
        return -1;
    }
    if (ht == NULL) {
        fprintf(stderr, "Failed to create hash table.\n");
        return -1;
    }

    FILE *badspeak_file = fopen("badspeak.txt", "r"); 
    Parser *p_bad = parser_create(badspeak_file);

    if(p_bad == NULL){
        fprintf(stderr, "Failed to parse stdin.\n");
        return -1;
    }

    while((next_word(p_bad,badspeak)) != false){
        //printf("%s\n", badspeak);
        bf_insert(bf,badspeak);
        ht_insert(ht,badspeak,NULL);
    }

    fclose(badspeak_file);

    FILE *newspeak_file = fopen("newspeak.txt", "r");
    Parser *p_new = parser_create(newspeak_file);

    if(p_new == NULL){
        fprintf(stderr, "Failed to parse stdin.\n");
        return -1;
    }

    while((next_word(p_new,oldspeak)) != false && next_word(p_new,newspeak) != false){
        bf_insert(bf,oldspeak);
        ht_insert(ht,oldspeak,newspeak);
    }

    fclose(newspeak_file);

    Parser *p = parser_create(stdin);
    if(p==NULL){
        fprintf(stderr, "Failed to parse stdin.\n");
        return -1;
    }

    Node *word_p;
    LinkedList *r_speak = ll_create(mtf);

    if (!r_speak){
        fprintf(stderr, "Can't create right Linkedlist");
        return -1;
    }

    LinkedList *crime = ll_create(mtf);

    if (!crime){
        fprintf(stderr,"Can't create thought Linkedlist");
        return -1;
    }

    while (next_word(p, possible) != false){
        if (bf_probe(bf, possible) == true){   
           word_p = ht_lookup(ht, possible);
            if (word_p == NULL){
                continue;
            }
            else{
                if (word_p -> newspeak == NULL){
                    ll_insert(crime, word_p -> oldspeak, NULL);
                }
                else{
                    ll_insert(r_speak, word_p -> oldspeak, word_p -> newspeak);
                }
            }
        }
    }

    if(ll_length(crime) > 0 && ll_length(r_speak) > 0){
        if(!statistics){
            printf("%s", mixspeak_message);
            ll_print(crime);
            ll_print(r_speak);
        }
    }
    if(ll_length(crime) > 0 && ll_length(r_speak) == 0){
        if (!statistics){
            printf("%s", badspeak_message);
            ll_print(crime);
        }
    }
    if(ll_length(crime) == 0 && ll_length(r_speak) > 0){
        if(!statistics){
            printf("%s", goodspeak_message);
            ll_print(r_speak);
        }
    }

    //stats
    if(statistics == true){

        uint32_t ht_keys = 0, ht_hits = 0, ht_misses = 0, ht_examined = 0;
        uint32_t bf_keys = 0, bf_hits = 0, bf_misses = 0, bf_examined = 0;
        ht_stats(ht,&ht_keys,&ht_hits,&ht_misses,&ht_examined);
        bf_stats(bf,&bf_keys,&bf_hits,&bf_misses,&bf_examined);

        printf("ht keys: %u\n", ht_keys); // get the values
        printf("ht hits: %u\n", ht_hits);
        printf("ht misses: %u\n", ht_misses);
        printf("ht probes: %u\n", ht_examined);
        printf("bf keys: %u\n", bf_keys);
        printf("bf hits: %u\n", bf_hits);
        printf("bf misses: %u\n", bf_misses);
        printf("bf bits examined: %u\n", bf_examined);

        float bf_examined_miss = 0, false_p = 0, seek_len = 0, bf_load = 0;

        if(bf_misses == 0){
            bf_examined_miss = 0;
            printf("Bits examined per miss: %0.6f\n", bf_examined_miss);
        }
        // n_hashes = 5
        else{
            bf_examined_miss = (float)((bf_examined) - (5 * bf_hits))/bf_misses;
            printf("Bits examined per miss: %0.6f\n", bf_examined_miss);

        }
        if(bf_hits == 0){
            false_p = 0;
            printf("False positives: %0.6f\n", false_p);
        }
        else{
            false_p = (float)ht_misses / bf_hits;
            printf("False positives: %0.6f\n", false_p);
        }

        if((ht_hits + ht_misses) == 0){
            seek_len = 0;
            printf("Average seek length: %0.6f\n",seek_len);
        }
        else{
            seek_len = (float)ht_examined / (ht_hits + ht_misses);
            printf("Average seek length: %0.6f\n",seek_len);
        }

        if(bf_size(bf) == 0){
            bf_load = 0;
            printf("Bloom filter load: %0.6f\n", bf_load);
        }
        else{
            bf_load = (float)bf_count(bf) / bf_size(bf);
            printf("Bloom filter load: %0.6f\n", bf_load);
        }

    }
    parser_delete(&p);
    parser_delete(&p_bad);
    parser_delete(&p_new);
    bf_delete(&bf);
    ht_delete(&ht);
    ll_delete(&r_speak);
    ll_delete(&crime);
    return 0; //lol


}










