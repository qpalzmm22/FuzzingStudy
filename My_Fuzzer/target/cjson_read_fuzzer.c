#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "cJSON.h"


int cjson_read_fuzzer(const uint8_t* data, size_t size)
{
    cJSON *json;
    size_t offset = 4;
    unsigned char *copied;
    char *printed_json = NULL;
    int minify, require_termination, formatted, buffered;


    if(size <= offset) return 0;
    if(data[size] != '\0') return 0;
    if(data[0] != '1' && data[0] != '0') return 0;
    if(data[1] != '1' && data[1] != '0') return 0;
    if(data[2] != '1' && data[2] != '0') return 0;
    if(data[3] != '1' && data[3] != '0') return 0;

    minify              = data[0] == '1' ? 1 : 0;
    require_termination = data[1] == '1' ? 1 : 0;
    formatted           = data[2] == '1' ? 1 : 0;
    buffered            = data[3] == '1' ? 1 : 0;

    json = cJSON_ParseWithOpts((const char*)data + offset, NULL, require_termination);

    if(json == NULL) return 0;

    if(buffered)
    {
        printed_json = cJSON_PrintBuffered(json, 1, formatted);
        
    }
    else
    {
        /* unbuffered printing */
        if(formatted)
        {
            printed_json = cJSON_Print(json);
        }
        else
        {
            printed_json = cJSON_PrintUnformatted(json);
        }
    }
    printf("%s\n", printed_json);

    if(printed_json != NULL) free(printed_json);

    if(minify)
    {
        copied = (unsigned char*)malloc(size);
        if(copied == NULL) return 0;

        memcpy(copied, data, size);

        cJSON_Minify((char*)copied + offset);

        free(copied);
    }

    cJSON_Delete(json);

    return 0;
}

int
main(int argc, char **argv)
{
    if(argc < 2){
        fprintf(stderr, "Usage : ./cJSON_read_tester [parse file]]");
        exit(1);
    }
    
    FILE * f = fopen(argv[1], "rb");

    if(f == NULL)
    {
        fprintf(stderr, "error opening input file %s\n", argv[1]);
        return 0;
    }

    fseek(f, 0, SEEK_END);

    long siz_buf = ftell(f);
    rewind(f);

    if(siz_buf < 1) return 0;

    char * buf = (char*)malloc((size_t)siz_buf);
    if(buf == NULL)
    {
        fprintf(stderr, "malloc() failed\n");
        free(buf); 
    }

    if(fread(buf, (size_t)siz_buf, 1, f) != 1)
    {
        fprintf(stderr, "fread() failed\n");
        free(buf); 
    }
    cjson_read_fuzzer((uint8_t *)buf, (size_t)siz_buf);
       
    return 0;
}
