CC=gcc
GDB=-g
DEBUG=-DDEBUG

CFILES=src/fuzzer.c
CFILES+=src/fuzz_input_maker.c
CFILES+=src/gcov_creater.c
CFILES+=src/coverage_calculator.c
CFILES+=src/utility.c
CFILES+=src/power_schedule.c

COPT=-lm


OUTBIN=fuzzer

all : fuzzinger_test
module_test : test_fuzz_input_maker test_gcov_creater test_coverage_calculator test_power_schedule
fuzzinger_test : fuzz_mj fuzz_sy fuzz_ji fuzz_ws fuzz_my

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

test_utility : src/utility.c
	$(CC) -o bin/utility src/utility.c ${DEBUG}
# MODULE TESTER BUILDS END

#fuzz_url_parser : $(CFILES)
#	$(CC) -o bin/fuzz_url_parser $(CFILES)

#url_parser_tester : $(CFILES)
#	$(CC) -o bin/url_parser_tester $(CFILES)
	
fuzz_my: $(CFILES) 
	$(CC) -o bin/fuzz_my_gry_mut $(CFILES) test/cJSON/my_driver_gry_mut.c $(COPT)
	$(CC) -o bin/fuzz_my_blk_mut $(CFILES) test/cJSON/my_driver_blk_mut.c $(COPT)
	$(CC) -o bin/fuzz_my_rand  $(CFILES) test/cJSON/my_driver_rand.c $(COPT)

# FUZZINGERS PROGRAM

fuzz_mj: $(CFILES)  
	$(CC) -o bin/fuzz_mj_gry_mut $(CFILES) test/cJSON/mj_driver_gry_mut.c $(COPT)
	$(CC) -o bin/fuzz_mj_blk_mut $(CFILES) test/cJSON/mj_driver_blk_mut.c $(COPT)
	$(CC) -o bin/fuzz_mj_rand  $(CFILES) test/cJSON/mj_driver_rand.c $(COPT)

fuzz_ws: $(CFILES)  
	$(CC) -o bin/fuzz_ws_gry_mut $(CFILES) test/cJSON/ws_driver_gry_mut.c $(COPT)
	$(CC) -o bin/fuzz_ws_blk_mut $(CFILES) test/cJSON/ws_driver_blk_mut.c $(COPT)
	$(CC) -o bin/fuzz_ws_rand  $(CFILES) test/cJSON/ws_driver_rand.c $(COPT)

fuzz_ji: $(CFILES)  
	$(CC) -o bin/fuzz_ji_gry_mut $(CFILES) test/cJSON/ji_driver_gry_mut.c $(COPT)
	$(CC) -o bin/fuzz_ji_blk_mut $(CFILES) test/cJSON/ji_driver_blk_mut.c $(COPT)
	$(CC) -o bin/fuzz_ji_rand  $(CFILES) test/cJSON/ji_driver_rand.c $(COPT)

fuzz_sy: $(CFILES)
	$(CC) -o bin/fuzz_sy_gry_mut $(CFILES) test/cJSON/sy_driver_gry_mut.c $(COPT)
	$(CC) -o bin/fuzz_sy_blk_mut $(CFILES) test/cJSON/sy_driver_blk_mut.c $(COPT)
	$(CC) -o bin/fuzz_sy_rand  $(CFILES) test/cJSON/sy_driver_rand.c $(COPT)
clean : 
	rm -r bin/* tmp* ./*.c a.out 
