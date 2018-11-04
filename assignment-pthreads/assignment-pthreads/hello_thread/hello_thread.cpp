#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

using namespace std;

typedef struct params
{
	int i;
	int noOfThreads;
}params;
void * printThreads(void *p);

int main (int argc, char* argv[]) {

  if (argc < 2) {
    std::cerr<<"usage: "<<argv[0]<<" <nbthreads>"<<std::endl;
    return -1;
  }

  long int noOfThreads = atoi(argv[1]);
  pthread_t threads[noOfThreads];
  for(int i=0;i<noOfThreads;i++) {
  	params p;
	p.i = i;
	p.noOfThreads = noOfThreads;
	pthread_create(&threads[i],NULL,printThreads,&p);
  }
  for(int i=0;i<noOfThreads;i++) {
  	pthread_join(threads[i],NULL);
  }
  
  return 0;
}

void * printThreads(void *p)
{
	printf("I am %d of %d\n",((params *)p)->i,((params *)p)->noOfThreads);
	return NULL;
}
