#include <stdio.h>
#include "heap.h"
#include "set.h"
#include "stats.h"

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

uint32_t l_child (uint32_t n){
    return (2*n+1);
}
uint32_t r_child (uint32_t n){
    return (2*n+2);
}
uint32_t parent (uint32_t n){
    return ((n-1)/2);
}

void up_heap (Stats *stats, uint32_t *arr, uint32_t n){
    while (n>0 && (cmp(stats,arr[n],arr[parent(n)]) == -1)){
        swap(stats,&arr[n],&arr[parent(n)]);
        n = parent(n);
    }
}

void down_heap(Stats *stats, uint32_t *arr, uint32_t heap_size){
    uint32_t n = 0;
    uint32_t smaller;
    while((l_child(n)) < heap_size){
        if (r_child(n) == heap_size){
            smaller = l_child(n);
        }
        else {
            if (cmp(stats,arr[l_child(n)],arr[r_child(n)]) == -1){
                smaller = l_child(n);
            }
            else{
                smaller = r_child(n);
            }
        }
        if (cmp(stats, arr[n],arr[smaller]) == -1){
            break;
        }
        swap(stats,&arr[n],&arr[smaller]);
        n = smaller;
    }
}

uint32_t* build_heap(Stats *stats, uint32_t *arr, uint32_t n_elements){
    uint32_t *heap = (uint32_t *)calloc(n_elements,sizeof(uint32_t));
    for(uint32_t n =0; n < n_elements; n+=1){
        heap[n] = move(stats,move(stats, arr[n]));
        up_heap(stats, heap,n);
    }
    return heap;
}

void heap_sort(Stats *stats, uint32_t *arr, uint32_t n_elements){
    uint32_t *heap = (build_heap(stats,arr,n_elements));
    //uint32_t *sorted_list = (uint32_t *)calloc(n_elements,sizeof(uint32_t));
    for(uint32_t n =0; n < n_elements; n+=1){
        arr[n] = move(stats,move(stats, heap[0]));
        heap[0] = move(stats,move(stats,heap[n_elements-n-1]));
        down_heap(stats,heap,n_elements-n);
    }
    free(heap);

}




