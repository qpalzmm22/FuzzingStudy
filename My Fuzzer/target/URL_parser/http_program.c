#include "http_program.h"

#define MAX_MATCHES 10

// #define DEBUG

p_url_attr_t
parse_url(char * url)
{
    p_url_attr_t p_urls_attr = (p_url_attr_t)calloc(1, sizeof(url_attr_t));

    char * pos[] = { p_urls_attr->scheme, p_urls_attr->netloc, p_urls_attr->path, p_urls_attr->query, p_urls_attr->fragment };

    regex_t preg;
    if(regcomp( &preg, "^(tcp)+:\\/\\/([^\\/?#]*)([^?#]*)\\?\?([^#]*)#?(.*)$", REG_EXTENDED) != 0){
        fprintf(stderr,"Error in compiling regex");
        exit(1);
    }

    regmatch_t matches[MAX_MATCHES]; 

    if (regexec(&preg, url, MAX_MATCHES, matches, 0) == REG_NOMATCH ) {
        return 0x0;
    }

    size_t len;
    for(int i = 0; i < 5; i++){
        if(matches[i+1].rm_so != -1){
            len = matches[i+1].rm_eo - matches[i+1].rm_so;
            memcpy(pos[i], url+matches[i+1].rm_so, len);
            pos[i][len] = 0x0;
            printf("[%d] : %s\n", i ,pos[i]);
        }
    }

    return p_urls_attr;
}

// 1 for true , 0 for false
int
is_valid_url(char * url)
{
    p_url_attr_t tmp = parse_url(url);
    if(tmp != 0x0){
        free(tmp);
        return 1;
    } else{
        return 0;
    }
}


#ifdef DEBUG

int
main()
{
    is_valid_url("http://wgooE\"0leV.gcAoZmbr``wBe:!^;sbhF?bu;dzIinoE%%");
    is_valid_url("https://www.google.com/asda/asd/as?asd=asdasdasd#heer");
}

#endif /* DEBUG */