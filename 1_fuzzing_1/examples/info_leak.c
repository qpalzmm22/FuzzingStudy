#include <stdio.h>
#include <string.h>
#include <time.h>

#include "../include/fuzzer.h"

// Needs to be freed.
char * make_secret()
{
    char *secrets = malloc( 2048 * sizeof(char) ) ;

    int secret_len  = 0 ;
    char *fuzzed_str1 ;
    char *fuzzed_str2 ; 
    char *fuzzed_str3 ;

    secret_len = default_fuzzer_n(&fuzzed_str1) ;
    secret_len += default_fuzzer_n(&fuzzed_str2) ;
    secret_len += default_fuzzer_n(&fuzzed_str3) ;

    char string_format[] = "<space for reply>%s<secret-certificate>%s<secret-key>%s<other-secret>" ;
    secret_len += strlen(string_format) - 6 ; // minus 6 is for %s
    sprintf(secrets, string_format, fuzzed_str1, fuzzed_str2, fuzzed_str3) ;
    
    char deadbeaf[] = "deadbeaf";
    int deafbeaf_len = strlen(deadbeaf);
    int i = 0;

    // Fills the unfilled part with "deadbeaf"s
    while (secret_len < 2048){
        secrets[secret_len++] = deadbeaf[i++] ;
        if(i == deafbeaf_len)
            i = 0;   
    }
    // For printing 
    secrets[secret_len] = 0x0 ;
    
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

    char * send = malloc(length * sizeof(char));
    strncpy(send, memory, length) ;
    return send ;
}


int main()
{
    srand(time(NULL));

    char * secrets = make_secret();
    // safe
    printf("%s\n", heartbleed("potato", strlen("potato"), secrets)) ;
    printf("%s\n", heartbleed("bird", strlen("bird"), secrets)) ;

    // unsafe
    printf("%s\n", heartbleed("hat", 500, secrets)) ;

    free(secrets);

    return 0 ;
}