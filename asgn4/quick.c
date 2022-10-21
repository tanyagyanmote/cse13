#include "quick.h"
#include "set.h"
#include "stats.h"
#include "shell.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

uint32_t SMALL = 8;
void quick_sort(Stats *stats, uint32_t *arr, uint32_t n_elements){
    if (n_elements < SMALL){
        shell_sort(stats,arr,n_elements);
        return;
    }
    uint32_t l = 0;
    uint32_t r = n_elements - 1;
    uint32_t pivot = (arr[0] + arr[n_elements/2] + arr[n_elements-1])/3;
    while (l < r){
        while(cmp(stats,pivot,arr[l]) == 1 || arr[l] == pivot){
            l += 1;
        }
        if (l == n_elements) {
            return;
        }
        while(cmp(stats,pivot,arr[r]) == -1){
            r -= 1;
        }
        if(r>l){
            swap(stats,&arr[l],&arr[r]);
        }
    }
    
    quick_sort(stats,&arr[0],l);

    quick_sort(stats,&arr[l],n_elements-l);
    

}

