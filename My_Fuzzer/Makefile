all : fuzzer psudo_troff test_p_troff

fuzzer : src/fuzzer.c src/fuzz_input_maker.c test/cat_tester.c 
	gcc -o bin/fuzzer src/fuzzer.c src/fuzz_input_maker.c test/cat_tester.c test/cat_oracle.c

test_p_troff: src/fuzzer.c src/fuzz_input_maker.c src/gcov_creater.c src/coverage_calculator.c target/psudo_troff/psudo_troff_tester.c
	gcc -o bin/test_p_troff src/fuzzer.c src/fuzz_input_maker.c target/psudo_troff/psudo_troff_tester.c src/gcov_creater.c src/coverage_calculator.c

fuzzer_w_coverage : src/fuzzer.c src/fuzz_input_maker.c src/gcov_creater.c src/coverage_calculator.c test/cgi_decoder_tester.c  
	gcc -o bin/fuzzer_w_coverage src/fuzzer.c src/fuzz_input_maker.c src/gcov_creater.c src/coverage_calculator.c test/cgi_decoder_tester.c


# MODULE TESTER BUILDS
# Enable Debug in the source code to test each
test_fuzz_input_maker : src/fuzz_input_maker.c
	gcc -o bin/fuzz_input_maker src/fuzz_input_maker.c

test_gcov_creater : src/gcov_creater.c
	gcc -o bin/gcov_creater src/gcov_creater.c

test_coverage_calculator : src/coverage_calculator.c src/gcov_creater.c
	gcc -o bin/coverage_calculator src/coverage_calculator.c src/gcov_creater.c

# MODULE TESTER BUILDS END

fuzz_url_parser :  src/fuzzer.c src/fuzz_input_maker.c src/gcov_creater.c src/coverage_calculator.c test/url_fuzzer_tester.c
	gcc -o bin/fuzz_url_parser src/fuzzer.c src/fuzz_input_maker.c src/gcov_creater.c src/coverage_calculator.c test/url_fuzzer_tester.c

url_parser_tester : src/fuzz_input_maker.c target/URL_parser/http_program.c test/url_parser_tester.c
	gcc -o bin/url_parser_tester src/fuzz_input_maker.c target/URL_parser/http_program.c test/url_parser_tester.c



clean : 
	rm -r bin/* tmp* a.out *.gcda *.gcno *.gcov seed_corpus/seed_*