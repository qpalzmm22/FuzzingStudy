#include "../include/fuzz_input_maker.h"



// returns 'len + 1' length of string with '\0' at the end 
int 
create_rand_str(in_config_t in_config, char * dest)
{
    int min_len = in_config.min_len ;
    int max_len = in_config.max_len ;
    int char_start = in_config.ch_start ;
    int char_range = in_config.ch_range ;
    
	int len ;	

	len = rand() % (max_len - min_len + 1) + min_len ; 

	for(int i = 0; i < len; i++){
		dest[i] = char_start + rand() % (char_range + 1) ;
	}
	dest[len] = '\0' ;

	return len ;
}