#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
int main(int argc,char *argv[])
{
  int fd;
  fd = open(argv[1],O_RDONLY);
  printf("fd = %d\n",fd);
  int newfd;
  newfd = dup(fd);
  
  printf("newfd = %d\n",newfd);
  close(fd);


  return 0;
}