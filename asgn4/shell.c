#include "shell.h"

#include "set.h"
#include "stats.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

uint32_t gap(uint32_t n){
    if (n<=2){
        return n-1;
    }
    if(n<=0){
        return 0;
    }
    else{
        return (n * 5) / 11;
    }
}

void shell_sort(Stats *stats, uint32_t *arr, uint32_t n_elements){
    for (uint32_t step = gap(n_elements); step > 0; step = gap(step)){
        for (uint32_t i = step; i < n_elements; i++) {
            uint32_t j = i;
            uint32_t temp = move(stats, arr[i]);
            while (j >= step && cmp(stats, temp, arr[j - step]) == -1) {
                arr[j] = move(stats, arr[j - step]);
                move(stats,arr[j]);
                //move(stats,arr[j-step]);
                j = j - step;
            }
            arr[j] = move(stats, temp);
            move(stats,arr[j]);
        }
    }
}

