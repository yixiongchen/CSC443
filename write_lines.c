#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/timeb.h>

#include "a1p1.h"


int main(int argc, char **argv) {
    
    struct timeb t_begin, t_end;
    long time_spent_ms = 0;
  
    char *file_name = argv[1];
    
    char current_line[MAX_CHARS_PER_LINE];
    FILE *fp_read, *fp_write;

    /* open text file for reading */
    if (!(fp_read= fopen ( file_name , "r" ))) {
	printf ("Could not open file \"%s\" for reading \n", file_name);
	return (-1);
    }
    
    /*first get the size of opening file*/
    fseek(fp_read, 0, SEEK_END);
    int file_size = ftell(fp_read);
    fseek(fp_read, 0, SEEK_SET); 
    
    char fp_write_name[strlen(file_name)+5];
    strcpy(fp_write_name, "copy_");
    /* open another csv file for writing */
    if (!(fp_write = fopen ( strcat(fp_write_name, file_name) , "wb" ))) {
	printf ("Could not open file \"copy_%s\" for reading \n", file_name);
	return (-1);
    }
    
    
    
    /* reading lines */
    while( fgets (current_line, MAX_CHARS_PER_LINE, fp_read)!=NULL ) {
	ftime(&t_begin);
	fputs(current_line, fp_write);
	fflush(fp_write);
	ftime(&t_end);
	time_spent_ms += (long) (1000 *(t_end.time - t_begin.time)
	    + (t_end.millitm - t_begin.millitm)); 
    }
      
    fclose(fp_write);
    
    /* time elapsed in milliseconds */
    
    /* result in MB per second */
    int MB = 1024 * 1024;
    printf ("rate: %.3f MBPS\n", ((file_size)/(float)time_spent_ms * 1000)/MB);
    
    fclose(fp_read);  

    return(0);
}

