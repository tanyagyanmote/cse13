#include <stdio.h>
#include "mtrand.h"

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

    for (int i = 0; i <= n_players; i++){
        players[i] = coins_per_player;
    }

    //int i = 0;

    while(i < 5){
        for(int j = 0; j < n_players; j++){
            char turn = spin_dreidel();
            if (turn=='G'){
                players[j] += pot;
                pot = 0;
            }
            if (turn=='H'){
                players[j] += pot/2;
                pot -= pot/2;
            }
            if (turn=='N'){
                break;
            }
            if (turn=='S'){
                if(players[j]==0){
                    continue;
                }
                players[j] -= 1;
                pot += 1;
            }

            //printf("%i", turn);
            //printf("%i\n", players[j]);
            //printf("%i\n", * n_rounds);
            
            for (int k =0; k < n_players;k+=1){
                if(players[k]) < 0){
                    remaining -=1
                }
            }

            if (remanining == 1){
                for (int l = 0; l<n_players; l+=1;){
                    if (players[l] >= 0){
                        return l;
                    }
                }
            }

            * n_rounds += 1;
            i += 1;

        }
    }

    return -1;

}

