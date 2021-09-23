#ifndef _HAVE_FUZZ_INPUT_MAKER_H_
#define _HAVE_FUZZ_INPUT_MAKER_H_ 

#include "types.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <time.h>



#define ARITH_MAX 35

#define KNOWNINT_8 \
  -128,          /* Overflow signed 8-bit when decremented  */ \
  -1,            /*                                         */ \
   0,            /*                                         */ \
   1,            /*                                         */ \
   16,           /* One-off with common buffer size         */ \
   32,           /* One-off with common buffer size         */ \
   64,           /* One-off with common buffer size         */ \
   100,          /* One-off with common buffer size         */ \
   127           /* Overflow signed 8-bit when incremented  */

#define KNOWNINT_16 \
  -32768,        /* Overflow signed 16-bit when decremented */ \
  -129,          /* Overflow signed 8-bit                   */ \
   128,          /* Overflow signed 8-bit                   */ \
   255,          /* Overflow unsig 8-bit when incremented   */ \
   256,          /* Overflow unsig 8-bit                    */ \
   512,          /* One-off with common buffer size         */ \
   1000,         /* One-off with common buffer size         */ \
   1024,         /* One-off with common buffer size         */ \
   4096,         /* One-off with common buffer size         */ \
   32767         /* Overflow signed 16-bit when incremented */

#define KNOWNINT_32 \
  -2147483648LL, /* Overflow signed 32-bit when decremented */ \
  -100663046,    /* Large negative number (endian-agnostic) */ \
  -32769,        /* Overflow signed 16-bit                  */ \
   32768,        /* Overflow signed 16-bit                  */ \
   65535,        /* Overflow unsig 16-bit when incremented  */ \
   65536,        /* Overflow unsig 16 bit                   */ \
   100663045,    /* Large positive number (endian-agnostic) */ \
   2147483647    /* Overflow signed 32-bit when incremented */


int
create_rand_str(in_config_t in_configs, char * dest);

int
delete_byte(char *src, int len, char *dest, int bytes);

int
insert_byte(char *src, int len, char *dest, int bytes);

int
flip_bit(char *src, int len, char *dest, int bytes);

int
run_mut(char * src, int len, char *dest, int bytes);

int
simp_arith(char * src, int len, char *dest, int bytes);

int
create_mut_str(int itr, char *src, int len, char *dest);

#endif /* !_HAVE_FUZZ_INPUT_MAKER_H_ */