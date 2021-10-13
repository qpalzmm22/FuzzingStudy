#ifndef _HAVE_UTILITY_H_
#define _HAVE_UTILITY_H_

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int my_fwrite(void * ptr, int n, FILE * stream);

int my_fread(void * ptr, int n, FILE * stream);

void * a_calloc(size_t nsize);

void * a_malloc(size_t nsize);

#endif /* !_HAVE_UTILITY_H_ */

