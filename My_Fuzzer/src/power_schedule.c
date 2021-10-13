#include "../include/power_schedule.h"

// #define DEBUG 

#define PERCENT_MODIFIER 10000
/*
    Description : 
    Search files with name starting with "seed_" and add them to seed list. 
    Here, the `seed_info->energy` contains energy of the seed list, and the `seed_info->num_seed` acts as index iterator 

    Expectation : 
    Users must make seed files with name 'seed_n' where n starts from 0.
    And there must not be a jump in number, or else fuzzer will not use that seed and overwrite the existing seed.

    Return value : 
    None
*/
void
seed_init(pSeed_info_t seed_info, char * corpus_path)
{
    char seed_path[PATH_MAX + NAME_MAX]; 

    FILE * fp;
    ssize_t len;
    char tmp[MAX_SEED_LEN];

    while(1){
        sprintf(seed_path, "%s/%s%d", corpus_path, SEED_FILE_PREFIX, seed_info->num_seed);
        
        if( access( seed_path, R_OK ) != 0 ) {
            if(seed_info->num_seed == 0){
                perror("You must put at least one seed in seed_corpus dir");
                exit(1);
            }
            break;
        }
        if((fp = fopen(seed_path, "r")) == 0x0){
            fprintf(stderr, "can't read seed_%d file : %s\n" , seed_info->num_seed, strerror(errno));
            break;
        }
        
        len = my_fread(tmp, MAX_SEED_LEN, fp);
        
        add_seed(seed_info, tmp, len, INIT_SEED_ENERGY);     
    }
    fclose(fp);
}

/*
    Description :
    Adds seed to seed list with given `energy`
    
    Return value : 
    None
*/
void
add_seed(pSeed_info_t seed_info, char * str, int len, int energy)
{
	//seed_info->seeds[seed_info->num_seed].str = (char *)a_malloc(sizeof(char) * (len + 1));
    
	memcpy(seed_info->seeds[seed_info->num_seed].str, str, len);
    seed_info->seeds[seed_info->num_seed].str[len] = '\0';

    seed_info->seeds[seed_info->num_seed].len = len;

    seed_info->seeds[seed_info->num_seed].energy = energy;

    seed_info->num_seed++;
}

/*
    Description :
    Delete seed nameed `corpus_path`/SEED_FILE_PREFIX`n where n is the iteration for newly genearted seeds 
    
    Return value : 
    None
*/
void 
delete_seed(char * corpus_path, int init_num_seed, int total_num_seed)
{
    char seed_path[PATH_MAX];
    for(int i = init_num_seed ; i < total_num_seed ; i++){
        sprintf(seed_path, "%s/%s%d", corpus_path, SEED_FILE_PREFIX, i);
        if(remove(seed_path) != 0){
            fprintf(stderr,"Error in removing [%s]: %s\n", seed_path,strerror(errno));
            exit(1);
        }
    }
}

/*
    Description : 
    Create file with name of "seed_n" where n is `seed_ind` 
    and content of `str` with length of `len`.

    Expectation : 
    
    Return value : 
    None
*/
void
make_seed_file(pSeed_info_t seed_info, char * corpus_path, char * str, int len, int seed_ind)
{
    char seed_path[PATH_MAX + NAME_MAX];
    sprintf(seed_path,"%s/%s%d", corpus_path, SEED_FILE_PREFIX, seed_ind);

    FILE * fp;

    if((fp = fopen(seed_path, "w")) == NULL){
        perror("Error in opening seed file");
        exit(1);
    }

    if(my_fwrite(str, len, fp) != len){
        fprintf(stderr, "Error in my_fwriting seed file\n");
    }

    fclose(fp);
}

/*
    Description : 
    Choose seed by distributed probabiltiy.
    O(3 * N)

    Expectation : 
    seed_info->norm_energy tp be set.

    Return value : 
    Isdandex of seeds to use in fuzzer loop.
*/
int
get_seed_ind(pSeed_info_t seed_info)
{
    double energy_sum = 0;
    double cumul_energy[MAX_SEED_FILES];
    double min_energy;
    int index; 

    if(seed_info->num_seed == 0){
        fprintf(stderr, "no seed yet");
        return -1;
    }

    min_energy = (double)seed_info->seeds[0].energy;

    // Calculate cumulative energy
    for(int i = 0; i < seed_info->num_seed; i++){
        seed_t seed = seed_info->seeds[i];
        if(seed.energy < min_energy){
            min_energy = seed.energy;
        }
        energy_sum += seed.energy;
        cumul_energy[i] = energy_sum;
    }
    double magnify_val = PERCENT_MODIFIER / energy_sum;
    double selected = rand() % (PERCENT_MODIFIER + 1); 

    for(int i = 0; i < seed_info->num_seed; i++){
        if(magnify_val * cumul_energy[i] < selected){
            index =  i ;
            return index; 
        }
    }

    return seed_info->num_seed - 1;
}

/*
    Description : 
    Search files with name of "seed_n" where n is `seed_ind` and return its content to `buf`. 

    Expectation : 
    The buf must be big enough to hold contents from seed file.

    Return value : 
    Length of the returned seed.
*/
int 
get_seed_str(pSeed_info_t seed_info, char* buf, int seed_ind)
{
    seed_t seed = seed_info->seeds[seed_ind];
    memcpy(buf, seed.str, seed.len); 

    buf[seed.len] = 0x0;

    return seed.len;
}

void
print_seed_info(pSeed_info_t seed_info)
{
    for(int i = 0; i < seed_info->num_seed; i++){
        printf("seed[%d] : %s [ %f ]\n", i, seed_info->seeds[i].str, seed_info->seeds[i].energy);
    }
}

#ifdef DEBUG

void
test()
{
    seed_info_t seed_info;
    add_seed(&seed_info, "seed1", 5, 1);
    add_seed(&seed_info, "seed2", 5, 10);
    add_seed(&seed_info, "seed3", 5, 3);
    
    print_seed_info(&seed_info);
    int a = get_seed_ind(&seed_info);
    printf("ind = %d\n", a);
}

int
main()
{
    test();
    return 0;
}


#endif /* DEBUG */
 
