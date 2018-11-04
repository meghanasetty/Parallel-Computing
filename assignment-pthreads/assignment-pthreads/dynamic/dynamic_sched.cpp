/***
 * Program to find the numercial integration of f(x)dx parallely and dynamically
 * Formula: Integration of f(x)dx ~ ((b-a)/n)*summation i= 0 to n-1 of f(a+((i+0.5)*(b-a)/n))
 *
 * Input:
 * 	funtionId = integer to integrate which function we want (from libfunctions)
 * 	a = lower bound of the integral
 * 	b = higher bound of the integral
 * 	n = an integer which is the number of points to compute the approx of the integral
 * 	intensity = operation intensity
 *	nbthreads = number of threads
 *	sync = string "iteration" or "thread" or "chunk" to indicate the level of sync
 *	granularity = how many iterations does one thread take at a time.
 * Output:
 * 	stdout = the value of the integral
 * 	stderr = the time taken to compute the integral in seconds
 *
 */
#include <iostream>
#include <chrono>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	long int *start;
	long int *end;
	long int n;
	long int granularity;
	float intensity;
	unsigned int synclevel;
	double *sum;
}params;

pthread_mutex_t mut_sum,mut_loop;

void *workerThreads(void *);
double calNumericalIntegration(params *i,long int start,long int end);
bool isLoopDone(long int *end, long int n);
void getnext(long int *start,long int *end,long int n,long int granularity);


using namespace std;
int main (int argc, char* argv[]) {

  if (argc < 9) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync> <granularity>"<<std::endl;
    return -1;
  }

  int functionid = -1,nbthreads;
  long int n,granularity;
  float a,b,interval;
  float intensity;
  double sum = 0;
  long int start = 0;
  long int end = 0;
  string sync = string(argv[7]);
  unsigned int synclevel = 0; // by default 0 = iteration, 1 = thread, 2 = chunk

  //get the input values
  sscanf(argv[1],"%d",&functionid);
  sscanf(argv[2],"%f",&a);
  sscanf(argv[3],"%f",&b);
  sscanf(argv[4],"%ld",&n);
  sscanf(argv[5],"%f",&intensity);
  sscanf(argv[6],"%d",&nbthreads);
  if(sync.compare("chunk") == 0)
      synclevel = 2;
  else if(sync.compare("thread") == 0)
      synclevel = 1;
  sscanf(argv[8],"%ld",&granularity);
  //find the interval
  interval = (b-a)/n;
  //printf("The values: functionid %d\n a %f\nb %f\n n %ld\n itensity %f\n nbthreads %d\n synclevel %d\n granularity %ld\n",functionid,a,b,n,intensity,nbthreads,synclevel,granularity); 
  //threads and mutexes
  pthread_t threads[nbthreads];
  pthread_mutex_init(&mut_sum,NULL);
  pthread_mutex_init(&mut_loop,NULL);
  //starting the clock
  chrono::time_point<std::chrono::system_clock> start_s = chrono::system_clock::now();

  //creating the threads
  for(int i=0;i<nbthreads;i++)
  {
	params p;
	p.functionid = functionid;
	p.a = a;
	p.interval = interval;
	p.intensity = intensity;
	p.synclevel = synclevel;
	p.start = &start;
	p.end = &end;
	p.n = n;	
	p.granularity = granularity;
	p.sum = &sum;
  	pthread_create(&threads[i],NULL,workerThreads,new params(p));
  }
	
  //waiting for the thread completion
  for(int i=0;i<nbthreads;i++)
	pthread_join(threads[i],NULL);

  //destroying the mutex
  pthread_mutex_destroy(&mut_sum);
  pthread_mutex_destroy(&mut_loop);

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

void *workerThreads(void *pfunc)
{
	params *p = (params *)pfunc;
	double sum_thread = 0;
	//We lock the start and end variables using mut_loop and unlock it after assigning the varaibles locally.
	while(!isLoopDone(p->end,p->n))
	{
		getnext(p->start,p->end,p->n,p->granularity);
		long int start = (*p->start);
		long int end = (*p->end);
		pthread_mutex_unlock(&mut_loop);

		
		if(p->synclevel == 2)
		{
			pthread_mutex_lock(&mut_sum);
			(*p->sum) += calNumericalIntegration(p,start,end);
			pthread_mutex_unlock(&mut_sum);
		}
		else if(p->synclevel == 0)
		{
			calNumericalIntegration(p,start,end);
		}
		else if(p->synclevel == 1)
		{
			sum_thread += calNumericalIntegration(p,start,end);
		}
	}
	pthread_mutex_unlock(&mut_loop);
	if(p->synclevel == 1)
	{
		pthread_mutex_lock(&mut_sum);
		(*p->sum) +=sum_thread;
		pthread_mutex_unlock(&mut_sum);
	}
	delete p;

}

bool isLoopDone(long int *end,long int n)
{
	pthread_mutex_lock(&mut_loop);
	if((*end) == n)
		return true;
	else
		return false;
}

void getnext(long int *start,long int *end,long int n,long int granularity)
{
	//Initially start and end = 0 , later we assign star = end and end = end+granularity(if end > n we say end = n)
	(*start) = (*end);
	long int last_num = (*end) + granularity;
	if(last_num > n)
		*end = n;
	else
		*end = last_num;
}

double calNumericalIntegration(params *p,long int start,long int end)
{

//	printf("THe start and stop values are : %ld - %ld\n", start,end);
	
	//when sync = iteration
	if(p->synclevel == 0) {
		for(int j = start;j<end;j++) {
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
			pthread_mutex_lock(&mut_sum);
			*(p->sum) += funVal;
			pthread_mutex_unlock(&mut_sum);
		}
		return 0;
	}
	else {
		//when sync = thread or chunk
		double sumOfFun = 0;
		for(int j = start;j<end;j++) {
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
		return sumOfFun;
	}
}
