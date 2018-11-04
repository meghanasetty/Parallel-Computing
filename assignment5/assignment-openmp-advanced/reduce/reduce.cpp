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

int calculatePartialSum(int start,int end,int * arr);

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
  
  if (argc < 3) {
    std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int * arr = new int [n];
  int nbthreads = atoi(argv[2]);
  int partial_sum[nbthreads];
  for(int i =0;i<nbthreads;i++)
	  partial_sum[i] = 0;
  generateReduceData (arr, atoi(argv[1]));
  //set the number of threads
  omp_set_num_threads(nbthreads);
  // start timing
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  int result = 0;
#pragma omp parallel
  {
	#pragma omp single
	{
			
  		int sum = 0;
		int start =0;
		int end = 0;
  	  for(int i = 0;i<nbthreads;i++)
	  {
	  	start = end;
		if(i == nbthreads -1)
			end = n;
		else
			end = (i+1)*(n/nbthreads);
		#pragma omp task shared(partial_sum)
			#pragma omp depend(inout: partial_sum[omp_get_thread_num()])
				partial_sum[omp_get_thread_num()] += calculatePartialSum(start,end,arr);
	  }
	  #pragma omp taskwait
	  for(int i=0;i<nbthreads;i++)
		  result += partial_sum[i];
	}
  }
  // end time
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapased_seconds = end-start;

  // display result
  std::cout<<result<<std::endl;
  std::cerr<<elapased_seconds.count()<<std::endl;

  
  delete[] arr;

  return 0;
}



int calculatePartialSum(int start,int end,int * arr)
{
	int temp = 0;
	for(int i=start;i<end;i++)
		temp = temp+arr[i];
	return temp;
}
