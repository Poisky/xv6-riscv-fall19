#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fcntl.h"
 
void runpipe(char *p[],int n);
char s[100][100];//数组s存储拆分输入后的结果，设为全局变量便于操作

//仿照sh.c文件读取指令
int getcmd(char *buf, int nbuf)
{
  fprintf(2, "@ ");
  memset(buf, 0, nbuf);
  gets(buf, nbuf);
  if(buf[0] == 0) // EOF
    return -1;
  return 0;
}

//处理输入,将输入拆分并存到s数组中，返回拆分后参数个数
int handleinput(char *buf)
{
  int x = 0;
  int y = 0;
  for(int i=0;buf[i]!='\0'&&buf[i]!='\n';i++)
  {
    if(buf[i]==' ');
    else if(buf[i]=='<'||buf[i]=='>'||buf[i]=='|')
    {
      if(buf[i-1]!=' ')
      {
        s[x][y]='\0';
        x++;
        y=0;
      }
      s[x][y++]=buf[i];
      s[x][y]='\0';
      x++;
      y=0;
    }
    else if(buf[i+1]!=' ')
    {
      s[x][y++]=buf[i];
    }
    else
    {
      s[x][y++]=buf[i];
      s[x][y]='\0';
      x++;
      y=0;
    }
  }
  s[x][y]='\0';
  return x;
}


//通过exec执行指令,若出现"<"或">"则需要进行重定向
void runcmd(char *p[],int n)
{
  for(int i = 0;i<n;i++)
  {
    if(!strcmp(p[i],"|"))
    {
      runpipe(p,n);//如果存在"|",说明需要执行管道指令
    }
  }
  int i;
  for(i=0;i<=n;i++)
  {
    if(!strcmp(p[i],">"))
    {
      close(1);
      open(p[i+1],O_CREATE|O_WRONLY);
      p[i]=0;
    }
    if(!strcmp(p[i],"<"))
    {
      close(0);
      open(p[i+1],O_RDONLY);
      p[i]=0;
    }
  }
  p[i]=0;
  exec(p[0],p);
}

//仿照sh.c文件实现管道指令
void runpipe(char *p[],int n)
{
  int i;
  //找到第一个‘|’符号
  for(i = 0;i<n;i++)
  {
    if(!strcmp(p[i],"|"))
    {
      p[i]=0;
      break;
    }
  }
  int fd[2];
  pipe(fd);
  if(fork()==0)
  {
    close(1);
    dup(fd[1]);
    close(fd[0]);
    close(fd[1]);
    runcmd(p,i);
  }
  else
  {
    close(0);
    dup(fd[0]);
    close(fd[0]);
    close(fd[1]);
    runcmd(p+i+1,n-i-1);
  }
  close(fd[0]);
  close(fd[1]);
  exit(0);
}


int main(void)
{
  char buf[100];
  
  while(getcmd(buf, sizeof(buf)) >= 0)
  {
    if(fork() == 0)
    {
      int n = handleinput(buf);
      char * p[100];
      for(int i=0;i<=n;i++)
      {
        p[i]=s[i];
      }
      runcmd(p,n);
    }
    wait(0);
  } 
  exit(0);
}

