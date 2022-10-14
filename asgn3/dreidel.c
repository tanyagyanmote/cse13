#include <stdio.h>
#include "mtrand.h"
#include <stdbool.h>

char spin_dreidel(void){
    
    char g = 'G';
    char h = 'H';
    char n = 'N';
    char s = 'S';

    uint64_t rand_num = mtrand_rand64()%4;

    if (rand_num==0){
        return g;
    }
    if (rand_num==1){
        return h;
    }
    if (rand_num==2){
        return n;
    }
    if (rand_num==3){
        return s;
    }
    else{
        return 0;
    }

 }

int play_game (int n_players , int coins_per_player, int * n_rounds ){

    int players [n_players];
    int remaining = n_players;
    int pot = 0;
    int num = 0;

    for (int i = 0; i < n_players; i++){
        players[i] = coins_per_player;
    }

    while(true){
        char turn = spin_dreidel();
        for(int j = 0; j < n_players; j++){
            if (turn=='G'){
                players[j] += pot;
                pot = 0;
            }

            if (turn=='H' && pot != 0){
                players[j] += pot/2;
                pot -= pot/2;
            }
 
            if (turn=='S'){
                if(players[j] == 0){
                    remaining -=1;
                }
                else if(players[j] > 0){
                    players[j] -= 1;
                    pot += 1;
                }
            }

            printf("player:     %i\n", j);
            printf("coins:      %i\n", players[j]);
            printf("round:      %i\n", num);
            //printf("remaining:  %i\n", remaining);
            //printf("turn:       %c\n", turn);

            if (remaining == 1){
                int max = 0;
                int p = 0;
                for (int l = 0; l < n_players; l+=1){
                    if(players[l] > max){
                        max = players[l];
                        p = l;
                    }
                }
                return p;
            } 

        }
        num += 1;
        * n_rounds += 1;

    }
}

