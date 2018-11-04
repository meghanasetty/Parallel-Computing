#include<stdio.h>
#include<unistd.h>
#include<errno.h>

#define MAX_HOSTNAME_LENGTH 30

int main () {
  char hostname[MAX_HOSTNAME_LENGTH];
  if(gethostname(hostname,MAX_HOSTNAME_LENGTH) == -1) {
    printf("Cannot get the hostname. Errno: %d\n",errno);
    return -1;
  }
  else
    printf("%s\n",hostname);
  return 0;
}
