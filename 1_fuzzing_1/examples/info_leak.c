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

char * heartbeat(char * reply, ssize_t length, char * memory)
{
    
    if (strlen(reply) > 16){
        fprintf(stderr, "Reply should be less than 16\n") ;
        return 0x0;
    }  

    int reply_len = strlen(reply) ;
    
    //strncpy 가능성
    for (int i = 0; i < reply_len; i++){
        memory[i] = reply[i] ;
    }

    char * send = (char*)malloc(length * sizeof(char));
    strncpy(send, memory, length) ;
    return send ;
}

void test_with_fuzzed(int trials, char *secrets){
    for(int i = 0 ; i < trials ; i ++){
        char * fuzzed_string = default_fuzzer();
        int rand_num = rand() % 500 + 1;
        char * received = heartbeat(fuzzed_string, rand_num, secrets);
        if(received == 0x0){
            continue;
        }
        assert(strstr(received, "deadbeaf") == 0x0);
        assert(strstr(received, "secret") == 0x0);
        
        printf("Passed the test string : %s, num : %d\n", received, rand_num);
    }
}


int main()
{
    srand(time(NULL));

    char * secrets = make_secret();
    char * pot = heartbeat("potato", strlen("potato"), secrets);
    char * bird = heartbeat("bird", strlen("bird"), secrets);
    char * hat = heartbeat("hat", 500, secrets);
    // safe
    printf("%s\n", pot) ;
    printf("%s\n", bird) ;
    free(pot);
    free(bird);

    // unsafe
    printf("%s\n", hat) ;
    free(hat);

    // test with fuzzed inputs_trials
    test_with_fuzzed(10, secrets);

    free(secrets);

    return 0 ;
}