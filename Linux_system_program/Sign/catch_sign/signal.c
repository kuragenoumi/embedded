#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
void sys_err(char *str)
{
  perror(str);
  exit(1);
}

void sys_sig(int signo){
  printf("----catch sign %d\n", signo);
}

int main(int argc,char *argv[])
{
  signal(SIGINT,sys_sig);
  signal(SIGQUIT, sys_sig);

while (1);
  return 0;
}