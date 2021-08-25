#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>


typedef struct _url_attr
{
    char scheme[32];
    char netloc[1024];
    char path[1024];
    char query[1024];
    char fragment[1024];
}url_attr_t, *p_url_attr_t;

p_url_attr_t
parse_url(char * url);

int
is_valid_url(char * url);