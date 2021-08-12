/* Thoughts on Fuzzer
 * 
 * Fuzzer should create a "string of random characters", which includes 0x0.
 * Things to config
 * time interrupt,  
 * 
 */

#include <stdio.h>



// Receives (char *) and saves the string to it. 
// dest have size of (max_length) + 1.
// 
int 
create_rand_string(char * dest, int min_length, int max_length, int char_start, int char_range)
{
	unsigned int len ;	

	len = rand() % (max_length) + 1 ; 

	for(int i = 0; i < len; i++){
		dest[i] = char_start + rand() % (char_range + 1) ;
	}
	dest[len] = '\0' ;

	return len ;
}

int
main()
{
    char dest [100];
    int len = create_rand_string(dest, 1, 1, 'a', 'z');
    return 0;
}