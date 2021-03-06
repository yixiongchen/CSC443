#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "a1p1.h"
#include <sys/timeb.h>
#include <time.h>


void write_ram_rand(char * file_name, int random_num){

    FILE *fp_read;
    struct timeb t_begin, t_end;
    long time_spent_ms;
    int i;

    /* Intializes random number generator */
    srand (time(NULL));;

    /* Open an existing binary file for reading a record at a time. */
    if ((fp_read = fopen (file_name, "rb, type=record" ) ) == NULL )
    {
        printf ( "Cannot open file\n" );
        exit (1);
    }
    
    /* determine the file size */
    fseek(fp_read, 0, SEEK_END);
    long file_size = ftell(fp_read);
    fseek(fp_read, 0, SEEK_SET); 
    long total_records = file_size / sizeof(Record);

    // proceed with allocating memory and reading the file
    Record* buffer = (Record *) calloc (total_records, sizeof(Record));

    // Read records from the file to the buffer.
    long result = fread (buffer, sizeof(Record), total_records, fp_read);
    ftime(&t_begin);
    if (result == total_records){
        /*update the buffer generate i positions*/
        for(i = 0; i < random_num; i++){
	    long update_position = rand() % total_records;
	    buffer[update_position].uid1 = 1;
	    buffer[update_position].uid2 = 2;
        }
    }
    else{
    	printf ("Fread Error");
    	return;
    }
    ftime(&t_end);
    
    free (buffer);
    fclose (fp_read);

    /* time elapsed in milliseconds */
    time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
       + (t_end.millitm - t_begin.millitm)); 
    
    long MB = 1024 * 1024;
    /* result in MB per second */
    printf ("Data rate: %.3f MBPS\n", ((random_num*sizeof(Record))/(float)time_spent_ms * 1000)/MB);
}


int main(int argc, char **argv){

    char *filename = argv[1];
    int random_num = atoi(argv[2]); 
    write_ram_rand(filename, random_num);
    return 0;

}



