#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/timeb.h>
#include "a1p1.h"

int main(int argc, char **argv) {
    
    char *file_name = argv[1];
    int block_size = atoi(argv[2]);
    int X = atoi(argv[3]);
    int records_per_block = block_size / sizeof(Record);
    int overall_max_num = 0;
    int overall_max_id;
    int total_unique_ids = 0;
    float overall_avg;
    long total_read_records = 0;
    
    FILE *fp_read;
    time_t t;
    struct timeb t_begin, t_end;
    
    if (!(fp_read = fopen ( file_name , "rb" ))){
	printf ("Could not open file \"%s\" for reading \n", file_name);
	return(-1);
    }
    
    // Get total number of records
    fseek(fp_read, 0, SEEK_END);
    long file_size = ftell(fp_read);
    fseek(fp_read, 0, SEEK_SET); 
    long total_records = file_size / sizeof(Record);
    
    /* Intializes random number generator */
    srand((unsigned) time(&t));

    Record* buffer = (Record *) calloc (records_per_block, sizeof(Record));

    ftime(&t_begin);
    
    int i;
    for (i = 0; i < X; i++){
	// Generate a random number in range [0, total_records)
	long record = rand() % total_records;
	
	long offset = record * sizeof(Record);
	fseek(fp_read, offset, SEEK_SET);
	long read_size;
	long size_left = file_size - offset;
	if (size_left > block_size){
	    read_size = block_size;
	}
	else{
	    read_size = size_left;
	}
	
	long records_to_read = read_size/sizeof(Record);
	long result = fread (buffer, sizeof(Record), records_to_read, fp_read);
	
	if (result == records_to_read){
	    
	    total_read_records += records_to_read;
	    
	    int sample_max_id = 0;
	    int sample_max_num = 0;
	    int current_id = 0;
	    int current_num = 0;
	    int unique_uids = 0;
	    
	    int j;
	    for (j = 0; j < records_to_read; j++){
		if (buffer[j].uid1 == current_id){
		    current_num += 1;
		}
		else{
		    unique_uids++;
		    if (current_num > sample_max_num){
			sample_max_id = current_id;
			sample_max_num = current_num;
		    }
		    /*initialization */
		    current_num = 1;
		    current_id = buffer[j].uid1;
		}	
	    }
	    
	    if (current_num > sample_max_num){
		sample_max_id = current_id;
		sample_max_num = current_num;
	    }
	    
	    if (sample_max_num > overall_max_num){
		overall_max_num = sample_max_num;
		overall_max_id = sample_max_id;
	    }
	    
	    total_unique_ids += unique_uids;
	    //printf("start_record = %d, sample_max_id = %d, sample_max_num = %d, unique_ids = %d\n", record+1, sample_max_id, sample_max_num, unique_uids);
	}
	else{
	    printf("FREAD ERROR\n");
	    return(-1);
	}
    }
    
    ftime(&t_end);
    
    free(buffer);
    fclose(fp_read); 
    
    /* time elapsed in milliseconds */
    long time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
	+ (t_end.millitm - t_begin.millitm)); 
 
    long MB = 1024 * 1024;
    /* result in MB per second */
    printf ("block size: %d bytes, rate: %.3f MBPS\n", block_size, ((total_read_records*sizeof(Record))/(float)time_spent_ms * 1000)/MB);
    
    overall_avg = (float) total_read_records/total_unique_ids;
    
    printf("Overall average = %f, max id = %d, max num = %d\n", overall_avg, overall_max_id, overall_max_num);
    
    return 0;
}