#ifndef MD5HASH_CPP
#define MD5HASH_CPP

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <openssl/md5.h>

#include "MD5Hash.hpp"

unsigned char result[MD5_DIGEST_LENGTH];
char polishedResult[2*MD5_DIGEST_LENGTH];

// Print the MD5 sum as hex-digits.
void print_md5_sum() {
    int i;
    for(i=0; i <MD5_DIGEST_LENGTH; i++) {
			char temp[2];
            sprintf(temp,"%02x",result[i]);
			polishedResult[2*i]=temp[0];
			polishedResult[2*i+1]=temp[1];
    }
	polishedResult[2*i]='\0';
}


// Get the size of the file by its file descriptor
unsigned long get_size_by_fd(int fd) {
    struct stat statbuf;
    if(fstat(fd, &statbuf) < 0) exit(-1);
    return statbuf.st_size;
}

char* getHashForFile( char *filename ) 
{
    int file_descript;
    unsigned long file_size;
    void* file_buffer;

    file_descript = open(filename, O_RDONLY);
    if(file_descript < 0) exit(-1);

    file_size = get_size_by_fd(file_descript);

    file_buffer = mmap(0, file_size, PROT_READ, MAP_SHARED, file_descript, 0);
    MD5((unsigned char*) file_buffer, file_size, result);

    print_md5_sum();

    return polishedResult;
}

#endif
