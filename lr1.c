/*The program that implements the basic shell functionality by generating a new process (system call "fork") 
and launching command's program in it inputted by user using the system call "execvp"
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_WORDS 16
#define MAX_WORD_LEN 80

int main() {
 char argvp[MAX_WORDS][MAX_WORD_LEN];
 char* argv[MAX_WORDS]
 int i = 0; 
 int j = 0; 
 int in = 0;
 int c;
 printf("$");
 while ((c = getchar()) != EOF) {
   if (isspace(c)) {
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
   if (c == '\n') {
     argv[i] = NULL;
     pid_t pid = fork();
     if (!pid) { // child branch
       int rv = execvp(argv[0], argv);
       if (rv == -1) {
         perror("execvp");
         return EXIT_FAILURE;
       }
     }
     // parent branch
     pid = wait(NULL);
     if (pid == -1) {
       perror("wait");
       return EXIT_FAILURE;
     }
     i = 0; 
     j = 0; 
     printf("$");
   }
 }
 return EXIT_SUCCESS;
}
