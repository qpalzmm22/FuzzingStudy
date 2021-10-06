all: cJSON_test_case mj sy

cJSON_tester : cJSON.c cJSON_tester.c cJSON.h
	gcc -o cJSON_tester --coverage cJSON.c cJSON_tester.c 

cJSON_read_tester : cJSON.c cjson_read_fuzzer.c cJSON.h
	gcc -o cjson_read_fuzzer --coverage cJSON.c cjson_read_fuzzer.c 

cJSON_test_case : cJSON.c cjson_test_case.c cJSON.h
	gcc -o cjson_test_case --coverage cJSON.c cjson_test_case.c 

mj :
	gcc -o mj --coverage cJSON.c mj.c

sy :
	gcc -o sy --coverage cJSON.c sy.c

clean : 
	rm -rf cJSON_tester cjson_read_fuzzer *.gc*
 
