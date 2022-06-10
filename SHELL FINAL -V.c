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
#include<readline/readline.h>
#include<readline/history.h>

char *cmd;
char * h1;
void procRead(int pid);

//this trim method is from the internet...
char * trim(char * s) {
  char * cp1; // for parsing the whole s
  char * cp2; // for shifting & padding

  // skip leading spaces, shift remaining chars
  for (cp1 = s; isspace( * cp1); cp1++); // skip leading spaces, via cp1
  for (cp2 = s;* cp1; cp1++, cp2++) // shift left remaining chars, via cp2
 
  * cp2 = * cp1;
  * cp2-- = 0; // mark new end of string for s

  // replace trailing spaces with '\0'
  while (cp2 > s && isspace( * cp2))
    *							
    cp2-- = 0; // pad with '\0's

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

  int check = chdir(h1); //chdir returns 0 if successful and -1 if path does not exist
  if (check == 0) { //path found
    printf("path found!\n");
  } else //path not found
    perror("path not available");

  return 0; //success
}

int killMethod(char split[]) {

  split = strtok(NULL, ", ;-"); //takes the first pid/name

  if (split == NULL) {
    printf("try typing some PIDS/process names seperated with ', ;-'\n");
  }
  int pids[] = {}; //intializing an empty string to fill in the pids
  char names[10][20]; //intializing a 2d string arry to fill in the names of pids
  int size_Pid = 0, size_Name = 0;
  while (split != NULL) {
    trim(split);
    if (isdigit( * split)) { //checking if it's a number
      int x = atoi(split); //converts it from string pid to int pid 
      pids[size_Pid] = x;
      size_Pid++;
    } else { //otherwise it's a name of pid
      strcpy(names[size_Name], split);
      size_Name++;
    }
    split = strtok(NULL, ", ;-"); //splits the pids/names based on ', whiteSpace ; and - ' between the numbers or names
  }

  for (int i = 0; i < size_Pid; i++) { //kills every single process according to its pid
    int k = kill(pids[i], SIGTERM);
    if (k == 0)
      printf("TERMINATED!\n");
    else
      perror("still alive\n");
  }

  for (int i = 0; i < size_Name; i++) { //kill every single process according to its name
    int k = kill( * names[i], SIGTERM);
    if (k == 0)
      printf("TERMINATED!\n");
    else
      perror("still alive\n");
  }

  return 0;
}
void help() {
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

int isEmptyAfterFirstWord() {

  h1 = strtok(NULL, " "); // this takes the second part of the command after the space
  if (h1 == NULL) {
    return 1;
  }
  return 0;
}


void PS() {

  DIR * directory;
  struct dirent * ptr;

  printf("   PID\tTTY\tTime\t    CMD\n");
  
    directory = opendir("/proc"); 	//open the dirctory

  if (isEmptyAfterFirstWord()) {	//for ps alone
    procRead(getppid()); 		//parent proccess
    procRead(getpid()); 		//child proccess
  } else if (strcasecmp(h1, "-a") == 0) { 		//for ps -a
	
    while ((ptr = readdir(directory)) != NULL) { // keep reading until EOF
      int pid;
      if ((ptr -> d_type == DT_DIR)) { //check if it's a directory
        pid = atoi(ptr -> d_name);	//change pid names into ints
        if (pid > 0) {
          procRead(pid);
        }
      }
    }
  } else
        perror("type ps command properly!");
}

//this method reads from the proc files at a specific given PID and returns the info about it

void procRead(int pid) {

  char pathFile[300];
  char * tty;
  int fd;

  sprintf(pathFile, "/proc/%d/fd/0", pid); //stores in the array 
  fd = open(pathFile, 'r'); //open the file 0
  tty = ttyname(fd); //gets the terminal name from the fd

  if (tty != NULL) {	//some ttys have null value
    FILE * statFile;
    sprintf(pathFile, "/proc/%d/stat", pid); //stores values in the array 
    statFile = fopen(pathFile, "r"); //open the file

    char cmd[300];
    long startT, userModeT;
    fscanf(statFile, "%*d %s %*c %*d %*d %*d %*d %*d %*d %*d %*d %*d %lu %lu", cmd, & userModeT, & startT);	//format for the stat file

    fclose(statFile); 		//close the file
    long hz = sysconf(_SC_CLK_TCK); //number of clocks per sec

    int total_Time = userModeT + startT;	//sums the user time and kernel time for proccess

    int Process_Time_Sec = total_Time / hz;	//divides the time of proccess by the clocks/s
    int Process_Time_Min = Process_Time_Sec / 60;	//converts into minutes
    int secs = Process_Time_Sec - ((int)(total_Time / (hz * 60)) * 60);	//converts the proccess time into actual seconds
    int mins = Process_Time_Sec / 60 - ((int)(Process_Time_Sec / (60 * 60)) * 60);  //into minutes
    int hours = Process_Time_Min / 60;	//into hours

    char time_s[400];

    //time format -hour, minutes, seconds-
    sprintf(time_s, "%.2d:%.2d:%.2d", hours, mins, secs);	//stores time in the array
    printf("%6d %6s \t%s %9s \n", pid, tty + 5, time_s, cmd); //print the output 

  }
}


int main() {

  while (true) { // infinite loop for shell

	cmd = readline("$ ");	//reads a whole line
	
       if (strlen(cmd) != 0) {
         add_history(cmd);	//add commands to its history
       }
    trim(cmd);
    h1 = strtok(cmd, " "); 	// we split the string into two parts and takes the first part
    if (h1 == NULL) { 		//if user inputs nothing
      printf("try typing some commands...\n");
      help();
    } else {

      /*----------------->PWD<-----------------*/

      if (strcmp(h1, "pwd") == 0) { 	//compares the first half to check if its pwd    
        if (isEmptyAfterFirstWord()) {	
          pwd();
        } else
          printf("wrong command! type only pwd\n");
      }

      /*----------------->CD<-----------------*/
      else if (strcmp(h1, "cd") == 0) {
        if (isEmptyAfterFirstWord()) {		//if users inputs nothing but cd
          cd("/");
        } else {
          cd(h1);
        }
      }

      /*----------------->KILL<-----------------*/
      else if (strcmp(h1, "kill") == 0) {
        killMethod(h1);
      }

      /*----------------->PS<------------------*/
      else if (strcmp(h1, "ps") == 0) {
        PS();
      }else if(strcasecmp(h1, "help") == 0){
      	help();
      }else if (strcmp(h1, "exit") == 0) {
        exit(1);
      } else {
        printf("command not found: %s\n", cmd);
      }
    }
    	free(cmd);		//deallocates memory for cmd

  }
  return 0;
}