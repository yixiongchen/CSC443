#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "write_blocks_seq.h"
#include <sys/timeb.h>


/*
  implement a query that reads the binary record file in blocks
*/
void read_blocks_seq(char * file_name, int blocksize){
    int block_size = blocksize;
    int records_per_block = block_size / sizeof(Record);
    FILE *fp_read;
    int most_follow_id;
    int max_num = 0;
    int current_id = NULL;
    int current_num = 1;
    int unique_uids = 0;

    struct timeb t_begin, t_end;
    long time_spent_ms;

    /* allocate buffer for 1 block */
    Record* buffer = (Record *) calloc (records_per_block, sizeof(Record)) ;
    if (!(fp_read = fopen ( file_name , "rb" ))){
	exit(1);
    }
    
    /*first get the size of opening file*/
    fseek(fp_read, 0, SEEK_END);
    int file_size = ftell(fp_read);
    fseek(fp_read, 0, SEEK_SET); 
    int total_following = file_size / sizeof(Record);
    int unread_records = total_following;
    printf("Number of unread records is %d.\n", unread_records);
   
    /* read records into buffer */
    ftime(&t_begin); 
    while (fread (buffer, sizeof(Record), records_per_block, fp_read) > 0){
	/*how many records left to read after reading one block*/
	int left_records = unread_records - records_per_block;
	int length;
	/*left data is greater than or equal to one block size*/
	if(left_records >= 0){
	    unread_records = unread_records - records_per_block;
	    length = records_per_block;
	}
	/*left data is smaller than one block size*/
	else{
	    length = unread_records;
	}
	/*compute the query*/
	int i;
	for (i = 0; i < length; i++){
	    if(buffer[i].uid1 == current_id){
		current_num += 1;
	    }
	    else{
		unique_uids++;
		if (current_num > max_num){
		    most_follow_id = current_id;
		    max_num = current_num;
		}
		/*initialization */
		current_num = 1;
		current_id = buffer[i].uid1;
	    }	
	}
    }

    fclose (fp_read);
    free (buffer);

    ftime(&t_end); 
    /* time elapsed in milliseconds */
    time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
	+ (t_end.millitm - t_begin.millitm)); 
    
    long MB = 1024 * 1024;
    /* result in MB per second */
    printf ("block size: %d bytes, rate: %.3f MBPS\n", block_size, ((total_following*sizeof(Record))/(float)time_spent_ms * 1000)/MB);

    float average = (float) total_following / unique_uids;
    printf("The userid has most follows is %d with Maximum number is %d\n", most_follow_id, max_num);
    printf("Avergae number is %f (total = %d, unique ids = %d)\n", average, total_following, unique_uids);
}


int main(int argc, char **argv) {
    char *filename = argv[1];
    int blockSize = atoi(argv[2]); 
    read_blocks_seq(filename, blockSize);
    return 0;
}