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
   FILE *fp, *records;
   char str[256];
   
   char *filename = argv[1];
   int blockSize = atoi(argv[2]);
   
   records = fopen("records.dat","wb");
   if (!records){
      perror("Error opening file to write");
      return 1;
   } 
   
   fp = fopen(filename, "r");
   if(fp == NULL)
   {
      perror("Error opening file to read");
      return(-1);
   }
   
   char buffer[blockSize];
   
   while (fgets(str, 256, fp)!=NULL ) 
   {
      Record r = parseToRecord(str);
      if (blockSize - strlen(buffer) >= sizeof(Record)){
	memcpy(buffer, &r, sizeof(Record));
      }
      else{
	fwrite(&buffer, blockSize, 1, records);
	memset(buffer, 0, blockSize);
      }
   }
   
   if (strlen(buffer) > 0){
     fwrite(&buffer, blockSize, 1, records);
   }
   
   fclose(fp);
   fclose(records);

   return(0);
}

