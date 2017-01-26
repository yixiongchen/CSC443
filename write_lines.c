#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "write_blocks_seq.h"


int main(int argc, char **argv) {
  
    char *file_name = argv[1];
    
    char current_line[MAX_CHARS_PER_LINE];
    FILE *fp_read, *fp_write;

    /* open text file for reading */
    if (!(fp_read= fopen ( file_name , "r" ))) {
	printf ("Could not open file \"%s\" for reading \n", file_name);
	return (-1);
    }
    
    char fp_write_name[strlen(file_name)+5];
    strcpy(fp_write_name, "copy_");
    /* open another csv file for writing */
    if (!(fp_write = fopen ( strcat(fp_write_name, file_name) , "wb" ))) {
	printf ("Could not open file \"copy_%s\" for reading \n", file_name);
	return (-1);
    }
    
    /* reading lines */
    while( fgets (current_line, MAX_CHARS_PER_LINE, fp_read)!=NULL ) {
	fprintf(fp_write, "%s", current_line);
    }
    
    fclose(fp_read);  
    fclose(fp_write);

    return(0);
}

