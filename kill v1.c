#include  <stdio.h>
#include  <string.h>
#include  <ctype.h>
#include  <stdlib.h>
#include  <unistd.h>


int main() {

   char cmd[50] = "kill 543, 866, 457";
   char *split = strtok(cmd , " ");
	split = strtok(NULL , ",");
	
   int i =0;
   int pids[] = {};
   while(split != NULL){
	   int x = atoi(split);
	   pids[i] = x;
      printf("%d\n" ,pids[i]);
	   split = strtok(NULL , ",");
      i++;
   }
   size_t size =  sizeof(pids)/sizeof(pids[0]);
   printf("%d", size);

      for(int i = 0 ; i < size ; i++){
         signal(SIGTERM , pids[i]);
      }

	
   return 0;
}