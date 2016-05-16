/*программа, реализующая базовый функционал оболочки путем порождения нового процесса (системный вызов fork) 
и запуска в нем программы команды, введенной пользователем с помощью системного вызова execvp.
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
 int i = 0; 
 int j = 0; 
 int in = 0;
 int c;
 printf("$");
 while ((c = getchar())!=EOF) {
   if (isspace(c)) {
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
   if (c=='\n') {
     argv[i]=NULL;
     pid_t pid = fork();
     if (!pid) { // child branch
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
     printf("$");
   }
 }
 return EXIT_SUCCESS;
}
