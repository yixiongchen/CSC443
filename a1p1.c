#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "a1p1.h"


Record parseToRecord(char* str) {

   Record r;
   
   r.uid1 = atoi(strtok(str, ","));
   r.uid2 = atoi(strtok(NULL, ","));

   return r; 
}


int main()
{
   /*
   FILE *fp;
   char str[60];

   fp = fopen("edges.csv" , "r");
   if(fp == NULL) 
   {
      perror("Error opening file");
      return(-1);
   }
   while ( fgets (str, 60, fp)!=NULL ) 
   {
      char *uid = strtok(str, ",");
      while( uid != NULL ) 
      {
	  
	  uid = strtok(NULL, ",");
      }
   }
   fclose(fp);*/
   
   char str[] = "1,2";
   Record r = parseToRecord(str);
   printf("%d, %d\n", r.uid1, r.uid2);
   
   return(0);
}

