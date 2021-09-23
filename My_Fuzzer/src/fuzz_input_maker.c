#include "../include/fuzz_input_maker.h"
/*

	dest str must be initialize before function call.
	Byte-dividable operation and type casting operations are treated seperated for implemnetation easiness

*/

// #define DEBUG

static int8_t interesting_8[] = { KNOWNINT_8 };
static int16_t interesting_16[] = { KNOWNINT_8, KNOWNINT_16 };
static int32_t interesting_32[] = { KNOWNINT_8, KNOWNINT_16, KNOWNINT_32 };


// returns 'len + 1' length of string with '\0' at the end 
int 
create_rand_str(in_config_t in_config, char * dest)
{
#ifdef DEBUG
	printf("================= RSG =================\n");
#endif // DEBUG
    int min_len = in_config.min_len ;
    int max_len = in_config.max_len ;
    int char_start = in_config.ch_start ;
    int char_range = in_config.ch_range ;
    
	int len = rand() % (max_len - min_len + 1) + min_len ; 

	for(int i = 0; i < len; i++){
		dest[i] = char_start + rand() % (char_range + 1) ;
	}
	dest[len] = '\0' ;

	return len ;
}

int
delete_byte(char *src, int len, char *dest, int num_bytes)
{
#ifdef DEBUG
	printf("================= Delete Bytes =================\n");
#endif // DEBUG
	if(len == 0){
		memcpy(dest, src, len+1);
		return 0;
	}

	char tmp_src[len + 1];
	memcpy(tmp_src, src, len + 1);
	int ind = rand() % (len - num_bytes + 1) ;


	memcpy(dest, tmp_src, ind);
	memcpy(dest + ind , tmp_src + ind + num_bytes, len - num_bytes - ind + 1);
#ifdef DEBUG
	printf("ind : %d\n", ind);
#endif // DEBUG	
	return len - num_bytes;
}

void
print_str(char *str, int len){
	printf("DEBUG] ");
	for(int i = 0; i < len; i++){
		printf("%x ", str[i]);
	}
	printf("\n");
}


int
insert_byte(char *src, int len, char *dest, int num_bytes)
{
#ifdef DEBUG
	printf("================= Insert Bytes =================\n");
#endif // DEBUG	

	char tmp_src[len + 1];
	memcpy(tmp_src, src, len + 1);
	int ind = rand() % (len + 1);

	memcpy(dest, tmp_src, ind);

	int input_mode = rand() % 3 ;

	// Know value su
	if(input_mode == 2){
		switch (num_bytes) {
			case 4 :
				*(uint32_t *) (dest + ind) = interesting_32[rand() % (sizeof(interesting_32) / 32)];
				break;
			case 2 :
				*(uint16_t *) (dest + ind) = interesting_16[rand() % (sizeof(interesting_16) / 16)];
				break;
			default :
				*(dest + ind) = interesting_8[rand() % (sizeof(interesting_8) / 8)];;
		}
	} else {
		for(int i = 0 ; i < num_bytes; i++){
			switch(input_mode){	
				// random
				case 0 :
					dest[ind + i] = rand() % (127-32) + 32;
					break;

				// Other offset 
				case 1 :
					if(len != 0)
						dest[ind + i] = tmp_src[rand() % (len + 1)];
					break;

				default :	
					fprintf(stderr, "Error in selecting input_mode");
					break; 
			}
		}
	}

	memcpy(dest + ind + num_bytes, tmp_src + ind, len - ind + 1);

#ifdef DEBUG
	printf("ind : %d, dest[ind] : %d, mode : %d\n", ind, dest[ind], input_mode);
#endif // DEBUG
	return len + num_bytes ;
}

//TODO : Change to unsigned integer
int
change_byte(char *src, int len, char *dest, int num_bytes)
{
#ifdef DEBUG
	printf("================= Change Byte =================\n");
#endif // DEBUG
	if(len == 0){
		memcpy(dest, src, len+1);
		return len;
	}
	char tmp_src[len + 1]; 
	memcpy(tmp_src, src, len + 1);

	int ind = rand() % (len - num_bytes + 1) ;
	memcpy(dest, tmp_src, ind);

	int change_mode = rand() % 3 ;
	if(change_mode == 2){
		switch (num_bytes) {
			case 4 :
				*(uint32_t *) (dest + ind) = interesting_32[rand() % (sizeof(interesting_32) / 32)];
				break;
			case 2 :
				*(uint16_t *) (dest + ind) = interesting_16[rand() % (sizeof(interesting_16) / 16)];
				break;
			default :
				*(dest + ind) = interesting_8[rand() % (sizeof(interesting_8) / 8)];
		}
	} else {
		for(int i = 0; i < num_bytes; i++){
			// bit flip
			switch(change_mode){
				// Random Value
				case 0 :
					dest[ind + i] = rand() % (127-32) + 32;
					break;
				
				// Other Offset
				case 1 :
					dest[ind + i] = tmp_src[rand() % (len + 1)];
					break;

				// Flip
				case 3 :;
					unsigned int bit = 0xFF; // b'11111111	
					dest[ind + i] = tmp_src[ind + i] ^ bit;
					break;
				
				default :
					fprintf(stderr, "Error in selecting change_mode");
					break; 
			}
		}
	}
	memcpy(dest + ind + num_bytes, tmp_src + ind + num_bytes, len - ind - num_bytes + 1);

#ifdef DEBUG
	printf("ind : %d, dest[ind] : %d, mode : %d\n", ind, dest[ind], change_mode);
#endif // DEBUG

	return len;
}

// Uses tmp_src to be prepared when src and dest are same. 
int
flip_bit(char *src, int len, char *dest, int num_bytes)
{
#ifdef DEBUG
	printf("================= Flip Bit =================\n");
#endif // DEBUG
	if(len == 0){
		memcpy(dest, src, len+1);
		return len;
	}
	char tmp_src[len + 1];
	memcpy(tmp_src, src, len + 1);

	int ind = rand() % len ;

	int lg_num_bit = rand() % 3 ;
	int num_bit = 1 << lg_num_bit ;

	int bit = rand() % (8 - num_bit + 1) ; // 0 ~ 7
	unsigned int xor_bit = 0;
	unsigned int ind_bit;
	for(int i = 0 ; i < num_bit; i++){
		ind_bit = 1 << (bit + i); 
		xor_bit = xor_bit | ind_bit;
	}  

	memcpy(dest, tmp_src, ind);
	dest[ind] = tmp_src[ind] ^ xor_bit;

	memcpy(dest + ind + 1, tmp_src + ind + 1, len - ind);

#ifdef DEBUG
	printf("ind : %d\n", ind);
#endif // DEBUG

	return len ;
}

int
run_mut_wrapper(char * src, int len, char *dest){
	int lg_num_bytes, num_bytes;
	
	if(len >= 4){
		lg_num_bytes = rand() % 3;
	} else if(len >= 2){
		lg_num_bytes = rand() % 2;
	} else {
		lg_num_bytes = 0;
	}
	num_bytes = (1 << lg_num_bytes);

	return run_mut(src, len, dest, num_bytes);
}

int
run_mut(char * src, int len, char *dest, int num_bytes)
{

    int (* fp[])(char* src, int len, char *dest, int num_bytes) 
		= { delete_byte, insert_byte, change_byte};//flip_bit, simp_arith ,swap_known_ints, flip_byte };

	int mut_ind = rand() % 3  ;

#ifdef DEBUG
	print_str(src, len);
	printf("> src : %s, len : %d, bytes: %d\n", src, len, num_bytes);
#endif // DEBUG	

	return (*fp[mut_ind])(src, len, dest, num_bytes);
}

int
create_mut_str(int max_mut, char *src, int len, char *dest)
{
	int itr = rand() % max_mut + 1 ;
	memcpy(dest, src, len + 1);
	
	int lg_num_bytes, num_bytes;
	
	for(int i = 0; i < itr; i++){
		
		if(len >= 4){
			lg_num_bytes = rand() % 3;
		} else if(len >= 2){
			lg_num_bytes = rand() % 2;
		} else {
			lg_num_bytes = 0;
		}
		num_bytes = (1 << lg_num_bytes);

		len = run_mut(dest, len, dest, num_bytes);

#ifdef DEBUG
	print_str(dest, len);
	printf("> dest : %s\n\n", dest);
#endif // DEBUG
	
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

	//test by seeing result
	// for(int i = 0 ; i < 99990 ; i++){
	// 	run_mut_wrapper(seed, len, dest);
	// 	printf("> dest : %s\n\n", dest);
	// }

	// test by rrecursively running
	create_mut_str(1100, seed, len, dest);

	printf("dest: %s\n", dest);

	return 0;
}

#endif /* DEBUG */