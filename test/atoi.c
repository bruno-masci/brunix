#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <brunix/string.h>


int main(int argc, char **argv) {
	char buf[512];
	assert(!strcmp(itoa(10, buf, 2), "1010"));
	return 0;
}

// gcc -o atoi-test atoi.c ../libkern/stdlib.c; ./atoi-test
