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
    float overall_max;
    float overall_avg;
    float avg_sum = 0;
    int max_sum = 0;
    
    FILE *fp_read;
    time_t t;
    struct timeb t_begin, t_end;
    
    if (!(fp_read = fopen ( file_name , "rb" ))){
	printf ("Could not open file \"%s\" for reading \n", file_name);
	return(-1);
    }
    
    // Get total number of records
    fseek(fp_read, 0, SEEK_END);
    int file_size = ftell(fp_read);
    fseek(fp_read, 0, SEEK_SET);
    int total_blocks = file_size / block_size + 1;
    int total_records = file_size / sizeof(Record);
    
    /* Intializes random number generator */
    srand((unsigned) time(&t));

    Record* buffer = (Record *) calloc (total_records, sizeof(Record));
    
    int result = fread (buffer, sizeof(Record), total_records, fp_read);
    if (result == total_records){
	ftime(&t_begin);
	
	int i;
	for (i = 0; i < X; i++){
	    // Generate a random number in range [0, total_blocks)
	    int block = rand() % total_blocks;
	    int start= block * records_per_block;
	    int end = start + records_per_block;
	    if (end > total_records){
		end = total_records;
	    }
	    
	    int sample_max_id = 0;
	    int sample_max_num = 0;
	    float sample_avg = 0;
	    int current_id = NULL;
	    int current_num = 0;
	    int unique_uids = 0;
	    
	    int j;
	    for (j = start; j < end; j++){
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
	    
	    max_sum += sample_max_num;
	    sample_avg = (float) (end - start)/unique_uids;
	    avg_sum += sample_avg;
	    printf("block = %d, sample_max_id = %d, sample_max_num = %d, sample_avg = %f\n", block+1, sample_max_id, sample_max_num, sample_avg);
	}
    }
    else{
	printf("FREAD ERROR\n");
	return(-1);
    }
    
    ftime(&t_end);
    
    free(buffer);
    fclose(fp_read); 
    
    /* time elapsed in milliseconds */
    long time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
	+ (t_end.millitm - t_begin.millitm)); 
 
    long MB = 1024 * 1024;
    /* result in MB per second */
    printf ("block size: %d bytes, rate: %.3f MBPS\n", block_size, ((X*sizeof(Record))/(float)time_spent_ms * 1000)/MB);
    
    overall_avg = (float) avg_sum/X;
    overall_max = (float) max_sum/X;
    
    printf("Overall average = %f, max = %f\n", overall_avg, overall_max);
    
    return 0;
}