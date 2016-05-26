#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_WORDS 16
#define MAX_WORD_LEN  80

int is_punctuation(int a) { 
  return isspace(a) || a == '>' || a == '<' || a == '|'; 
}

int main() {
 int fildes[2]; 
 char argvp[MAX_WORDS][MAX_WORD_LEN];
 char* argv[MAX_WORDS];
 char* argv1[MAX_WORDS];
 char* argv2[MAX_WORDS];
 int i = 0;
 int j = 0; 
 int in = 0; 
 int pi = 0;
 int c;
 printf("$");
 while ((c = getchar()) != EOF) {
   if (is_punctuation(c)) {
     if (in == 1) {
       in = 0;
       argvp[i][j] = '\0'; 
       argv[i] = argvp[i]; 
       ++i; 
       j = 0;
     }          
   } else {
       argvp[i][j] = c; 
       ++j; 
       in = 1;
   }        
   if (c == '|') {
     pi = i; 
     argv[pi] = NULL; 
     ++i;
   }
   if (c == '\n') {
     argv[i] = NULL;
     for (int j = 0; j < pi; j++){
      argv1[j] = argv[j]; 
     }
     for (int j = 0; j < i-pi-1; j++){
      argv2[j] = argv[j+pi+1]; 
     }
     if (pipe(fildes) == -1){
       perror("pipe");
       return EXIT_FAILURE;
     }
     pid_t pid1 = fork();
     if (!pid1) { // child1 branch
       close (fildes[0]); 
       if (-1 == dup2(fildes[1], STDOUT_FILENO)) {
          perror("dup2");
          return EXIT_FAILURE;
       }
       int rv = execvp(argv1[0], argv1);
       if (rv == -1) {
          perror("execvp");
          return EXIT_FAILURE;
       }
     }
     pid_t pid2 = fork();
     if (!pid2) { // child2 branch
       close( fildes[1]); 
       if (-1 == dup2(fildes[0], STDIN_FILENO)) {
          perror("dup2");
          return EXIT_FAILURE;
       }
       int rv = execvp(argv2[0], argv2);
       if (rv == -1) {
          perror("execvp");
          return EXIT_FAILURE;
       }
     }
     // parent branch
     pid1 = wait(NULL);
     if (pid1 == -1) {
       perror("wait");
       return EXIT_FAILURE;
     }
     pid2 = wait(NULL);
     if (pid2 == -1) {
       perror("wait");
       return EXIT_FAILURE;
     }
     close (fildes[0]);
     close (fildes[1]);
     i = 0; 
     j = 0; 
     pi = 0;
     printf("$");
   }
 }
 return EXIT_SUCCESS;
}
