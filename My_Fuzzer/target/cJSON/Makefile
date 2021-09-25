all: cJSON_tester

cJSON_tester : cJSON.c cJSON_tester.c cJSON.h
	gcc -o cJSON_tester --coverage cJSON.c cJSON_tester.c 

clean : 
	rm -rf cJSON_tester
