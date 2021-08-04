#include <stdio.h>
#include <sys/wait.h>

ssize_t my_fwrite(void * ptr, size_t n, FILE * stream);
ssize_t my_fread(void * ptr, ssize_t n, FILE * stream);