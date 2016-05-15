#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>

int main(){
 int i, j, c, in, STDOUT_Forward, STDIN_Forward, sh;
 int fildes[2]; 
 char argvp[16][80];
 char* argv[16];
 printf("$"); 
 i=0; j=0; in=0; 
 STDOUT_Forward=0; STDIN_Forward=0; 
 sh=0;
 while ((c=getchar())!=EOF) {
   if (c==' ' || c=='\n' || c=='>' || c=='<' || c=='|') {
      if (in==1) {in=0; argvp[i][j]='\0'; argv[i]=&argvp[i]; ++i; j=0;}          
   } else {argvp[i][j]=c; ++j; in=1;};        
   
   if (c=='|') {sh=i; argv[sh]=NULL; ++i;};

   if (c=='\n') {
     argv[i]=NULL;
     pipe(fildes);
     pid_t pid1 = fork();
     if (!pid1) { // child1 branch
       close (fildes[0]); 
       if (-1 == dup2(fildes[1], STDOUT_FILENO)) {
          perror("dup2");
          return EXIT_FAILURE;
       }
       int rv = execvp(argv[0], argv);
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
       int rv = execvp(argv[sh+1], argv);
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
     i=0; j=0; in=0; 
     STDOUT_Forward=0; STDIN_Forward=0; 
     sh=0;
     printf("$");
   }
 }
 printf("\n");
 return EXIT_SUCCESS;
}
