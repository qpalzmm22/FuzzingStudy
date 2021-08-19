all : fuzzer psudo_troff test_p_troff

fuzzer : src/fuzzer.c src/fuzz_input_maker.c test/cat_tester.c 
	gcc -o bin/fuzzer src/fuzzer.c src/fuzz_input_maker.c test/cat_tester.c test/cat_oracle.c

test_p_troff: src/fuzzer.c src/fuzz_input_maker.c test/psudo_troff_tester.c
	gcc -o bin/test_p_troff src/fuzzer.c src/fuzz_input_maker.c test/psudo_troff_tester.c

fuzzer_w_coverage : src/fuzzer.c src/fuzz_input_maker.c src/gcov_creater.c test/cgi_decoder_tester.c
	gcc -o bin/fuzzer_w_coverage src/fuzzer.c src/fuzz_input_maker.c src/gcov_creater.c test/cgi_decoder_tester.c

clean : 
	rm -r bin/* tmp* a.out