#ifndef _HAVE_POWER_SCHEDULE_H_
#define _HAVE_POWER_SCHEDULE_H_

#include "types.h"
#include "utility.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#define SEED_FILE_PREFIX "seed_"

void seed_init(pSeed_info_t seed_info, char * seed_path);

void make_seed_file(pSeed_info_t seed_info, char * seed_path, char * str, int len, int seed_ind);
void add_seed(pSeed_info_t seed_info, char * str, int len, int energy);
void delete_seed(char * corpus_path, int init_num_seed, int total_num_seed);


int get_seed_ind(pSeed_info_t seed_info);
int get_seed_str(pSeed_info_t seed_info, char* buf, int seed_ind);

#endif /* !_HAVE_POWER_SCHEDULE_H_ */