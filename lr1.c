#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(){
 int i, j, c, in; 
 char argvp[16][80];
 char* argv[16];
 printf("$"); 
 i=0; j=0; in=0;
 while ((c=getchar())!=EOF) {
   if (c==' ' || c=='\n') {
     if (in==1) {in=0; argvp[i][j]='\0'; argv[i]=&argvp[i]; ++i; j=0;}          
   } else {argvp[i][j]=c; ++j; in=1;};        
 
   if (c=='\n') {
     argv[i]=NULL;
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
     i=0; j=0; in=0;
     printf("$");
   }
 }
 printf("\n");
 return EXIT_SUCCESS;
}
