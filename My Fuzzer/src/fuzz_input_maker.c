#include "../include/fuzz_input_maker.h"
/*

	dest str must be initialize before function call.

*/

// #define DEBUG


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

int
delete_byte(char *src, int len, char *dest)
{
	if(len == 0){
		memcpy(dest, src, len+1);
		return 0;
	}
	char tmp_src[len + 1];
	memcpy(tmp_src, src, len + 1);
	int ind = rand() % len ;
	
	memcpy(dest, tmp_src, ind);
	memcpy(dest + ind, tmp_src + ind + 1, len - ind);
	return len - 1;
}

int
insert_byte(char *src, int len, char *dest)
{
	char tmp_src[len + 1];
	memcpy(tmp_src, src, len + 1);
	int ind = rand() % (len + 1);

	memcpy(dest, tmp_src, ind);
	dest[ind] = rand() % (127-32) + 32; //32 ~ 126 
	memcpy(dest + ind + 1, tmp_src + ind, len - ind + 1);
	return len + 1 ;
}

int
flip_bit(char *src, int len, char *dest)
{
	if(len == 0){
		memcpy(dest, src, len+1);
		return len;
	}
	char tmp_src[len + 1];
	memcpy(tmp_src, src, len + 1);
	int ind = rand() % len ;
	
	unsigned int bit = rand() % 7 ; // 0 ~ 6
	bit = 1 << bit;

	memcpy(dest, tmp_src, ind);
	dest[ind] = tmp_src[ind] ^ bit;

	memcpy(dest + ind + 1, tmp_src + ind + 1, len - ind);
	return len ;
}


int
run_mut(char * src, int len, char *dest)
{
    int(* fp[3])(char* src, int len, char *dest) = {delete_byte, insert_byte, flip_bit};

	int mut_ind = rand() % 3;

	return (*fp[mut_ind])(src, len, dest);
}

int
create_mut_str(int max_mut, char *src, int len, char *dest)
{
	int itr = rand() % max_mut + 1 ;
	memcpy(dest, src, len + 1);

	for(int i = 0; i < itr; i++){
		// if(i % 5 == 0)
		//  	printf("%d : %s\n",i , dest);
		len = run_mut(dest, len, dest);
	}
	return len;
}


#ifdef DEBUG

int
main()
{
	srand(time(0x0));

	char seed[] = "Hello World";
	int len = strlen(seed);
	char dest[1024];

	run_mut(seed, len, dest);

	printf("str: %s\n", dest);

	create_mut_str(10, seed, len, dest);

	printf("dest: %s\n", dest);

	return 0;
}

#endif /* DEBUG */