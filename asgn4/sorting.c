#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "shell.h"
#include "stats.h"
#include "bubble.h"
#include "heap.h"
#include "quick.h"
#include "set.h"
#include <unistd.h>
#include "mtrand.h"
#include <inttypes.h>

#define OPTIONS "Habqhsn:p:r:"

void print_h(){
    printf("SYNOPSIS\n");
    printf("   A collection of comparison-based sorting algorithms. \n");
    printf("\n");
    printf("USAGE\n");
    printf("   ./sorting [-Hasbhq] [-n length] [-p elements] [-r seed]\n");
    printf("\n");
    printf("OPTIONS\n");
    printf("   -H            Display program help and usage.\n");
    printf("   -a            Enable all sorts.\n");
    printf("   -b            Enable Bubble Sort.\n");
    printf("   -h            Enable Heap Sort.\n");
    printf("   -q            Enable Quick Sort.\n");
    printf("   -s            Enable Shell Sort.\n");
    printf("   -n length     Specify number of array elements (default: 100).\n");
    printf("   -p elements   Specify number of elements to print (default: 100).\n");
    printf("   -r seed       Specify random seed (default: 13371453).\n");

}

uint32_t* populate_seed (uint64_t default_seed,uint32_t default_num){
    mtrand_seed(default_seed);
    uint32_t *arr = (uint32_t *)calloc(default_num, sizeof(uint32_t));
    for(uint32_t i = 0; i < default_num; i+=1){
        arr[i] = mtrand_rand64() & (uint32_t)((1<<30)-1);
    }
    return arr;
}

int main(int argc, char **argv) {
    Set emp_set = set_empty();
    Stats *stats = malloc(sizeof(Stats));
    uint64_t default_seed = 13371453;
    uint32_t default_num = 100;
    uint32_t default_elements = 100;
    int opt = 0;
    
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
            case 'a':
                emp_set = set_insert(emp_set, 1); // Shell
                emp_set = set_insert(emp_set, 2); // Bubble
                emp_set = set_insert(emp_set, 3); // Quick
                emp_set = set_insert(emp_set, 4); // Heap
                break;
            case 's':
                emp_set = set_insert(emp_set, 1); // Shell
                break;
            case 'b':
                emp_set = set_insert(emp_set, 2);  // Bubble
                break;
            case 'q':
                emp_set = set_insert(emp_set, 3); // Quick
                break;
            case 'h':
                emp_set = set_insert(emp_set, 4); // Heap
                break;
            case 'r':
                default_seed = strtoul(optarg,NULL,10); 
                break;
            case 'n':
                if(atoi(optarg) <= 250000000 && atoi(optarg) >= 1){
                    default_num = atoi(optarg);
                }
                break;
            case 'p':
                default_elements = strtoul(optarg,NULL,10); 
                break;
            case 'H':
                (print_h());
                return 1;
            default: 
                (print_h());
                return 1;
        }
    }

    if(default_num < default_elements){
        default_elements = default_num;
    }
    
    if (set_member(emp_set, 2)) {
        reset(stats);
        uint32_t* arr = populate_seed(default_seed, default_num);
        bubble_sort(stats, arr, default_num);
        uint32_t temp = 0;
        printf("Bubble Sort, %u elements, %lu moves, %lu compares\n", default_num, stats->moves, stats->compares); 
        for (uint32_t i = 0; i < default_elements; i++) {
            printf("%13" PRIu32, arr[i]);
            temp += 1;
            if (temp % 5 == 0) {
                printf("\n");
            }
        }
        if (temp % 5 != 0){
            printf("\n");
        }
    }

    if (set_member(emp_set, 4)) {
        reset(stats);
        uint32_t* arr = populate_seed(default_seed, default_num);
        heap_sort(stats, arr, default_num);
        uint32_t temp = 0;
        printf("Heap Sort, %u elements, %lu moves, %lu compares\n", default_num, stats->moves, stats->compares); 
        
        for (uint32_t i = 0; i < default_elements; i++) {
            printf("%13" PRIu32, arr[i]);
            temp += 1;
            if (temp % 5 == 0) {
                printf("\n");
            }
            
        }
        if (temp % 5 != 0){
            printf("\n");
        }
    }

    if (set_member(emp_set, 3)) {
        reset(stats);
        uint32_t* arr = populate_seed(default_seed, default_num);
        quick_sort(stats, arr, default_num);
        uint32_t temp = 0;
        printf("Quick Sort, %u elements, %lu moves, %lu compares\n", default_num, stats->moves, stats->compares); 
        for (uint32_t i = 0; i < default_elements; i++) {
            printf("%13" PRIu32, arr[i]);
            temp += 1;
            if (temp % 5 == 0) {
                printf("\n");
            }
        }
        if (temp % 5 != 0){
            printf("\n");
        }
    }

    if (set_member(emp_set, 1)) {
        reset(stats);
        uint32_t* arr = populate_seed(default_seed, default_num);
        shell_sort(stats, arr, default_num);
        uint32_t temp = 0;
        printf("Shell Sort, %u elements, %lu moves, %lu compares\n", default_num, stats->moves, stats->compares); 
        for (uint32_t i = 0; i < default_elements; i++) {
            printf("%13" PRIu32, arr[i]);
            temp += 1;
            if (temp % 5 == 0) {
                printf("\n");
            }
        }
        if (temp % 5 != 0){
            printf("\n");
        }
    }


    // uint32_t arr[] = {4,5,2,7,8,7,5,1,6};
    // Stats *stats = (Stats *) calloc(1,sizeof(Stats));
    // heapsort(stats,arr,9);
    // for(int i =0; i<9;i+=1){
    //     printf("%d\n",arr[i]);
    // }

}
