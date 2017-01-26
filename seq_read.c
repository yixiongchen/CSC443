#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "write_blocks_seq.h"



int read_blocks_seq(char * file_name, int blocksize){

	int block_size = blocksize;
	int records_per_block = block_size / sizeof(Record);
	FILE *fp_read;

 

	/* allocate buffer for 1 block */
	Record* buffer = (Record *) calloc (records_per_block, sizeof(Record)) ;
	if (!(fp_read = fopen ( file_name , "rb" ))){
		exit(1);
	}
	/*first get the size of opening file*/
	fseek(fp_read, 0, SEEK_END);
    int sz = ftell(fp_read);
    fseek(fp_read, 0, SEEK_SET); 
    int file_record_size = sz / sizeof(Record);
    printf("Record size is %d\n", file_record_size);

	/* read records into buffer */
	while (fread (buffer, sizeof(Record), records_per_block, fp_read) > 0){

		int left_records = file_record_size - records_per_block;
		if(left_records >= 0)
			for(int i=0; i < records_per_block; i++){
				printf("Record %d\n", i);
				printf("%d  %d\n", buffer[i].uid1, buffer[i].uid2);
				file_record_size = file_record_size - records_per_block;
			}
		else{
			for(int j=0; j < file_record_size; j++){
				printf("Record %d\n", j);
				printf("%d  %d\n", buffer[j].uid1, buffer[j].uid2);
			}

		}
	};

	fclose (fp_read);
	free (buffer);
	return 0;
}

void read_ram_seq(char *filename){
	FILE *fp;
    // proceed with allocating memory and reading the file
    char buf[1024];
    /* Open an existing binary file for reading a record at a time. */
    if ((fp = fopen (filename, "rb, type=record" ) ) == NULL )
    {
        printf ( "Cannot open file\n" );
        exit (1);
    }
    /* Read records from the file to the buffer.                 */
    fread (buf, sizeof(buf), 1, fp);
    printf ( "%6s\n", buf );
    fclose (fp);
}



int main(int argc, char **argv) {
	 char *filename = argv[1];
     int blockSize = atoi(argv[2]);
     read_blocks_seq(filename, blockSize);
	}








