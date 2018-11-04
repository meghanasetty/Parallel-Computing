/***
 * Program to find the numercial integration of f(x)dx parallely
 * Formula: Integration of f(x)dx ~ ((b-a)/n)*summation i= 0 to n-1 of f(a+((i+0.5)*(b-a)/n))
 *
 * Input:
 * 	funtionId = integer to integrate which function we want (from libfunctions)
 * 	a = lower bound of the integral
 * 	b = higher bound of the integral
 * 	n = an integer which is the number of points to compute the approx of the integral
 * 	intensity = operation intensity
 *	nbthreads = number of threads
 *	sync = string "iteration" or "thread" to indicate the level of sync
 * Output:
 * 	stdout = the value of the integral
 * 	stderr = the time taken to compute the integral in seconds
 *
 */

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>
#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

typedef struct params {
	int functionid;
	float a;
	float interval;
	long int start;
	long int end;
	float intensity;
	bool isIteration;
	double *sum;
}params;
pthread_mutex_t mut;


void * calNumericalIntegration(void *);



using namespace std;
int main (int argc, char* argv[]) {
  
  if (argc < 8) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync>"<<std::endl;
    return -1;
  }
  int functionid = -1,nbthreads;
  long int n;
  float a,b,interval;
  float intensity;
  double sum = 0;
  string sync = string(argv[7]);
  bool isIteration = false; // true if sync = 'iteration'

  //get the input values
  sscanf(argv[1],"%d",&functionid);
  sscanf(argv[2],"%f",&a);
  sscanf(argv[3],"%f",&b);
  sscanf(argv[4],"%ld",&n);
  sscanf(argv[5],"%f",&intensity);
  sscanf(argv[6],"%d",&nbthreads);
  if(sync.compare("iteration") == 0)
      isIteration = true;
  
  //find the interval
  interval = (b-a)/n;
  
  //threads and mutexes
  pthread_t threads[nbthreads];
  pthread_mutex_init(&mut,NULL);
  
  //starting the clock
  chrono::time_point<std::chrono::system_clock> start_s = chrono::system_clock::now();
  
  //calculating the start and end iteration values
  long int j = 0;
  for(int i =0;i<nbthreads;i++)
  {
	params p;
	p.functionid = functionid;
	p.a = a;
	p.start = j;
	if(i == nbthreads - 1) { // last one
		p.end = n;
	}
	else {
		p.end = (n/nbthreads)*(i+1);
		j = p.end;
	}
	p.interval = interval;
	p.intensity = intensity;
	p.isIteration = isIteration;
	p.sum = &sum;
  	pthread_create(&threads[i],NULL,calNumericalIntegration,new params(p));
  }

  //waiting for the thread completion
  for(int i=0;i<nbthreads;i++)
	pthread_join(threads[i],NULL);

  //destroying the mutex
  pthread_mutex_destroy(&mut);

  //calculating the result
  sum = sum*interval;

  //output
  cout<<sum<<endl;

  //stoping the clock
  chrono::time_point<std::chrono::system_clock> stop_s = chrono::system_clock::now();
  
  chrono::duration<double> elapsed_seconds = stop_s - start_s;
  
  
  //error as time taken for computation
  cerr << elapsed_seconds.count() <<endl;
  return 0;
}


void * calNumericalIntegration(void * pfunc)
{
	params *p = (params * )pfunc;

	//when sync = iteration
	if(p->isIteration) {
		for(int j = p->start;j<p->end;j++) {
			float funVal = 0;
			float funParam1 = p->a+(j+0.5)*p->interval;
			switch(p->functionid)
			{
				case 1:
					funVal = f1(funParam1,p->intensity);
					break;
				case 2:
					funVal = f2(funParam1,p->intensity);
					break;
				case 3:
					funVal = f3(funParam1,p->intensity);
					break;
				case 4:
					funVal = f4(funParam1,p->intensity);
					break;
			}
			pthread_mutex_lock(&mut);
			*(p->sum) += funVal;
			pthread_mutex_unlock(&mut);
		}
	}
	else {
		//when sync = thread
		double sumOfFun = 0;
		for(int j = p->start;j<p->end;j++) {
			float funVal = 0;
			float funParam1 = p->a+(j+0.5)*p->interval;
			switch(p->functionid)
			{
				case 1:
					funVal = f1(funParam1,p->intensity);
					break;
				case 2:
					funVal = f2(funParam1,p->intensity);
					break;
				case 3:
					funVal = f3(funParam1,p->intensity);
					break;
				case 4:
					funVal = f4(funParam1,p->intensity);
					break;
			}
			sumOfFun +=funVal;
		}
		pthread_mutex_lock(&mut);
		*(p->sum) += sumOfFun;
		pthread_mutex_unlock(&mut);
	}
	delete p;
	return NULL;
}
