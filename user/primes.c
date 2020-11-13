#include "kernel/types.h"
#include "user/user.h"

void primes(int *num, int numsize)
{
    int pipe_fd[2];
    pipe(pipe_fd);
    int count=0;
    printf("prime %d\n",num[0]);
    int nextnum[34];
    
    if(fork())
    {
        close(pipe_fd[0]);
        for(int i=0;i<numsize;i++)
        {
            if(num[i]%num[0]!=0)
            {
                write(pipe_fd[1],&num[i],sizeof(num[i]));
            }
        }
        close(pipe_fd[1]);
	wait();
    }
    else
    {
        close(pipe_fd[1]);
        while(read(pipe_fd[0],&nextnum[count],sizeof(nextnum[count]))) 
        {
            count++;
        }
     
        close(pipe_fd[0]);
        if(count>=1)
        {
            primes(nextnum,count);
        }    
    }
    exit();
}
int main(int argc, char *argv[]) 
{
    int buffer[34];
    for(int i=0;i<34;i++)
    {
        buffer[i]=i+2;
    }
    primes(buffer,34);
    
    exit();
}
