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

#define DEBUG 1
#define MAXLINELEN 4096
#define MAXARGS 128
#define END_OF_LINE 0
#define SEQ_OP ';'
#define SEQUENCE 1

struct cmd {
  struct cmd * next;
  int terminator;
  char * exe_path;
  int nargs;
  char * arg[MAXARGS];
};

void * ck_malloc(size_t size) {
  void * ret = malloc(size);
  if (!ret) {
    perror("dumbshell:ck_malloc");
    exit(1);
  }
  return ret;
}

char * skip_to_non_ws(char * p) {
  int ch;
  while (ch = * p) {
    if (ch != ' ' && ch != '\t' && ch != '\n') return p;
    ++p;
  }
  return 0;
}

char * skip_to_ws_or_sep(char * p) {
  int ch;
  while (ch = * p) {
    if (ch == ' ' || ch == '\t' || ch == '\n') return p;
    if (ch == SEQ_OP) return p;
    ++p;
  }
  return 0;
}

struct cmd * parse_commands(char * line) {
  char * ptr;
  int ix;
  struct cmd * cur;

  ptr = skip_to_non_ws(line);
  if (!ptr) return 0;
  cur = ck_malloc(sizeof * cur);
  cur -> next = 0;
  cur -> exe_path = ptr;
  cur -> arg[0] = ptr;
  cur -> terminator = END_OF_LINE;
  ix = 1;
  for (;;) {
    ptr = skip_to_ws_or_sep(ptr);
    if (!ptr) {
      break;
    }
    if ( * ptr == SEQ_OP) {
      * ptr = 0;
      cur -> next = parse_commands(ptr + 1);
      if (cur -> next) {
        cur -> terminator = SEQUENCE;
      }
      break;
    }
    * ptr = 0;
    ptr = skip_to_non_ws(ptr + 1);
    if (!ptr) {
      break;
    }
    if ( * ptr == SEQ_OP) {
      /* found a sequence operator */
      cur -> next = parse_commands(ptr + 1);
      if (cur -> next) {
        cur -> terminator = SEQUENCE;
      }
      break;
    }
    cur -> arg[ix] = ptr;
    ++ix;
  }
  cur -> arg[ix] = 0;
  cur -> nargs = ix;
  return cur;
}

void execute(struct cmd * clist) {
  int pid, npid, stat;

  if(strcmp(clist->exe_path, "pwd") == 0){
  	pwd();
  }
  if (strcmp(clist->exe_path, "cd") == 0){
  	
  	cd(clist->arg[1]);
     }
  pid = fork();
  if (pid == -1) {
    perror("dumbshell:fork");
    exit(1);
  }

  
  if (!pid) {
    /* child */
    execvp(clist -> exe_path, clist -> arg);
    fprintf(stderr, "No such command: %s\n", clist -> exe_path);
    exit(1);
  }
  do {

    npid = wait( & stat);
    printf("Process %d exited with status %d\n", npid, stat);
  } while (npid != pid);
  switch (clist -> terminator) {
  case SEQUENCE:
    execute(clist -> next);
  }
}
int pwd(){
   char current_dir[400];
    if(getcwd(current_dir , sizeof(current_dir)) == NULL){
        printf("error occur!");
    }else{
        printf("The current working directory the user on is: %s\n", current_dir);
    }
    return 0;
}

int cd(char cmd[100]) {

    //char cmd[50] = "cd /home/andrea/Documents/";
    
    char current_dir[100];
    
    printf("directory before changing");
    getcwd(current_dir, sizeof(current_dir));   //directory before changing
    printf("%s\n", current_dir);
    
    char *h1 = strtok(cmd," "); 	// we split the string into two parts
    h1 = strtok(NULL, " ");		// this takes the second part of the command after the space
    int check = chdir(h1);		//chdir returns 0 if successful and -1 if path does not exist
    printf("%d\n",check);
     if(check == 0){			//path found
    	chdir(h1);
    	printf("directory after changing");
    	getcwd(current_dir, sizeof(current_dir));
    	printf("%s\n", current_dir);		//directory after changing
    }
    else				//path not found
    	perror("path not available");

    return 0; //success
}


void free_commands(struct cmd * clist) {
  struct cmd * nxt;

  do {
    nxt = clist -> next;
    free(clist);
    clist = nxt;
  } while (clist);
}

char * get_command(char * buf, int size, FILE * in ) {
  if ( in == stdin) {
    fputs("@ ", stdout); /* prompt */
  }
  return fgets(buf, size, in );
}

void main(void) {
  char linebuf[MAXLINELEN];
  struct cmd * commands;

  while (get_command(linebuf, MAXLINELEN, stdin) != NULL) {
    commands = parse_commands(linebuf);
    if (commands) {
      execute(commands);
      free_commands(commands);
    }
  }
}
