#include "../include/fuzz_input_maker.h"
#include "../target/URL_parser/http_program.h"
#include <stdio.h>
#include <string.h>

int
main()
{
    srand(time(0x0));
    
    char src[] = "http://www.google.com/search?q=fuzzing";
    int len = strlen(src);
    char dest[len + 50 + 1];
    len = create_mut_str(50, src, len, dest);
    
    int i = 0;
    
    while(is_valid_url(dest) != 1){
        printf("%s is invalid\n", dest);
        memset(dest, 0, len + 50 + 1);
        len = create_mut_str(50, src, len, dest);
        i++;
    }
    printf("%s is valid! %d - times\n", dest, i);
    return 0;
}