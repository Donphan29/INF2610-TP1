#include <unistd.h> 
#include <stdio.h> 
#include <sys/wait.h> 
#include <fcntl.h>

int main( ) {
    int pid;
    // création de deux pipe
    int fd[2]; pipe(fd);
    int fd2[2]; pipe(fd2);

    if (fork()==0) {
        // premier fils
        int file = open("./In.txt", O_RDONLY);

        dup2(file,0);         
        dup2(fd[1],1);  

        close(fd[0]); 
        close(fd[1]); 
        close(fd2[0]); 
        close(fd2[1]);  

        execl("/bin/rev","rev",NULL); 

        _exit(1);     
    }     

    if (fork()==0) {
        // second  fils            
        dup2(fd[0],0);
        dup2(fd2[1],1);   

        close(fd[0]); 
        close(fd[1]);
        close(fd2[0]); 
        close(fd2[1]); 

        execl("/bin/rev","rev", NULL);

        _exit(1);     
    }

    if (fork()==0) {
        // troisième  fils            
        dup2(fd2[0],0);  

        close(fd[0]); 
        close(fd[1]);
        close(fd2[0]);
        close(fd2[1]); 

        execl("/bin/diff","diff", "-", "./In.txt", "-s", NULL);

        _exit(1);     
    } 

    close(fd[0]); 
    close(fd[1]);
    close(fd2[0]); 
    close(fd2[1]); 
     
    while ((pid=wait(NULL)) > 0);    
    return 0;
} 