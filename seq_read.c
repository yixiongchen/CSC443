#include <stdio.h>
#include "a1.h"



void read_blocks_seq(char * filename, int size){

	if(size % sizeof(struct record) != 0){
		return 0;
	}
	FILE *fp;
    char buf[size];
    /* Open an existing binary file for reading a record at a time. */
    if ((fp=fopen (filename, "rb, type=record")) == NULL)
    {
        printf ("Cannot open file\n");
        exit (1);
    }
    /* Read records from the file to the buffer. */
              
    while (fread (buf, size, 1, fp) > 0){

    	for(int i=0; i< (size / sizeof(struct record)); i++){

    		printf ("%d\n", buf[i*sizeof(struct record)]);

    	}
    }


    fclose (fp);
    return 0;
}

void read_ram_seq(char *filename){
	FILE *fp;
	fseek(f, 0, SEEK_END); // seek to end of file
    int size = ftell(f); // get current file pointer
    fseek(f, 0, SEEK_SET); // seek back to beginning of file
    // proceed with allocating memory and reading the file
    char buf[size];
    /* Open an existing binary file for reading a record at a time. */
    if (( fp = fopen (filename, "rb, type=record" ) ) == NULL )
    {
        printf ( "Cannot open file\n" );
        exit (1);
    }
    /* Read records from the file to the buffer.                 */
    fread (buf, sizeof(buf), 1, fp);
    printf ( "%6s\n", buf );
    fclose ( fp );
    return 0;
}