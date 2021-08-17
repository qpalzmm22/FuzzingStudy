#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

int 
hex2int(char num)
{
    if(isdigit(num)){
        return num - '0';
    } else if(isupper(num)){
        return num - 'A' + 10;
    } else if(islower(num)){
        return num - 'a' + 10;
    }
    return -1;
}


char * 
cgi_decode(char * str, int len)
{
    char * ret_str = (char*)malloc((len + 1) * sizeof(char)); 
    assert(ret_str);

    int i = 0;
    int j = 0;
    while(i < len){
        char c = str[i];
        if(c == '+'){
            ret_str[j++] = ' ';
        } else if( c == '%'){
            // what if the str ends with % => seg_fault
            char high_digit = str[i+1];
            char low_digit = str[i+2];

            i += 2;
            
            if(isxdigit(high_digit) && isxdigit(low_digit)){
                int value = hex2int(high_digit) * 16 + hex2int(low_digit) ;
                ret_str[j++] = value;
            } else {
                fprintf(stderr, "Invalide Encoding\n");
                return 0x0;
            }
        } else {
            ret_str[j++] = c;
        }
        i++;
    }
    ret_str[j] = 0x0;

    return ret_str;
}

char * no_fuzz_cgi_decode(char * str)
{
    return cgi_decode(str, strlen(str));
}

int main()
{
    // Test1
    char* str_to_decode = "Hello +world%21";
    int len = strlen(str_to_decode);

    char * decoded_str = cgi_decode(str_to_decode, len);
    printf("decoded string : %s\n", decoded_str);
    
    // Test2

    assert(strcmp(no_fuzz_cgi_decode("+"), " ") == 0);
    assert(strcmp(no_fuzz_cgi_decode("%20"), " ") == 0);
    assert(strcmp(no_fuzz_cgi_decode("abc"), "abc") == 0);
    
    assert(no_fuzz_cgi_decode("%?a") == 0x0);

    free(decoded_str);
    return 0;
}