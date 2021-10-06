#include "cJSON.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

char *
test_functions(char *str){
    int len = strlen(str);
    
    cJSON *root = cJSON_CreateObject();
    char *string = NULL;

    int i = 0 ;
    cJSON *parent = root;
    while(i < len){
        cJSON *child;
        char * arg = (char *)malloc(len * sizeof(char));
        assert(arg);

        char * arg2 ;
        
        char cha = str[i++];
        int arglen = str[i] < len - i ? str[i] - '0' : len - i;
        memcpy(arg, str + i, arglen);
        arg[arglen] = '\0';
        i += arglen;

        switch(cha){
            case 'O':  // Object
                child = cJSON_CreateObject();
                if(child == 0x0){
                    goto end;
                }
                
                cJSON_AddItemToObject(parent, arg, child);
                parent = child;
                break ;
                
            case 'S':  // String
                arglen = str[i] < len - i ? str[i] - '0' : len - i;
                arg2 = (char *)malloc(arglen * sizeof(char));
                memcpy(arg, str + i, arglen);
                arg[arglen] = '\0';
                i += arglen;

                if(cJSON_AddStringToObject(parent, arg, arg2) == 0x0){
                    free(arg2);
                    goto end;
                }
                free(arg2);
                break ;

            case 'N':  // Number
                arglen = str[i] < len - i ? str[i] - '0' : len - i;
                arg2 = (char *)malloc(arglen * sizeof(char));
                memcpy(arg, str + i, arglen);
                arg[arglen] = '\0';
                i += arglen;

                if(cJSON_AddNumberToObject(parent, arg, atoi(arg2))  == 0x0){
                    free(arg2);
                    goto end;
                }
                free(arg2);
                break ;

            default :
                break ;
        }
        free(arg);
    }
    string = cJSON_Print(root);
    if (string == NULL)
    {
        fprintf(stderr, "Failed to print root.\n");
    }

end:
    cJSON_Delete(root);
    return string;
}

int
main(int argc, char **argv)
{
    if(argc < 2){
        fprintf(stderr, "Usage : ./cJSON_tester [cJSON cmd]");
        exit(1);
    }
    char * str = test_functions(argv[1]);
    if(str != 0x0){
        printf("%s\n", str);
    }    
}
