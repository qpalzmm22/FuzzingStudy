CC=gcc

CFILES=src/fuzzer.c
CFILES+=src/fuzz_input_maker.c
CFILES+=src/gcov_creater.c
CFILES+=src/coverage_calculator.c
CFILES+=src/utility.c
CFILES+=src/power_schedule.c

OUTBIN=fuzzer

all : fuzz_cjson_mut fuzz_cjson_rand
module_test : test_fuzz_input_maker test_gcov_creater test_coverage_calculator test_power_schedule

# MODULE TESTER BUILDS
# Enable Debug in the source code to test each
test_fuzz_input_maker : src/fuzz_input_maker.c
	$(CC) -o bin/fuzz_input_maker src/fuzz_input_maker.c

test_gcov_creater : src/gcov_creater.c
	$(CC) -o bin/gcov_creater src/gcov_creater.c

test_coverage_calculator : src/coverage_calculator.c src/gcov_creater.c
	$(CC) -o bin/coverage_calculator src/coverage_calculator.c src/gcov_creater.c

test_power_schedule : src/power_schedule.c src/utility.c
	$(CC) -o bin/power_schedule src/power_schedule.c src/utility.c

# MODULE TESTER BUILDS END

#fuzz_url_parser : $(CFILES)
#	$(CC) -o bin/fuzz_url_parser $(CFILES)

#url_parser_tester : $(CFILES)
#	$(CC) -o bin/url_parser_tester $(CFILES)
	
fuzz_cjson_mut : $(CFILES) test/cjson_fuzzer_mut_driver.c 
	$(CC) -o bin/fuzz_cjson_mut $(CFILES) test/cjson_fuzzer_mut_driver.c
	
fuzz_cjson_rand : $(CFILES) test/cjson_fuzzer_rand_driver.c 
	$(CC) -o bin/fuzz_cjson_rand $(CFILES) test/cjson_fuzzer_rand_driver.c

clean : 
	rm -r bin/* tmp* ./*.c a.out 
