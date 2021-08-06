#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "../include/fuzzer.h"

// TODO : Functionalize


void crash_if_too_long(char *str)
{
    printf("num : %s\n", str);
    assert( strlen(str) < 8) ;
}

// recieve input from user and allocate memory
// `calloc` does similar job to this (checks for integer overflow).
void alloc_in_size(char *in)
{ 
    int n = atoi(in) ;
    printf("n : %d\n", n);
    // collapse_if_too_large
    assert(n < 1000 && n > 0) ;

}

int main(int argc, char **argv)
{
    srand(time(NULL));
    char *input ;

    

#ifdef BUFOVF_UI
    if(argc != 2){
        printf("Needs only one and only one argument\n") ;
        return -1 ;
    }

    input = argv[1] ;
#endif

#ifdef BUFOVF_FUZZ
    input = default_fuzzer() ;
    crash_if_too_long(input) ;
    printf("hello\n") ;

    char weekday[9] ;
    strcpy(weekday, input) ;
    free(input) ;
#endif
    
#ifdef BIG_INPUT_SIZE_UI
    if(argc != 2){
        printf("Needs only one and only one argument\n") ;
        return -1 ;
    }
    input = argv[1] ;
#endif

#ifdef BIG_INPUT_SIZE_FUZZ
    input = fuzzer(100, '0', 10);
    alloc_in_size(input) ;`2
    free(input) ;
#endif

    return 0;
}
