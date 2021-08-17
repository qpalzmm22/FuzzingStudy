all : cgi_decode cgi_decode_ex

cgi_decode : src/cgi_decode.c 
	gcc -o bin/cgi_decode src/cgi_decode.c

cgi_decode_ex : src/cgi_decode_ex.c
	gcc --coverage -o bin/cgi_decode_ex src/cgi_decode_ex.c

clean : 
	rm -r bin/* *.gcda *.gcno *.gcov