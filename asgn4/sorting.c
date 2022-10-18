#include <stdio.h>
#include <stdbool.h>
#include "shell.h"
#include "stats.h"


#define OPTIONS "sn:p:r:"


int main(int argc, char **argv) {
    uint64_t default_seed = 13371453;
    int default_num = 100;
    int default_elements = 100;

    mtrand_seed(default_seed);
     while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
            case 's':

            case 'n':
            default_num = atoi(optarg); 
            if(default_num <= 250000000 && default_num >= 1){
                break;
            }
            return 1;  

            case 'p':

            case 'r':

        }
}