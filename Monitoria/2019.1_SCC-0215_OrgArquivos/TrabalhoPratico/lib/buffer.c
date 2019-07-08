#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <buffer.h>



unsigned long B_count(FILE *stream) {
	long n, p;

	n = ftell(stream);

	if(n < 0) {
		return -1;
	}

	p = n/PAGE_SIZE;
	if(n % PAGE_SIZE != 0) {
		p++;
	}
	
	return p;
}

unsigned long B_offset(FILE *stream) {
	long n;

	n = ftell(stream);
	if(n < 0) {
		return -1;
	}

	n = n % PAGE_SIZE;
	return n;
}


