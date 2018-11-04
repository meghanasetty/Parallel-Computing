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

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif
int lcs(char *X,int m,char *Y,int n,int **comp);
int index_matrix(int x,int y, int m);
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

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int nbthreads = atoi(argv[3]);
  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);
 // int *comp = new int[(m+1)*(n+1)];
 	int **comp = new int*[m+1];
	for (int i=0;i<=m;i++)
		comp[i] = new int[n+1]; 
 //insert LCS code here.
  int result = -1; // length of common subsequence
  omp_set_num_threads(nbthreads);
  //printf("X = %s\n",X);
  //printf("Y = %s\n",Y);
chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();
#pragma omp parallel for
	for(int i = 0;i<=m;i++)
	{
		//comp[index_matrix(i,0,m+1)] = 0;
		comp[i][0] = 0;
	}

#pragma omp parallel for
	for(int j = 0;j<=n;j++)
	{
		//comp[index_matrix(0,j,m+1)] = 0;
		comp[0][j] = 0;
	}
  
	result = lcs(X,m,Y,n,comp);


chrono::time_point<std::chrono::system_clock> end = chrono::system_clock::now();
  chrono::duration<double> elpased_seconds = end-start;
cerr<<elpased_seconds.count()<<endl;
//printf("result %d\n",result);
  checkLCS(X, m, Y, n, result);

delete []X;
delete []Y;
delete []comp;

  return 0;
}


int lcs(char *X,int m,char *Y,int n,int **comp)
{
	for(int i=1;i<=m;i++){
		#pragma omp parallel for 
		for(int j = 1;j<=n;j++)
		{
			if(X[i-1] == Y[j-1])
				comp[i][j] = comp[i-1][j-1] +1;
			else
				comp[i][j] = max(comp[i-1][j],comp[i][j-1]);
		}
	}
     		
	return comp[m][n];
}
int index_matrix(int x,int y,int m)
{
	return (x*m)+y;
}
