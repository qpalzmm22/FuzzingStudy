#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#define EPSILON 1E-8

double my_sqrt(double x)
{
    double approx;
    double guess = x / 2;

    while(guess != approx){
        approx = guess;
        guess = (approx + x / approx) / 2;   
    }
    return approx;
}

void assert_equal(double x, double y)
{
    double result = fabs(x-y);
    assert(result < EPSILON);
}

double my_sqrt_fixed(double x)
{
    assert(0 <= x);
    if (x == 0){
        return 0;
    }
    return my_sqrt(x);
}

void sqrt_program(double arg)
{
    if (arg < 0)
        printf("Illegal Number\n");
    else
        printf("The root of %lf is %lf", arg, my_sqrt(arg));
}

void measure_time_for_random_nums()
{
    clock_t start, end;
    double time_taken, x;

    start = clock();
    for(int i = 1; i < 100000; i++){
        assert_equal(my_sqrt(i) * my_sqrt(i), i);
    }
    end = clock();

    time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    printf("It took %lf seconds\n", time_taken);

    start = clock();
    for(int i = 1; i < 100000; i++){
        x = 1 + rand();
        assert_equal(my_sqrt(x) * my_sqrt(x), x);
    }
    end = clock();

    time_taken = (double)(end - start) / CLOCKS_PER_SEC;

    printf("It took %lf seconds\n", time_taken);
}


