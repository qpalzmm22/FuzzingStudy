#include <stdio.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>
#include "../include/fuzzer.h"

// TODO : Functionalize


void crash_if_too_long(char *str)
{
    assert( strlen(str) < 8) ;
}

void bufovf_ex()
{
    // Check fuzzed input
    char * input = default_fuzzer() ;
    crash_if_too_long(input) ;
    
    // copy safe? input
    char weekday[9] ;
    strcpy(weekday, input) ;
    weekday[8] = 0x0;

    printf("weekday : %s\n", weekday);

    free(input) ;
}

// recieve input from user and allocate memory
// `calloc` does similar job to this (checks for integer overflow).
char * alloc_input_size()
{
    char * input = fuzzer(100, '0', 10);

    int n = atoi(input) ;
    free(input) ; 

    // collapse_if_too_large
    assert(n < 1000 && n > 0) ;

    char *ptr = (char*) malloc( n * sizeof(char));
   

    return ptr;
}

void hang_if_no_space(char *s){
    int i = 0;

    while (1) {
        if (i < strlen(s))
            if(s[i] == ' ')
                break;
        i++;
    }
}

void alarm_handler(int sig){
    fprintf(stderr, "Error : It's hanged..\n");
    exit(1);
}

void missing_error_checks(int trials, ssize_t timeout){

    signal(SIGALRM, alarm_handler);

    alarm(timeout);

    for(int i = 0; i < trials; i++){

        char * s = default_fuzzer();
        hang_if_no_space(s);
        free(s);
    }
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    
    // buffer overflow example
    bufovf_ex();

    // // allocate memory of input size
    char * p = alloc_input_size();
    printf("%p\n", p);
    free(p);

    // missing_error_checks
    missing_error_checks(100 , 2);

    return 0;
}
