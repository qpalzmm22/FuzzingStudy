g_json:
	gcc -o cjson_fuzzer -g cjson_test.c ../greybox_mutation_fuzzer/src/create_inp.c ../greybox_mutation_fuzzer/src/coverage.c ../greybox_mutation_fuzzer/src/fuzzer.c ../greybox_mutation_fuzzer/src/config.c ../greybox_mutation_fuzzer/src/sched.c

r_json:
	gcc -o cjson_fuzzer cjson_test.c ../random_input_fuzzer/src/create_inp.c ../random_input_fuzzer/src/coverage.c ../random_input_fuzzer/src/fuzzer.c ../random_input_fuzzer/src/config.c 

clean:
	rm -rf cjson_fuzzer test testcase crashme_fuzzer *.gcno *.gcda *.gcov tmp.*