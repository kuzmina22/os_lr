/* программа, реализующая базовый функционал оболочки + 
перенаправление ввода/вывода с помощью системных вызовов open и dup2
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_WORDS 16
#define MAX_WORD_LEN 80

int main() {
 char argvp[MAX_WORDS][MAX_WORD_LEN];
 int i = 0; 
 int j = 0; 
 int in = 0; 
 int STDOUT_Forward = 0; 
 int STDIN_Forward = 0; 
 printf("$");
 int c;
 while ((c = getchar())!=EOF) {
   if (c == ' ' || c == '\n' || c == '>' || c == '<') {
     if (in == 1) {
       in = 0; 
       argvp[i][j] = '\0'; 
       ++i; 
       j = 0;
     }          
   } else {
      argvp[i][j] = c; 
      ++j; 
      in = 1;
     }        
   if (c == '>') {
     STDOUT_Forward = i;
   }
   if ( c== '<') {
     STDIN_Forward = i;
   }
   if ( c== '\n') {
     argvp[i] = NULL;
     pid_t pid = fork();
     if (!pid) { // child branch
       if (STDOUT_Forward != 0) { 
         int fd_out = open(&argvp[STDOUT_Forward], O_WRONLY | O_CREAT | O_TRUNC, 0666);
         if (fd_out == -1) {
           perror("open");
           return EXIT_FAILURE;
         }
         if (-1 == dup2(fd_out, STDOUT_FILENO)) {
           perror("dup2");
           return EXIT_FAILURE;
         }
         argvp[STDOUT_Forward]=NULL;
       }
       if (STDIN_Forward != 0) { 
         int fd_in = open(&argvp[STDIN_Forward], O_RDONLY);
         if (fd_in == -1) {
           perror("open");
           return EXIT_FAILURE;
         }
         if (-1 == dup2(fd_in, STDIN_FILENO)) {
           perror("dup2");
           return EXIT_FAILURE;
         }
         argvp[STDIN_Forward]=NULL;
       }
       int rv = execvp(&argvp[0], &argvp);
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
     in = 0; 
     STDOUT_Forward = 0; 
     STDIN_Forward = 0;
     printf("$");
   }
 }
 return EXIT_SUCCESS;
}
