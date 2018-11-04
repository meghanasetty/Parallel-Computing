#include <omp.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>

#define CUTOFF 1000
#ifdef __cplusplus
extern "C" {
#endif

  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);

#ifdef __cplusplus
}
#endif

void merge(int *arr,int l,int m,int r,int *temp);
void mergeSort(int *arr,int l,int r,int *temp);
void mergeSort_seq(int *arr,int l,int r,int *temp);

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
 // printf("came here\n");
  int n = atoi(argv[1]);
  int nbthreads = atoi(argv[2]);
//  printf("n is %d\n",n);
  // get arr data
  int * arr = new int [n];
  int *temp = new int[n];
  generateMergeSortData (arr, n);

  //printf("Unsorted data:%d\n",n);
  //for(int i =0;i<n;i++)
//	  std::cout << arr[i] << " ";

//set the number of threads
   omp_set_num_threads(nbthreads);

// begin timing
   chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();

  //insert sorting code here.
#pragma omp parallel
{
	#pragma omp single
	{
	  mergeSort(arr,0,n-1,temp);
	}
}
  chrono::time_point<chrono::system_clock> end = chrono::system_clock::now();
  chrono::duration<double> elpased_seconds = end-start;

  // display time to cerr
  cerr<<elpased_seconds.count()<<endl;
  
//  printf("Sorted data:\n");
//  for(int i =0;i<n;i++)
//	  std::cout << arr[i] << " ";
  checkMergeSortResult (arr, n);
  
  delete[] arr;
  return 0;
}

void mergeSort(int *arr,int l,int r,int *temp)
{
	if((r-l) <1) 
		return;
	if((r-l) < CUTOFF)	
	{
		mergeSort_seq(arr,l,r,temp);
	}
	else
	{
		int m = (l+r)/2;
		#pragma omp task
			mergeSort(arr,l,m,temp);
		#pragma omp task
			mergeSort(arr,m+1,r,temp);
		#pragma omp taskwait
		merge(arr,l,m+1,r,temp);
	}
	
}

void mergeSort_seq(int *arr,int l,int r,int *temp)
{
	if((r-l)<1)
		return;
	int m = (l+r)/2;
	mergeSort_seq(arr,l,m,temp);
	mergeSort_seq(arr,m+1,r,temp);
	merge(arr,l,m+1,r,temp);
}
void merge(int *arr,int l,int m,int r,int *temparr)
{
  if (l == r) return;
  if (r-l == 1) {
	
    if (arr[l] > arr[r]) {
      int temp = arr[l];
      arr[l] = arr[r];
      arr[r] = temp;
    }
    return;
  }


  int i, j, k;
  int n = m - l;

  // declare and init temp arrays
  for (i=0; i<n; ++i)
  	temparr[l+i] = arr[l+i];
  i = 0;    // temp left half
  j = m;  // right half
  k = l;    // write to

  // merge
	while (i<n && j<=r) {
     		if (temparr[l+i] <= arr[j] ) {
       			arr[k++] = temparr[l+(i++)];
     		} else {
       			arr[k++] = arr[j++];
     		}
  	}

  // exhaust temp
	while (i<n) {
    		arr[k++] = temparr[l+(i++)];
  	}
}
