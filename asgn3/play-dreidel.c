#include <stdio.h>
#include "mtrand.h"
#include "dreidel.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>


#define OPTIONS "p:s:c:v"

char *list_players[] = { "Aharon", "Batsheva", "Chanah", "David","Ephraim", "Faige", "Gamaliel", "Hannah"};
bool test_v = false;

int main(int argc, char **argv) {
    int default_players = 4;
    int default_coins = 3;
    uint64_t default_seed = 613;
    int opt = 0;


    while ((opt = getopt(argc, argv, OPTIONS)) != -1){
        switch(opt){
        case 'p':
            default_players = atoi(optarg); 
            if (default_players >= 2 && default_players <= 8){
                break;
            }
            return 1;
        case 's':
            default_seed = strtoul(optarg,NULL,10);
            if(default_seed <= 9999999999){
                break;
            }
            return 1;      
        case 'c':
            default_coins = atoi(optarg);
            if (default_coins >= 1 && default_coins <= 20){
                break;
            }
            return 1;
        case 'v':
            test_v = true;
            break;
        default: return 1;
        }
    }

    int n_rounds = 0;
    mtrand_seed(default_seed);
    int index = play_game(default_players,default_coins,&n_rounds);
    printf("%s %d %d %d %lu\n", list_players[index], default_players, default_coins, n_rounds, default_seed);
}

