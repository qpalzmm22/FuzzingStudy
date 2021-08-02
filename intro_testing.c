#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <assert.h>

#define EPSILON 1E-8

double my_sqrt(double x){
    double approx;
    double guess = x / 2;

    while(guess != approx){
        printf("approx = %llf\n", approx);
        approx = guess;
        guess = (approx + x / approx) / 2;   
    }
    return approx;
}

void assertEqual(double x, double y){
    //printf("x : %llf, y : %llf fabs: %llf\n", x, y, fabs(x-y));
    assert(fabs(x - y) < EPSILON);
}

int main(){
    for(int i = 1 ; i < 1000; i++){
        assertEqual(my_sqrt(i) * my_sqrt(i), i);
    }
    return 0;
}

