#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct _airport_code{
    char code[4] ;
    char airport_name[32] ;
    struct _airport_code * next ; 
} airport_code, *p_airport_code ;

typedef struct _airport_info{
    p_airport_code airport_code ;    // arrays of airport_codes
    int size ;
} airport_info, *p_airport_info ;


p_airport_info g_airport_info ;

void add_airport(char * code, char * city){
    p_airport_code new_airport_code = (p_airport_code) malloc(sizeof(airport_code)) ;
    strcpy(new_airport_code->code,  code) ;
    strcpy(new_airport_code->airport_name, city) ;

    //g_airport_info->airport_code_list = (p_airport_code) malloc(g_airport_info->airport_code_list, g_airport_info->size * sizeof(p_airport_code));

    g_airport_info->size++ ;
}

p_airport_code get_airport(char *code)
{
    p_airport_code cur_node = g_airport_info->airport_code ;

    if(cur_node == 0x0){
        printf("No node found");
        return 0;
    } else if( strcmp(cur_node->code, code) == 0){
        return cur_node;
    }
}

void init(){
    p_airport_info g_airport_info = (p_airport_info) malloc( sizeof(airport_info)) ;
    g_airport_info->size = 0 ;
    g_airport_info->airport_code = 0x0 ;

    printf("No node found");

    add_airport("YVR", "Vancouver");

    printf("%s\n",get_airport("YVR")->airport_name); 
    // p_airport_code airport_codes = g_airport_info->airport_code_list;
    
    // airport_codes[0]->code = "YVR" ; airport_codes[0]->airport_name = "Vancouver"; 
    // airport_codes[1]->code = "JFK" ; airport_codes[1]->airport_name = "New York-JFK"; 
    // airport_codes[2]->code = "CDG" ; airport_codes[2]->airport_name = "Paris-Chales de Gaulle"
    // airport_codes[3]->code = "CAI" ; airport_codes[3]->airport_name = "Cairo"; 
    // airport_codes[4]->code = "LED" ; airport_codes[4]->airport_name = "St. Petersburg"; 
    // airport_codes[5]->code = "PEK" ; airport_codes[5]->airport_name = "Beiging"; 
    // airport_codes[6]->code = "HND" ; airport_codes[6]->airport_name = "Tokyo-Haneda"; 
    // airport_codes[7]->code = "AKL" ; airport_codes[7]->airport_name = "Auckland"; 
}



int main(){
    init();
    return 0;
}