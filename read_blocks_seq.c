#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "write_blocks_seq.h"
#include <sys/timeb.h>

/*
  calculate how many users in total
*/
int read_users(char * file_name){
	FILE *fp_read;
	int count_num = 0;
	char current_line[MAX_CHARS_PER_LINE];
	if (!(fp_read= fopen ( file_name , "r" ))) {
	 	printf ("Could not open file \"%s\" for reading \n", file_name);
	 	return (-1);
    }
    while( fgets (current_line, MAX_CHARS_PER_LINE, fp_read)!=NULL ) {
    	count_num ++ ;
    }
    fclose (fp_read);
    return count_num;
}


/*
  implement a query that reads the binary record file in blocks
*/
int read_blocks_seq(char * file_name, int blocksize){

	int block_size = blocksize;
	int records_per_block = block_size / sizeof(Record);
	FILE *fp_read;
	int most_follow_id;
	int max_num = 0;
	int current_id;
	int current_num;
	int total_following = 0;

	struct timeb t_begin, t_end;
    long time_spent_ms;
    long total_records = 0; 

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
	ftime(&t_begin); 
	while (fread (buffer, sizeof(Record), records_per_block, fp_read) > 0){
		/*how many records left to read after reading one block*/
		int left_records = file_record_size - records_per_block;
		int length;
		/*left data is greater than or equal to one block size*/
		if(left_records >= 0){
			file_record_size = file_record_size - records_per_block;
			length = records_per_block;
		}
		/*left data is smaller than one block size*/
		else{
			length = file_record_size;
		}
		/*compute the query*/
		for(int i=0; i < length; i++){

			total_records ++;

			total_following += 1;

			if(buffer[i].uid1 == current_id){
				current_num += 1;
				current_id = buffer[i].uid1;
				if(current_num > max_num){
					most_follow_id = current_id;
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
		}
		printf("read one block\n");

	}

	fclose (fp_read);
	free (buffer);

	ftime(&t_end); 
	/* time elapsed in milliseconds */
	time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
       + (t_end.millitm - t_begin.millitm)); 
    /* result in MB per second */
    printf ("Data rate: %.3f BPS\n", ((total_records*sizeof(Record))/(float)time_spent_ms * 1000));


    int total_user = read_users("nodes.csv");
    int average = total_following / total_user;
	printf("The userid has most follows is %d with Maximum number is %d\n", most_follow_id, max_num);
	printf("Avergae number is %d\n", average );
	return 0;
}




/*
  read the entire binary file into a large in-memory buffer, 
  and perform the same query
 */
void read_ram_seq(char *filename){
	FILE *fp_read;
	int most_follow_id;
	int max_num = 0;
	int current_id;
	int current_num;
	int total_following = 0;

	struct timeb t_begin, t_end;
    long time_spent_ms;
    long total_records = 0; 

    /* Open an existing binary file for reading a record at a time. */
    if ((fp_read = fopen (filename, "rb, type=record" ) ) == NULL )
    {
        printf ( "Cannot open file\n" );
        exit (1);
    }
    /* determine the file size */
    fseek(fp_read, 0, SEEK_END);
    int file_size = ftell(fp_read);
    fseek(fp_read, 0, SEEK_SET); 
    int file_record_size = file_size / sizeof(Record);

    // proceed with allocating memory and reading the file
    Record* buffer = (Record *) calloc (file_record_size, sizeof(Record)) ;
    printf("Total allocated records size is %d\n", file_record_size);
    /* Read records from the file to the buffer.                 */
    total_records =  file_record_size;
    fread (buffer, sizeof(Record), file_record_size, fp_read);
    ftime(&t_end); 
    /*compute the query*/
    for(int i=0; i < file_record_size; i++){

    	total_following += 1;

    	if(buffer[i].uid1 == current_id){
			current_num += 1;
			current_id = buffer[i].uid1;
			if(current_num > max_num){
				most_follow_id = current_id;
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
	}

	free (buffer);
    fclose (fp_read);

    /* time elapsed in milliseconds */
	time_spent_ms = (long) (1000 *(t_end.time - t_begin.time)
       + (t_end.millitm - t_begin.millitm)); 
    /* result in MB per second */
    printf ("Data rate: %.3f BPS\n", ((total_records*sizeof(Record))/(float)time_spent_ms * 1000));


    int total_user = read_users("nodes.csv");
    int average = total_following / total_user;
	printf("The userid has most follows is %d with Maximum number is %d\n", most_follow_id, max_num);
	printf("Avergae number is %d\n", average );
}


int main(int argc, char **argv) {
	 char *filename = argv[1];
     /* int blockSize = atoi(argv[2]); */
     /*read_blocks_seq(filename, blockSize); */
     read_ram_seq(filename);
	}








