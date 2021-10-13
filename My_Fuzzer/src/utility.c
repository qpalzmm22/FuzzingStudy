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


