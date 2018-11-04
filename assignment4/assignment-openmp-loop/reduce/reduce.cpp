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
  void generateReduceData (int* arr, size_t n);
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
  
  if (argc < 5) {
    std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads> <scheduling> <granularity>"<<std::endl;
    return -1;
  }
  int n = atoi(argv[1]);
  int * arr = new int [n];

  generateReduceData (arr, atoi(argv[1]));
  
  int nbthreads = atoi(argv[2]);
  omp_set_num_threads(nbthreads);
  omp_sched_t schkind;
  string sch = string(argv[3]);
  if(sch.compare("static"))
	schkind = omp_sched_static;
  else if (sch.compare("dynamic"))
	schkind = omp_sched_dynamic;
  else
	schkind = omp_sched_guided;
  int granularity = atoi(argv[4]);
  omp_set_schedule(schkind,granularity);
 
  // start timing
  chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
 
  //insert reduction code here
  int sum =0;
#pragma omp parallel for reduction(+:sum) schedule(runtime)
  for(int i=0;i<n;i++)
  {
	sum += arr[i];
  }

  cout<<sum<<endl;
  
  // end time
  chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  chrono::duration<double> elapased_seconds = end-start;
 
  // display result
  cerr<<elapased_seconds.count()<<endl;
 
  delete[] arr;

  return 0;
}
