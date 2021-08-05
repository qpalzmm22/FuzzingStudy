#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Ex3 part2
double * quadratic_solver(double a, double b, double c)
{
    double * solutions = (double*)malloc(sizeof(double) * 2);

    if(a == 0){
        if(b == 0){
            if(c == 0){
                solutions[0] = 0;
                solutions[1] = 0;
                return solutions;
            } else{
                // unsolvable
                solutions = 0x0;
                return solutions;

            }
        }
        else {
            solutions = 0x0;
            return solutions;
        }
    }  
    
    double q = b * b - 4 * a * c;
    if (q < 0){
        solutions = 0x0;
        return solutions;
    }

    solutions[0] = (-b + my_sqrt_fixed(q)) / (2*a);
    solutions[1] = (-b - my_sqrt_fixed(q)) / (2*a);
    
    return solutions;
}

void do_quad(double a, double b, double c)
{
    printf("Do Quad of %lf %lf %lf\n", a, b, c);

    double* solutions = quadratic_solver(a,b,c);
    if(solutions == 0x0)
        printf("(No solutions or imaginary number included)\n");
    else    
        printf("(%lf , %lf)\n", solutions[0], solutions[1]);

    free(solutions);
}

// There are some assumptions for solving this problem
// 1. intputs are integers. Or else, the possibility increases by infinite
// 2. we are only considering finding a and b becoming 0 bug. 
void calc_part3(){
    unsigned long long combinations = 1;
    combinations = (combinations << 63);
    unsigned long long compute_per_year = 1000000000;
    compute_per_year = compute_per_year * 3600 * 24 * 365;
    printf("It takes %llu years to complete 2^64 cases testing\n", (combinations/compute_per_year) *2);

}

// ex4
void calc_part4(){
    float inf =  __FLT_MAX__ * 1000;
    // this hangs the program. 
    printf("sqrt of %f is %f\n", inf,my_sqrt_fixed(inf));
}
