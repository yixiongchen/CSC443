#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "write_blocks_seq.h"
#include <sys/timeb.h>


void write_blocks_rand(char * file_name, int random_num){
	FILE *fp_read;
	int i;


	/* Intializes random number generator */
    srand (time(NULL));

	/* Open an existing binary file for reading a record at a time. */
    if ((fp_read = fopen (file_name, "rb, type=record" ) ) == NULL )
    {
        printf ( "Cannot open file\n" );
        exit (1);
    }
    
    /* determine the file size */
    fseek(fp_read, 0, SEEK_END);
    int file_size = ftell(fp_read);
    fseek(fp_read, 0, SEEK_SET); 
    int total_records = file_size / sizeof(Record);
    
    // proceed with allocating memory and reading the file
    Record* buffer = (Record *) calloc (1, sizeof(Record));
    buffer[0].uid1 = 1;
    buffer[1].uid2 = 2;

    for(i = 0; i < random_num; i++){
    	int update_position = rand() % total_records;
    	printf("position: %d\n", update_position);
    	fseek(fp_read, update_position * sizeof(Record), SEEK_SET)
    	fputs(buffer[0],fp_read);
    	fseek(fp_read, 0, SEEK_SET);
    }
}


int main(int argc, char **argv){
	char *filename = argv[1];
    int random_num = atoi(argv[2]); 
    write_blocks_rand(filename, random_num);
    
	return (0);
}

