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

    ssize_t recv; 
    char c;
    int slash_flag = 0;
    int d_flag = 0;
    int dot_flag = 0;
    int bit8_flag = 0;

    while((c = getchar()) != EOF){
        printf("%c", c);

        // non-printable
        if(c == '\\'){
            slash_flag = 1;
        }
        if(slash_flag && c == 'D'){
            d_flag = 1 ;
        } else {
            slash_flag = 0;
        }
        if(d_flag && (c < 9 || ( c > 13 && c < 31 ) || c > 126)){
            fprintf(stderr, "\\D followed by non-printable\n");
            exit(1);
        } else {
            d_flag = 0;
        }

        // . followed by new line
        if(c == '.'){
            dot_flag = 1;
        }
        if(dot_flag && c == '\n'){
            fprintf(stderr, ". followed by new line\n");
            exit(1);
        } else {
            dot_flag = 0;
        }

        // 8 bit ascii followed by new line
        if( c >= 128 && c <= 255){
            bit8_flag = 1;
        }
        if(bit8_flag && c == '\n'){
            fprintf(stderr, "ascii between 128 to 255 followed by new line\n");
            exit(1);
        } else {
            bit8_flag = 0 ; 
        }
    }

    return 0;
}


        // if(i > 0 && c == '\n'){
        //     if( buf[i - 1]  == '.'){
        //         fprintf(stderr, ". followed by new line\n");
        //         exit(1);
        //     }
        // } 
        //     if( buf[i - 1] >= 128 && buf[i - 1] <= 255){
        //         fprintf(stderr, "ascii between 128 to 255 followed by new line\n");
        //         exit(1);
        //     }

        //     if( i + 2 <= recv && buf[i] == '\\'){
        //         if( buf[i + 1] == 'D'){
        //             // non-printable
        //             if(buf[i + 2] < 9 || buf[i + 2] > 13 && buf[i + 2] < 31 || buf[i + 2] > 126){
        //                 fprintf(stderr, "\\Dfollowed by non-printable\n");
        //                 exit(1);
        //             } 
        //         }
        //     }
        // }