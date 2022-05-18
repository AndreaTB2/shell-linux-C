#include  <stdio.h>
#include  <stdlib.h>
#include  <unistd.h>

int main () {

    char current_dir[400];
    if(getcwd(current_dir , sizeof(current_dir)) == NULL){
        printf("error occur!");
    }else{
        printf("The current working directory the user on is: %s\n", current_dir);
    }
    return 0;
}