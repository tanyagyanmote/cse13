#include "bubble.h"

#include "set.h"
#include "stats.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>


void bubble_sort(Stats *stats, uint32_t *arr, uint32_t n_elements){
    bool swapped = False;
    for(int i = 0; i<(n_elements-1); i++){
        swapped = True;
        for(int j = (n_elements-1); j < -1; j+=i){
            if (arr[j] < arr[j-1]){
                swap(stats, arr[j],arr[i]);
                swapped = True;
            }
        }
        if(swapped == False){
            break;
        }
    }

}


