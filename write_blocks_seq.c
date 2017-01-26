#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>

#include "write_blocks_seq.h"


Record parseToRecord(char* str) {

    Record r;
    
    r.uid1 = atoi(strtok(str, ","));
    r.uid2 = atoi(strtok(NULL, ","));

    return r; 
}


int main(int argc, char **argv) {
    
    struct timeb t_begin, t_end;
    long time_spent_ms;
    long total_records = 0;
  
    char *file_name = argv[1];
    int block_size = atoi(argv[2]);
    int records_per_block = block_size/sizeof(Record);
    
    char current_line[MAX_CHARS_PER_LINE];
    FILE *fp_read, *fp_write;
    Record * buffer = (Record *) calloc (records_per_block, sizeof (Record));
 

    /* open text file for reading */
    if (!(fp_read= fopen ( file_name , "r" ))) {
	printf ("Could not open file \"%s\" for reading \n", file_name);
	return (-1);
    }
    
    /* open dat file for writing */
    if (!(fp_write = fopen ( "records.dat" , "wb" ))) {
	printf ("Could not open file \"records.dat\" for reading \n");
	return (-1);
    }
    
    int records_in_buffer = 0;
    
    int j = 0;
    /* reading lines */
    ftime(&t_begin);
    while( fgets (current_line, MAX_CHARS_PER_LINE, fp_read)!=NULL ) {
	current_line [strcspn (current_line, "\r\n")] = '\0'; //remove end-of-line characters
	Record r = parseToRecord(current_line);
	if (records_in_buffer >= records_per_block){
	    fwrite ( buffer, sizeof(Record), records_in_buffer, fp_write);
	    fflush (fp_write);
	    memset(buffer, 0, block_size);
	    records_in_buffer = 0;
	}
	buffer[j] = r;
	j++;
	records_in_buffer++;
	total_records++;
    }
    
    if (records_in_buffer > 0){
	fwrite ( buffer, sizeof(Record), records_in_buffer, fp_write);
    }
    
    fflush(fp_write);
    fclose(fp_write);
    
    ftime(&t_end);
    
    /* time elapsed in milliseconds */
    time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
	+ (t_end.millitm - t_begin.millitm)); 
 
    long MB = 1024 * 1024;
    /* result in MB per second */
    printf ("Data rate: %.3f MBPS\n", ((total_records*sizeof(Record))/(float)time_spent_ms * 1000)/MB);
    
    free(buffer);
    fclose(fp_read);     

    return(0);
}

