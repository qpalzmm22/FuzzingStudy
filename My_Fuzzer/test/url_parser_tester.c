#include "../include/fuzz_input_maker.h"
#include "../target/URL_parser/http_program.h"
#include <stdio.h>
#include <string.h>

// do mutation fuzzing until it's valid url.
int
main()
{
    srand(time(0x0));
    
    char src[] = "http://www.google.com/search?q=fuzzing";
    int len = strlen(src);
    char dest[1024];
    len = create_mut_str(100, src, len, dest);
    
    unsigned long long i = 0;
    
    while(is_valid_url(dest) != 1){
        //printf("%llu : %s is invalid\n",i, dest);
        memset(dest, 0, 1024);
        len = create_mut_str(100, src, len, dest);
        i++;
    }
    printf("%s is valid! %llu - times\n", dest, i);
    return 0;
}