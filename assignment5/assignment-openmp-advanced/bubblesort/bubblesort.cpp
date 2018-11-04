#include <omp.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>

#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (const int* arr, size_t n);

#ifdef __cplusplus
}
#endif

void swap(int* arr, int i, int j) {
  int temp = arr[i];
  arr[i] = arr[j];
  arr[j] = temp;
}

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
  
  if (argc < 3) { std::cerr<<"usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);  
  // get arr data
  int * arr = new int [n];

  generateMergeSortData (arr, n);

  //set the threads
  omp_set_num_threads(nbthreads);

  chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();

  //insert sorting code here.
		for(int i = 0;i<n/2;i++)
		{
		  #pragma omp parallel
		  {
			#pragma omp for
			for(int j=0;j<n-1;j=j+2)
			{
				if(arr[j] > arr[j+1])
				{
					swap(arr,j,j+1);
				}
			}
			#pragma omp for
			for(int k=1;k<n-1;k=k+2)
			{
				if(arr[k] > arr[k+1])
				{
					swap(arr,k,k+1);
				}
	 		}
		  }		
	 	}		

  chrono::time_point<std::chrono::system_clock> end = chrono::system_clock::now();
  chrono::duration<double> elpased_seconds = end-start;

  cerr<<elpased_seconds.count()<<std::endl;

  
  checkMergeSortResult (arr, n);
  
  delete[] arr;

  return 0;
}
