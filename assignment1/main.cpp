/***
 * Program to find the numercial integration of f(x)dx
 * Formula: Integration of f(x)dx ~ ((b-a)/n)*summation i= 0 to n-1 of f(a+((i+0.5)*(b-a)/n))
 *
 * Input:
 * 	funtionId = integer to integrate which function we want (from libfunctions)
 * 	a = lower bound of the integral
 * 	b = higher bound of the integral
 * 	n = an integer which is the number of points to compute the approx of the integral
 * 	intensity = operation intensity
 *
 * Output:
 * 	stdout = the value of the integral
 * 	stderr = the time taken to compute the integral in seconds
 *
 */
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <chrono>
#include <ctime>
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

  int start_s = clock();
  if (argc < 6) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity>"<<std::endl;
    return -1;
  }

  int functionid = -1;
  float a,b,n,interval;
  float intensity;
  double sumOfFun = 0,res = 0;
  //get the input values
  sscanf(argv[1],"%d",&functionid);
  sscanf(argv[2],"%f",&a);
  sscanf(argv[3],"%f",&b);
  sscanf(argv[4],"%f",&n);
  sscanf(argv[5],"%f",&intensity);
  // find the intervals
  interval = (b-a)/n;
  //Solution
  for(int i=0;i<n;i++)
  {
	float funVal = 0;
	float funParam1 = a+(i+0.5)*interval;
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
  	sumOfFun += funVal;;
  }
  res = interval * sumOfFun;
  cout << res ;
  int stop_s = clock();
  double time_exec = (stop_s - start_s)/double(CLOCKS_PER_SEC);
  cerr << time_exec;
  return 0;
}
