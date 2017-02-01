#include <stdio.h>
#include <stdlib.h>

/*reads one line from a file*/
char * readLine(FILE *input){
   int c = getc(input);
   int count = 0;
   int scale = 1;
   char *lineptr;
   if(c == EOF){
      return NULL;
   }
   lineptr = (char *) malloc(10*sizeof(char));
   if(lineptr==NULL){
      printf("error allocating memory :(");
      exit(1);
   }
           
   while((c!= EOF) && (c != '\n') ){
      if(count+1 == 10*scale){
         scale++;
         lineptr = (char *) realloc(lineptr,sizeof(char)*(10*scale));
         if(lineptr==NULL){
     	    printf("error allocating memory :(");
            exit(1);
         }
      }
      lineptr[count] = c;
      count++;
      c = getc(input);     
   }
   lineptr[count] = '\0';
   return lineptr;
}


