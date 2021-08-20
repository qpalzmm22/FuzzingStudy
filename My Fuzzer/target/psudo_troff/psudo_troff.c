#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

/*
> Error Cases
1. The input sequence \D (backslash + D) followed by a non-printable character
2. A character in the ASCII range 128–255 (i.e., the 8th bit is set) followed by a newline character
3. A single dot (.) followed by a newline character.

*/


int
main(){
 
    char c;
    int slash_flag = 0;
    int d_flag = 0;
    int dot_flag = 0;
    int bit8_flag = 0;

    while((c = getchar()) != EOF){

        if(d_flag && (c < 9 || ( c > 13 && c < 31 ) || c > 126)){
            fprintf(stderr, "\\D followed by non-printable\n");
            exit(1);
        } 
        if(slash_flag && c == 'D'){
            d_flag = 1 ;
        }else {
            d_flag = 0 ;
        }
        if(c == '\\'){
            slash_flag = 1;
        } else {
            slash_flag = 0;
        }

        
        // . followed by new line
        if(dot_flag && c == '\n'){
            fprintf(stderr, "dot followed by new line\n");
            exit(2);
        }
        if(c == '.'){
            dot_flag = 1;
        } else{
            dot_flag = 0;
        }
        

        // 8 bit ascii followed by new line
        if(bit8_flag && c == '\n'){
            fprintf(stderr, "ascii between 128 to 255 followed by new line\n");
            exit(3);
        }
        if(c >= 128 || c < 0){
            bit8_flag = 1;
        } else {
            bit8_flag = 0;
        }
        
    }

    return 0;
}
