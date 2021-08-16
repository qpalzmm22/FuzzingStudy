/* Oracle should deal with ret code and in,out,err files.
 * Then, figure out what the bug is.
 * 
 * It takes return code, file_path location as arguments. 
 * Returns 1 if it's not a bug and returns 0 if it's bug 
 * 
 */ 
#include "../include/cat_oracle.h"

int
cat_oracle(int ret_code, char* input_buff, int input_len , char* stdout_buff, char* stderr_buff)
{
    
    if(ret_code == 0){ // False positive
        for(int i = 0; i < input_len; i++){
            if(input_buff[i] != stdout_buff[i]){
                return 0;
            }
        }
    } else { // False negative
        for(int i = 0; i < input_len; i++){
            if(input_buff[i] != stdout_buff[i]){
                break;  
            }
            if(i == input_len - 1){
                return 0;
            }
        }
    }
    return 1;
}