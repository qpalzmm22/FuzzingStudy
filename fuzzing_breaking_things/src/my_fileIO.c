#include "../include/my_fileIO.h"

// fwrite() safe version
ssize_t my_fwrite(void * ptr, size_t n, FILE * stream)
{
	ssize_t sent = 0;

	while(sent < n){
		sent += fwrite(ptr + sent, n - sent, 1, stream);
	}

	return sent;
}

// fread() safe version
ssize_t my_fread(void * ptr, ssize_t n, FILE * stream)
{
	ssize_t received = 0;

	while(received < n){
		received += fread(ptr + received , n - received, 1, stream);
	}

	return received;
}

// create a file and return fd.
int create_write_file(char* file_name){
	int fd;
	fd = fopen(file_name, "w");
	fclose(fd);
}