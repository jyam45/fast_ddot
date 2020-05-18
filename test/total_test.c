#include "ddot.h"
#include "fast_ddot.h"
#include "check_error.h"
#include "check_speed.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAXDIM 128*1024*1024

int main( int argc, char** argv ){

	test_t original = { ddot, 0, 0e0 }; 
	test_t tuned    = { fast_ddot, 0, 0e0 }; 

	double dt1,dt2;
	size_t nflop;

	int error=0;

	printf("NVec, ddot time [sec], fast_ddot time [sec], ddot pfmc [FLOP/s], fast_ddot pfmc [FLOP/s], speed-up ratio\n" );

	for( size_t ndim = 16; ndim < MAXDIM; ndim=ndim*2 ){

		original.nvec = ndim;
		tuned.nvec    = ndim;

		dt1 = check_speed( &original );
		dt2 = check_speed( &tuned    );

		error = check_error( original.result,  tuned.result, NAN );
		if( error ){ break; }

		nflop = 2 * ndim;
		printf("%zu, %G, %G, %G, %G, %G\n", ndim, dt1,dt2, nflop/dt1, nflop/dt2, dt1/dt2 );

	}

	return 0;
}

