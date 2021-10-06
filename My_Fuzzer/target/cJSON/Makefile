all: cJSON_test_case mj sy


cJSON_test_case : cJSON.c cjson_test_case.c cJSON.h
	gcc -o cjson_test_case --coverage cJSON.c cjson_test_case.c 

mj :
	gcc -o mj/mj --coverage mj/cJSON.c mj/mj.c

sy :
	gcc -o sy/sy --coverage sy/cJSON.c sy/sy.c

clean : 
	rm -rf cJSON_tester cjson_read_fuzzer /sy/sy mj/mj *.gc*
 
