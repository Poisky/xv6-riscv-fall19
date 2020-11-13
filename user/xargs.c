#include "kernel/types.h"
#include "kernel/param.h" 
#include "user/user.h"

int main(int argc, char *argv[]) {
  int args_len;
  char buf[128] = {0};
  char *args[MAXARG];

  if (argc < 2)
  {
    exit();
  }

  for (int i = 1; i < argc; i++) {
    args[i - 1] = argv[i];
  }

  while (gets(buf, sizeof(buf))) {
    int buf_len = strlen(buf);
    if (buf_len < 1)
      break;
    buf[buf_len - 1] = 0;
    args_len = argc - 1;
    char *p = buf;

    while (*p) {
      while (*p && (*p == ' '))
        *p++ = 0;
      if (*p)
        args[args_len++] = p;
      while (*p && (*p != ' '))
        p++;
    }

    if (args_len >= MAXARG) {
      printf("Too many args\n");
      exit();
    }

    args[args_len] = 0;
    
    if (fork()) {
      wait();
    }
    else{
      exec(args[0], args);
      exit();
    }
  }
  exit();
}