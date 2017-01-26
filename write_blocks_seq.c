#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "write_blocks_seq.h"


Record parseToRecord(char* str) {

    Record r;
    
    r.uid1 = atoi(strtok(str, ","));
    r.uid2 = atoi(strtok(NULL, ","));

    return r; 
}


int main(int argc, char **argv) {
  
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
    
    int total_records = 0;
    printf("records_per_block = %d\n", records_per_block);
    
    /* reading lines */
    while( fgets (current_line, MAX_CHARS_PER_LINE, fp_read)!=NULL ) {
	current_line [strcspn (current_line, "\r\n")] = '\0'; //remove end-of-line characters
	Record r = parseToRecord(current_line);
	if (total_records >= records_per_block){
	    fwrite ( buffer, sizeof(Record), total_records, fp_write);
	    fflush (fp_write);
	    memset(buffer, 0, block_size);
	    total_records = 0;
	}
	memcpy(buffer, &r, sizeof(Record));
	total_records++;
    }

    printf("total_records: %d\n", total_records);
    
    if (total_records > 0){
	fwrite ( buffer, sizeof(Record), total_records, fp_write);
    }
    
    fflush(fp_write);
    free(buffer);
    fclose(fp_read);  
    fclose(fp_write);

    return(0);
}

