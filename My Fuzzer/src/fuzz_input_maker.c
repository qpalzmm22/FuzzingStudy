#include "../include/fuzz_input_maker.h"
/*

	dest str must be initialize before function call.

*/

// #define DEBUG

static int8_t interesting_8[] = { KNOWNINT_8 };
static int16_t interesting_16[] = { KNOWNINT_8, KNOWNINT_16 };
static int32_t interesting_32[] = { KNOWNINT_8, KNOWNINT_16, KNOWNINT_32 };


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


// Uses tmp_src to be prepared when src and dest are same. 
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

	int lg_num_bit = rand() % 3 ;
	int num_bit = 1 << lg_num_bit ;

	int bit = rand() % (9 - num_bit) ; // 0 ~ 7
	unsigned int xor_bit = 0;
	unsigned int ind_bit;
	for(int i = 0 ; i < num_bit; i++){
		ind_bit = 1 << (bit + i); 
		xor_bit = xor_bit | ind_bit;
	}  

	memcpy(dest, tmp_src, ind);
	dest[ind] = tmp_src[ind] ^ xor_bit;

	memcpy(dest + ind + 1, tmp_src + ind + 1, len - ind);
	return len ;
}


// 8 bit flip
int
flip_byte(char *src, int len, char *dest)
{
	if(len == 0){
		memcpy(dest, src, len+1);
		return len;
	}
	char tmp_src[len + 1];
	memcpy(tmp_src, src, len + 1);
	
	int lg_num_bytes;   // 0 => 1byte, 1 => 2byte, 2 => 4byte 
	if(len >= 4){
		lg_num_bytes = rand() % 3;
	} else if(len >= 2){
		lg_num_bytes = rand() % 2;
	} else {
		lg_num_bytes = 0;
	}

	int num_bytes = (1 << lg_num_bytes);

	unsigned int bit = 0xFF; // b'11111111	

	int ind = rand() % (len - num_bytes) ;

	memcpy(dest, tmp_src, ind);
	for(int i = 0; i < num_bytes; i++){
		dest[ind + i] = tmp_src[ind + i] ^ bit;
	}
	
	memcpy(dest + ind + 1, tmp_src + ind + 1, len - ind);
	return len ;
}

int
simp_arith(char *src, int len, char *dest)
{
	if(len == 0){
		memcpy(dest, src, len+1);
		return len;
	}
	char tmp_src[len + 1];
	memcpy(tmp_src, src, len + 1);
	
	int operand = rand() % (ARITH_MAX * 2 + 1) - 35; // [-35 ~ 35] inclusive
	int ind = rand() % len;

	memcpy(dest, tmp_src, ind);
	dest[ind] = tmp_src[ind] + operand ; 

	return len;
}


int
swap_known_ints(char *src, int len, char *dest)
{
	if(len == 0){
		memcpy(dest, src, len+1);
		return len;
	}
	char tmp_src[len + 1];
	memcpy(tmp_src, src, len + 1);

	int lg_num_bytes;   // 0 => 1byte, 1 => 2byte, 2 => 4byte 
	int num_bytes ;

	memcpy(dest, src, len + 1);
	if(len >= 4){
		lg_num_bytes = rand() % 3;
		num_bytes = (1 << lg_num_bytes);
	} else if(len >= 2){
		lg_num_bytes = rand() % 2;
		num_bytes = (1 << lg_num_bytes);
	} else {
		lg_num_bytes = 0;
		num_bytes = (1 << lg_num_bytes); 
	}

	int ind = rand() % (len - num_bytes);
	int rand_interest ;
	printf("bf:%d dest: %x\n",num_bytes, *(dest+ind));
	if(num_bytes == 4){
		rand_interest = interesting_32[rand() % (sizeof(interesting_32) / 32)];
		*(uint32_t *) (dest + ind) = rand_interest;

	} else if(num_bytes == 2){

		rand_interest = interesting_16[rand() % (sizeof(interesting_16) / 16)];
		*(uint16_t *) (dest + ind) = rand_interest;

	} else{

		rand_interest = interesting_8[rand() % (sizeof(interesting_8) / 8)];
		*(dest + ind) = rand_interest;

	}
	printf("af:%d dest: %d\n",num_bytes, *(dest+ind));
	return len;
}

int
run_mut(char * src, int len, char *dest)
{
    int(* fp[])(char* src, int len, char *dest) = {delete_byte, insert_byte, flip_bit, flip_byte, simp_arith, swap_known_ints};

	int mut_ind = rand() %6;

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
	printf("str: %s\n", seed);

	swap_known_ints(seed, len, dest);

	printf("str: %s\n", dest);

	run_mut(seed, len, dest);

	printf("str: %s\n", dest);

	create_mut_str(10, seed, len, dest);

	printf("dest: %s\n", dest);

	return 0;
}

#endif /* DEBUG */