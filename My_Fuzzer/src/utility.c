#include "../include/utility.h"
 

/*
    Description : 
    Simple wrapper function that writes to `stream` for `n` bytes
    and store from `ptr` buffer.
    
    Return value : 
    Actual number of bytes wrote. It must match n.
*/
int
my_fwrite(void * ptr, int n, FILE * stream){
	int sent = 0;

	while(sent < n){

		sent += fwrite(ptr + sent, 1, n - sent, stream);
	}

	return sent;
}

/*
    Description : 
    Simple wrapper function that reads from `stream` for `n` bytes 
    and store it in `ptr`

    Return value :
    Actually received number of bytes. If eof is found, it stops reading.
*/
int
my_fread(void * ptr, int n, FILE * stream)
{
	int received = 0;

	while(received < n){
        if(feof(stream) != 0) {
            break;
        }
		received += fread(ptr + received , 1, n - received, stream);
	}

	return received;
}

/*
    Description : 
    Simple wrapper function that does calloc, and then assert that it was valid.

    Return value : 
    pointer to allocated block.
*/
void *
a_calloc(size_t nsize)
{
    void * tmp = calloc(1, nsize);
    assert(tmp);
    return tmp;
}


/*
    Description : 
    Simple wrapper function that does malloc, and then assert that it was valid.

    Return value : 
    pointer to allocated block.
*/
void *
a_malloc(size_t nsize)
{
    void * tmp = malloc(nsize);
    //assert(tmp);
    return tmp;
}

/*
	Description :
	This is simple hash function, it provides fair distribtution and fewer splits.
	http://www.cse.yorku.ca/~oz/hash.html
	hash(i) = hash(i - 1) * 65599 + str[i]

	Retrun value:
	2 byte long hash short 
*/
unsigned short
sdmb_hash(char * str)
{
	unsigned short hash;
	int c;
	while(c = *str++){
		hash = c + (hash << 6) + (hash << 16) - hash; 
	}
	return hash;
}

#ifdef DEBUG
int main(){
	char str[4];
	str[0] = 1;
	str[1] = 1;
	str[2] = 1;
	str[3] = 1;

	printf("str: 0x%x\n", str);
	printf("HASH : 0x%x\n", sdmb_hash(str));
	return 0;
}

#endif /* DEBUG */
