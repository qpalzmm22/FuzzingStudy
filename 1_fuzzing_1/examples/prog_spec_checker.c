#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

typedef struct _airport_code{
    char * code ;
    char * name ;
    struct _airport_code * next ; 
}airport_code, *p_airport_code ;

typedef struct _airport_info{
    p_airport_code head ;    // arrays of airport_codes
    int size ;
}airport_info; 

p_airport_code create_node(char * code, char * name);
p_airport_code add_airport(char * code, char * city);
p_airport_code get_airport(char *code);
int is_code_in(char *code);
void print_airport_list();
void init();
int code_repOK(char* code);
int airport_codes_repOK();

//#define ERR

airport_info g_airport_info = {
    0x0,
    0
} ;



p_airport_code create_node(char * code, char * name)
{
    p_airport_code new_airport_code = (p_airport_code) calloc(1, sizeof(airport_code)) ;
    assert(new_airport_code);

    new_airport_code->code = (char*) calloc(strlen(code), sizeof(code));
    assert(new_airport_code->code);

    new_airport_code->name = (char*) calloc(strlen(name), sizeof(name));
    assert(new_airport_code->name);

    strcpy(new_airport_code->code, code);
    strcpy(new_airport_code->name, name);

    //printf("code :%s, name: %s\n", new_airport_code->code, new_airport_code->name);
    return new_airport_code;
}


// adds node infront of the list (addFront)
p_airport_code add_airport(char * code, char * city)
{
    assert(code_repOK(code));

    p_airport_code new_airport_code = create_node(code, city);
    new_airport_code->next = g_airport_info.head;

    g_airport_info.head = new_airport_code;   
    g_airport_info.size++ ;
}


// gets airport of the code
p_airport_code get_airport(char *code)
{
    p_airport_code cur_node, itr;
    
    for(itr = g_airport_info.head; itr != 0x0; itr = itr->next){
        if(strcmp(itr->code, code) == 0)
            return itr;
    }
    return 0x0;
}

int is_code_in(char *code)
{
    if(get_airport(code) == 0x0)
        return 0;
    return 1;
}

// print list
void print_airport_list()
{
    p_airport_code cur_node, itr;
    
    printf("List is as follows... \n");
    for(itr = g_airport_info.head; itr != 0x0; itr = itr->next){
        printf(" (%s, %s) ->", itr->code, itr->name);
    }
    printf("\n");
}

int code_repOK(char* code)
{
    assert(strlen(code) == 3 && "Airport code must have three characters") ;
    for(int i = 0; i < strlen(code); i++){
        assert(isalpha(code[i]) && "Non-letter in airport code");
        assert(isupper(code[i]) && "Lowercase letter in airport code");
    }
    return 1;
}

int airport_codes_repOK()
{
    p_airport_code itr;
    
    for(itr = g_airport_info.head; itr != 0x0; itr = itr->next){
        assert(code_repOK(itr->code));
    }
    return 1;
}

void init(){
    add_airport("YVR", "Vancouver") ;
    add_airport("JFK", "New York-JFK"); 
    add_airport("CDG", "Paris-Chales de Gaulle") ;
    add_airport("CAI", "Cairo") ;
    add_airport("LED", "St. Petersburg") ;
    add_airport("PEK", "Beiging") ;
    add_airport("HND", "Tokyo-Haneda") ;
    add_airport("AKL", "Auckland") ;

    print_airport_list();
}

void test1(){
    p_airport_code YVR = get_airport("YVR");
    printf("%s\n", YVR->name);

    if(is_code_in("AKL"))
        printf("True\n");
    else
        printf("False\n");
}

void test2(){
    airport_codes_repOK();

    // Add error causing element

#ifdef ERR
    add_airport("YMML", "Melbourne");
    airport_codes_repOK();
#endif

}

int main(){

    // Initilze the linked list of codes and cities
    init();

    // airport_codes["YVR"]
    // "AKL" in airport_codes
    test1();
    
    // repOk
    // airport_repOK()
    test2();

    return 0;
}