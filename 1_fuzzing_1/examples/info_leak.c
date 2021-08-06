#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../include/fuzzer.h"

// Needs to be freed.
char * make_secret()
{
    char *secrets = malloc( 2048 * sizeof(char) ) ;

    int message_len  = 0 ;
    char *fuzzed_str1 ;
    char *fuzzed_str2 ; 
    char *fuzzed_str3 ;

    message_len = default_fuzzer_n(&fuzzed_str1) ;
    message_len += default_fuzzer_n(&fuzzed_str2) ;
    message_len += default_fuzzer_n(&fuzzed_str3) ;

    char string_format[] = "<space for reply>%s<secret-certificate>%s<secret-key>%s<other-secret>" ;
    message_len += strlen(string_format) - 6 ; // minus 6 is for %s
    sprintf(secrets, string_format, fuzzed_str1, fuzzed_str2, fuzzed_str3) ;
    
    char deadbeaf[] = "deadbeaf";
    int deafbeaf_len = strlen(deadbeaf);
    int i = 0;

    // Fills the unfilled part with "deadbeaf"s
    while (message_len < 2048){
        secrets[message_len++] = deadbeaf[i++] ;
        if(i == deafbeaf_len)
            i = 0;   
    }

    // For printing 
    secrets[message_len] = 0x0 ;
    
    free( fuzzed_str1 ) ;
    free( fuzzed_str2 ) ;
    free( fuzzed_str3 ) ;

    return secrets ;
}

char * heartbleed(char * reply, ssize_t length, char * memory)
{
    
    if (strlen(reply) > 16){
        fprintf(stderr, "Reply should be less than 16") ;
        exit(1) ;
    }  

    int reply_len = strlen(reply) ;
    
    for (int i = 0; i < reply_len; i++){
        memory[i] = reply[i] ;
    }

    char * send = (char*)malloc(length * sizeof(char));
    strncpy(send, memory, length) ;
    return send ;
}


int main()
{
    srand(time(NULL));

    char * secrets = make_secret();
    char * pot = heartbleed("potato", strlen("potato"), secrets);
    char * bird = heartbleed("bird", strlen("bird"), secrets);
    char * hat = heartbleed("hat", 500, secrets);
    // safe
    printf("%s\n", pot) ;
    printf("%s\n", bird) ;

    // unsafe
    printf("%s\n", hat) ;

    free(secrets);
    free(pot);
    free(bird);
    free(hat);
    
    return 0 ;
}