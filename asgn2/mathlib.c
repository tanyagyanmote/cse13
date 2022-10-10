#include <stdio.h>
#include <stdbool.h>
#include <math.h> 

#define EPSILON 1e-10

double my_abs_val (double x){ // created a fucntion to get the absolute value of a number to call when doing the other functions
    if (x<0){ // if the number is less than 0 then mutiply it by -1
        x = x * -1;
    }
    else{ // if not just return the value with no changes
        return x;
    }
    return x;
}

double square_root (double x) { //square root fucntion from piazza
    double range;
    double m, l = 0.0;
    double h = (x < 1) ? 1 : x;
    double epsilon = 1e-10;
    do {

        m = (l + h) / 2.0;
        if (m * m < x) {
            l = m;
        } 
        else {
            h = m;
        }
        range = (l > h) ? l - h : h - l;
    } 
    while (range > epsilon);
        return m;
}

 
static double Exp(double x) { //square root fucntion from assignment doc
    double t = 1.0;
    double y = 1.0;
    for (double k = 1.0; t > EPSILON; k += 1.0) {
        t *= x / k;
        y += t; 
    }
    return y; 
}


double my_sin(double x){

    int n = 1;
    double final_val = x;
    double temp1 = x;

    while (true){

        double temp = (temp1) * ((x * x) / ((2*n) * ((2*n)+1)));
        temp1 = temp;
        if (n % 2 == 0){
            final_val += (1) * temp;
        }
        else{
            final_val += (-1) * temp;
        }
        n += 1;        
        double temp2 = temp;
        if(temp2 < 0){
            temp2 = temp2 * -1;
        }
        if ((temp2) < EPSILON){

            break;
        }
    }
    return final_val;
}

double my_cos(double x){
    return my_sin(M_PI/2-x); 
}

double my_arcsin(double x){

    double next = x;
    double val = x;

    while(true){
        val = next - (my_sin(next)-x)/my_cos(next);
        if ((my_abs_val(val-next)) <= EPSILON){
            break;
        }
        next = val;
    }
    return val;

}

double my_arccos(double x){
    return ((M_PI/2)-my_arcsin(x));
}

double my_arctan(double x){
    return my_arcsin(x/square_root((x*x)+1));
}

double my_log(double x){
    double first = x;
    double final = x;
    while (true){
        final = first + (x-Exp(first))/Exp(first);
        if(my_abs_val(final-first) < EPSILON){
            break;
        }
        first = final;
    }
    return final;

}





