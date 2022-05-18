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

int main() {

    		//if changing directory (chdir) returns -1 handle error.
    char cmd[100] = "cd ..";

    char *h1 = strtok(cmd," "); // we split the string into two parts
    h1 = strtok(NULL, " ");	// this takes the second part of the command after the space
    printf(h1);
    int check = chdir(h1);	//chdir returns 0 if successful and -1 if path does not exist
    printf("%d",check);
    if(check == -1)	//path not found
    	perror("path not available");
    else if(check == 0){	//path found
    	chdir(h1);
    	}
    else if(strcmp(h1,"..") == 0){	//parent paths
    	chdir(h1);
    	}
    else if(strcmp(h1,"/") == 0)	//root path
    	chdir(h1);
    

    return 0; //success
}