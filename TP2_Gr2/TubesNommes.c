#include <unistd.h> 
#include <stdio.h> 
#include <sys/wait.h> 
#include <fcntl.h>
#include <sys/stat.h>

int main( ) {
    int pid;
    // création de deux pipe
    mkfifo("fd", 0660);
    mkfifo("fd2", 0660);

    if (fork()==0) {
        // premier fils
        int file = open("./In.txt", O_RDONLY);
        int fd = open("fd", O_WRONLY);

        dup2(file,0);         
        dup2(fd,1);  
        close(fd);    

        execl("/bin/rev","rev",NULL);       

        _exit(1);     
    }   

    if (fork()==0) {
        // second  fils   
        int fd = open("fd", O_RDONLY); 
        int fd2 = open("fd2", O_WRONLY);    

        dup2(fd,0);
        dup2(fd2,1);        
        close(fd);
        close(fd2);

        execl("/bin/rev","rev", NULL);

        _exit(1);     
    }

    if (fork()==0) {
        // troisième  fils    
        int fd2 = open("fd2", O_RDONLY);  

        dup2(fd2,0);        
        close(fd2);
        
        execl("/bin/diff","diff", "-", "./In.txt", "-s", NULL);

        _exit(1);     
    }

    while ((pid=wait(NULL)) > 0);    
    return 0;
} 