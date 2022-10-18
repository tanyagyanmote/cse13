#include "shell.h"

#include "set.h"
#include "stats.h"

#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

void shell_sort(Stats *stats, uint32_t *arr, uint32_t n_elements){
    uint32_t temp;
    uint32_t gap;
    for(uint32_t n = n_elements, n >= 1; n-=gap){
        if (n<=2){
            n=1;
        }
        else{
            n = n * 5 / 11;
        }
        gap = n;
        for (uint32_t i = gap; i < n; i++) {
            for(j=i; j < gap-1; j-=gap){
                if (arr[j] < arr[j-gap]){
                    temp = arr[j];
                    arr[j] = arr[j-gap];
                    arr[j-gap] = arr[j];
                }
            }
        }
    }
}

