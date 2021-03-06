#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#include "ranlxd.h"

void ranlxd(double r[ ],int n);

// Limits of the integral
static const double a = 0.0;
static const double b = 3.0;

static const double ACTUAL_RESULT = 0.873109;

// f(s) = (e^-s) / (1 + (s/9))
static double func(double s){
	double temp1 = pow(M_E, -s);
	double temp2 = 1 + (s / 9);
	return (temp1 / temp2);
}

// Adjust the operations so that random numbers in interval [0,1)
// can be used to estimate integral from 0 to 3.
// See Simulation by Sheldon Ross, page 39
static double func_adjusted(double rv){
	double res = func(a + ((b - a) * rv));
	return res * (b-a);
}


// The number of random numbers to be used in each estimation
#define NUM_SIZES 5
static const int SIZES[NUM_SIZES] = { 100, 1000, 10000, 100000, 1000000 };

int main(void){
	int i;
	for(i  = 0; i < NUM_SIZES; i++){
		int n = SIZES[i];
		double *rn = malloc(sizeof(double) * n);
		struct timeval tv;
		gettimeofday(&tv, NULL);
		rlxd_init(1, tv.tv_usec); 
		ranlxd(rn, n);
		double result = 0.0;
		int k;
		for(k = 0; k < n; k++){
			result = result + ((func_adjusted(rn[k])) / n);
		}
		printf("n: %d, result: %lf, absolute error: %lf\n", n, result, fabs(result - ACTUAL_RESULT));
		free(rn);
	}
	return 0;
}

