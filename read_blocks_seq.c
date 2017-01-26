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
	int most_follow_id;
	int max_num = 0;
	int current_id;
	int current_num;

 

	/* allocate buffer for 1 block */
	Record* buffer = (Record *) calloc (records_per_block, sizeof(Record)) ;
	if (!(fp_read = fopen ( file_name , "rb" ))){
		exit(1);
	}
	/*first get the size of opening file*/
	fseek(fp_read, 0, SEEK_END);
    int file_size = ftell(fp_read);
    fseek(fp_read, 0, SEEK_SET); 
    int file_record_size = file_size / sizeof(Record);
    printf("Record size is %d\n", file_record_size);

   
	/* read records into buffer */
	while (fread (buffer, sizeof(Record), records_per_block, fp_read) > 0){
		/*how many records left to read*/
		int left_records = file_record_size - records_per_block;
		if(left_records >= 0){
			for(int i=0; i < records_per_block; i++){
				if(buffer[i].uid1 == current_id){
					current_num += 1;
					current_id = buffer[i].uid1;
					if(current_num > max_num){
						most_follow_id = current_num;
						max_num = current_num;
					}
				}
				else{
					/*initialization */
					current_num = 1;
					current_id = buffer[i].uid1;
					if(current_num > max_num){
						most_follow_id = current_id;
						max_num = current_num;
					}
				}
				file_record_size = file_record_size - records_per_block;
			}
		}

		else{
			for(int j=0; j < file_record_size; j++){
				if(buffer[j].uid1 == current_id){
					current_num += 1;
					current_id = buffer[j].uid1;
					if(current_num > max_num){
						most_follow_id = current_id;
						max_num = current_num;
					}
				}
				else{
					/*initialization */
					current_num = 1;
					current_id = buffer[j].uid1;
					if(current_num > max_num){
						most_follow_id = current_id;
						max_num = current_num;
					}
				}
			}
		}
	};

	printf("The userid has most follows is %d\n", most_follow_id);
	printf("Maximum number is %d\n", max_num);

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








