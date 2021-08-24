#ifndef _HAVE_FUZZ_INPUT_MAKER_H_
#define _HAVE_FUZZ_INPUT_MAKER_H_ 

#include "types.h"

#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

int
create_rand_str(in_config_t in_configs, char * dest);

int
delete_byte(char *src, int len, char *dest);

int
insert_byte(char *src, int len, char *dest);

int
flip_bit(char *src, int len, char *dest);

int
run_mut(char * src, int len, char *dest);

int
create_mut_str(int itr, char *src, int len, char *dest);
#endif /* !_HAVE_FUZZ_INPUT_MAKER_H_ */