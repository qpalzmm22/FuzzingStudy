#ifndef _HAVE_FUZZ_INPUT_MAKER_H_
#define _HAVE_FUZZ_INPUT_MAKER_H_ 

#include "types.h"

#include <stdlib.h>
#include <sys/types.h>

int create_rand_str(in_config_t in_configs, char * dest);

#endif /* !_HAVE_FUZZ_INPUT_MAKER_H_ */