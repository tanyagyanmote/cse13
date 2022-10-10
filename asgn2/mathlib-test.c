#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h> // For getopt().
#include "mathlib.h"

int main(int argc, char **argv) {
    bool opt_s = false; //creating bool options, for cases like -a, -a
    bool opt_c = false;
    bool opt_S = false;
    bool opt_C = false;
    bool opt_T = false;
    bool opt_l = false;
    int opt = 0;
    double difference = 0.0;

    #define OPTIONS "ascSCTl"

    while ((opt = getopt(argc, argv, OPTIONS)) != -1){
        switch(opt){ // creating switch options for the different options
        case 'a':
            opt_s = true;
            opt_c = true;
            opt_S = true;
            opt_C = true;
            opt_T = true;
            opt_l = true;
            break;
        case 's':
            opt_s = true;
            break;
        case 'c':
            opt_c = true;
            break;
        case 'S':
            opt_S = true;
            break;
        case 'C':
            opt_C = true;
            break;
        case 'T':
            opt_T = true;
            break;
        case 'l':
            opt_l = true;                                                                                                                                                                                                   
            break;

        default: printf("error\n");
        
        }
    }

    if (opt_s == true) { //printing the table with x, sin, library value, difference 
       printf("  x\t      sin\t       Library\t     Difference\n");
       printf("  -\t      ----\t       -------\t     -----------\n");
       for (double x=0.0; x <= 2*M_PI;x+= 0.05*M_PI){
            difference = (my_sin(x)-sin(x));
            printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n",x,my_sin(x),sin(x),difference);
       }
    }
    if (opt_c == true) {
        printf("  x\t      cos\t       Library\t     Difference\n");
        printf("  -\t      ----\t       -------\t     -----------\n");
        for (double x=0.0; x <= 2*M_PI;x+= 0.05*M_PI){
            difference = (my_cos(x)-cos(x));
            printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n",x,my_cos(x),cos(x),difference);
       }
       
    }
    if (opt_S == true) {
        printf("  x\t      arcsin\t       Library\t     Difference\n");
        printf("  -\t      -------\t       -------\t     -----------\n");
        for (double x=-1; x >= -1.0 && x <= 1.0 ;x+= 0.05){
            difference = (my_arcsin(x)-asin(x));
            printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n",x,my_arcsin(x),asin(x),difference);
       }

    }
    if (opt_C == true) {
        printf("  x\t      arccos\t       Library\t     Difference\n");
        printf("  -\t      -------\t       -------\t     -----------\n");
        for (double x=-1; x >= -1.0 && x <= 1.0 ;x+= 0.05){
            difference = (my_arccos(x)-acos(x));
            printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n",x,my_arccos(x),acos(x),difference);
       }
        
    }
    if (opt_T == true) {
        printf("  x\t      arctan\t       Library\t     Difference\n");
        printf("  -\t      -------\t       -------\t     -----------\n");
        for (double x=1; x < 10 ;x+= 0.05){
            difference = (my_arctan(x)-atan(x));
            printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n",x,my_arctan(x),atan(x),difference);
       }
    }
    if (opt_l == true) {
       printf("  x\t      log\t       Library\t     Difference\n");
        printf("  -\t      -----\t       -------\t     -----------\n");
        for (double x=1; x < 10 ;x+= 0.05){
            difference = (my_log(x)-log(x));
            printf(" %7.4lf % 16.9lf % 16.9lf % 16.12lf\n",x,my_log(x),log(x),difference);
       }
    }
}







 



