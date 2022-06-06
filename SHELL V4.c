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

char *trim(char *s)
{
    char *cp1;                              // for parsing the whole s
    char *cp2;                              // for shifting & padding

                                            // skip leading spaces, shift remaining chars
    for (cp1=s; isspace(*cp1); cp1++ )      // skip leading spaces, via cp1
        ;
    for (cp2=s; *cp1; cp1++, cp2++)         // shift left remaining chars, via cp2
        *cp2 = *cp1;
    *cp2-- = 0;                             // mark new end of string for s

    // replace trailing spaces with '\0'
    while ( cp2 > s && isspace(*cp2) )
        *cp2-- = 0;                         // pad with '\0's

    return s;
}
int pwd() {
  char current_dir[512];
  if (getcwd(current_dir, sizeof(current_dir)) == NULL) {
    printf("error occur!");
  } else {
    printf("The current working directory the user on is: %s\n", current_dir);
  }
  return 0;
}

int cd(char h1[]) {

  char current_dir[512];
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

  split = strtok(NULL, ", ;-"); 	//takes the first pid/name
 
    if(split == NULL){
       printf("try typing some PIDS/process names seperated with ','\n");
    }
  int pids[] = {}; 	        //intializing an empty string to fill in the pids
  char names[10][20];   	//intializing a 2d string arry to fill in the names of pids
  int size_Pid = 0, size_Name = 0;
  while (split != NULL) {
    trim(split);
    if (isdigit( * split)) { 	//checking if it's a number
	 int x = atoi(split);	 //converts it from string pid to int pid 
	 pids[size_Pid] = x;
         printf("%d\n",pids[size_Pid]);
      size_Pid++;
    } else { 			//otherwise it's a name of pid
	 strcpy(names[size_Name], split);
	 printf("%s\n", names[size_Name]);
      size_Name++;
    }
    split = strtok(NULL, ", ;-"); 	//splits the pids/names based on ', whiteSpace ; and - ' between the numbers or names
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
void help(){
    printf("\n");
    printf("You can enter the following commands:\n\n");
    printf("pwd  >>> To show the working directory\n");
    printf("cd   >>> To change the path directory\n");
    printf("kill >>> To kill some proccesses\n");
    printf("ps   >>> To list proccess\n");
    printf("ps -a   >>> To list all proccess\n");

    
    printf("exit   >>> To exit\n");
    printf("\n");
}


 

int main() {

  while (true) {                        // infinite loop for shell
    fputs("@ ", stdout);

    fgets(cmd, 4096, stdin);        // reads input from the user
    //cmd[strlen(cmd) - 1] = '\0';    //drops the '\n' after the last word
    trim(cmd);
    char *h1 = strtok(cmd," "); 	// we split the string into two parts
    //printf("%s",h1); 

    if(h1 == NULL){                 //if user inputs nothing
        printf("try typing some commands...\n");
        help();
    }
    else{
        
        /*----------------->PWD<-----------------*/
        
        if (strcmp(h1, "pwd") == 0) {   //compares the first half to check if its pwd
            h1 = strtok(NULL, " ");     // this takes the second part of the command after the space
            if(h1 == NULL)
                pwd();
            else
                printf("wrong command! type only pwd\n");
        }
        
        /*----------------->CD<-----------------*/
        
        else if (strcmp(h1, "cd") == 0) {
            h1 = strtok(NULL, " ");     // this takes the second part of the command after the space
            if(h1 == NULL)
                cd("/");
            else{
                cd(h1);
            }
        }
        
        /*----------------->KILL<-----------------*/
        
        else if(strcmp(h1 ,"kill") == 0 ){
            killF(h1);
        }else if(strcmp(h1 , "exit") == 0){
            exit(1);
        }
        else{
            printf("command not found: %s\n" , cmd );
         }
         
        }
            memset(cmd, 0, 4096);

    }
    return 0;
}