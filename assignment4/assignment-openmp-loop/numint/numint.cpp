#include <omp.h>
#include <stdio.h>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <chrono>

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


using namespace std;
int main (int argc, char* argv[]) {
  //forces openmp to create the threads beforehand
#pragma omp parallel
  {
    int fd = open (argv[0], O_RDONLY);
    if (fd != -1) {
      close (fd);
    }
    else {
      std::cerr<<"something is amiss"<<std::endl;
    }
  }
  
  if (argc < 9) {
    std::cerr<<"Usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }
  int functionid = -1;
  long int n;
  float a,b,interval;
  float intensity;
  int nbthreads = atoi(argv[6]);
  int granularity = atoi(argv[8]);
  double sum=0;

  //get the input values
  sscanf(argv[1],"%d",&functionid);
  sscanf(argv[2],"%f",&a);
  sscanf(argv[3],"%f",&b);
  sscanf(argv[4],"%ld",&n);
  sscanf(argv[5],"%f",&intensity);
  
  omp_set_num_threads(nbthreads);
  omp_sched_t schkind;
  string sch = string(argv[7]);
  if(sch.compare("static"))
	schkind = omp_sched_static;
  else if (sch.compare("dynamic"))
	schkind = omp_sched_dynamic;
  else
	schkind = omp_sched_guided;
  omp_set_schedule(schkind,granularity);
 
  interval = (b-a)/n;
  // start timing
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
 
  //insert code here
#pragma omp parallel for schedule(runtime) reduction(+:sum)
  for(int i=0;i<n;i++)
  {
	float funVal = 0;
	float funParam1 = a+(i + 0.5)*interval;
	switch(functionid)
	{
		case 1:
			funVal = f1(funParam1,intensity);
			break;
		case 2:
			funVal = f2(funParam1,intensity);
			break;
		case 3:
			funVal = f3(funParam1,intensity);
			break;
		case 4:
			funVal = f4(funParam1,intensity);
			break;
	}
	sum+=funVal;
  }  

  sum = sum*interval;
  // end time
  chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  chrono::duration<double> elapased_seconds = end-start;
 
  // display result
  cout<<sum<<endl;
  cerr<<elapased_seconds.count()<<std::endl;
 

  return 0;
}
