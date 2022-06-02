#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <stdbool.h>

char cmd[4096];

int pwd() {
  char current_dir[400];
  if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
    printf("error occur!");
  } else {
    printf("The current working directory the user on is: %s\n", current_dir);
  }
  return 0;
}

int cd(char h1[]) {

  char current_dir[100];

  printf("directory before changing");
  getcwd(current_dir, sizeof(current_dir)); //directory before changing
  printf("%s\n", current_dir);
    
  int check = chdir(h1);        //chdir returns 0 if successful and -1 if path does not exist
  if (check == 0) {             //path found
    chdir(h1);
    printf("directory after changing");
    getcwd(current_dir, sizeof(current_dir));
    printf("%s\n", current_dir);            //directory after changing
  } else                                        //path not found
    perror("path not available");

  return 0; //success
}

int killF(char split[]) {

  split = strtok(NULL, ","); 	//takes the first pid/name
    
    if(split == NULL){
       printf("try typing some PIDS/process names seperated with ','\n");
    }
  int pids[] = {}; 	        //intializing an empty string to fill in the pids
  char names[10][20];   	//intializing a 2d string arry to fill in the names of pids
  int size_Pid = 0, size_Name = 0;
  while (split != NULL) {
    if (isdigit( * split)) { 	//checking if it's a number
	 int x = atoi(split);	 //converts it from string pid to int pid 
	 pids[size_Pid] = x;
         printf("%d\n",pids[size_Pid]);
      size_Pid++;
    } else { 			//otherwise it's a name of pid
	 strcpy(names[size_Name], split);
	 printf("%s\n", split);
      size_Name++;
    }
    split = strtok(NULL, ","); 	//splits the command based on the , between the numbers or names
  }
  

  for (int i = 0; i < size_Pid; i++) { //kills every single process according to its pid
    int k = kill(pids[i], SIGTERM);
     if (k == 0)
      printf("TERMINATED!");
    else
      perror("still alive"); 
  }

  for (int i = 0; i < size_Name; i++) { //kill every single process according to its name
    int k = kill(*names[i], SIGTERM);
    if (k == 0)
      printf("TERMINATED!");
    else
      perror("still alive");
  }

  return 0;
}
void main(void) {

  while (true) {                        // infinite loop for shell
    fputs("@ ", stdout);
    
    fgets(cmd, 4096, stdin);      // reads input from the user
    cmd[strlen(cmd) - 1] = '\0';    //drops the '\n' after the last word
    
    char *h1 = strtok(cmd," "); 	// we split the string into two parts
 
    if (strcmp(h1, "pwd") == 0) {   //compares the first half to check if its pwd
      pwd();
      
    } else if (strcmp(h1, "cd") == 0) {
        h1 = strtok(NULL, " ");     // this takes the second part of the command after the space
            if(h1 == NULL){		    //user inputs nothing but cd
                cd("/");
            }else
                cd(h1);
    }else if(strcmp(h1 ,"kill") == 0 ){
        killF(h1);
    }else
        printf("command not found: %s\n" , cmd );

    }
}