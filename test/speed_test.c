#include "ddot.h"
#include "fast_ddot.h"
#include "check_error.h"
#include "check_speed.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NDIM (1024*1024*8)
#define NFLOP (NDIM*2)

int main( int argc, char** argv ){

	test_t original = { ddot, NDIM, 0e0 }; 
	test_t tuned    = { fast_ddot, NDIM, 0e0 }; 

	double dt1,dt2;
	dt1 = check_speed( &original );
	dt2 = check_speed( &tuned    );

	int error=0;
	error = check_error( original.result,  tuned.result, NAN );

	printf("ELAPS TIME %zu entries : ddot %G sec ( %G FLOP/s ), fast_ddot %G sec ( %G FLOP/s )\n", 
		(size_t)(NDIM), dt1, NFLOP/dt1, dt2, NFLOP/dt2 );

	return 0;
}

