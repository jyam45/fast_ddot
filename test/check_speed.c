#include "check_speed.h"
#include "Timer.h"

#define OVERFLOW_GUIRD 1024*1024

double check_speed( test_t* test ){

	double *x,*y;
	x = (double*) calloc( test->nvec, sizeof(double) );
	y = (double*) calloc( test->nvec, sizeof(double) );

	/* init */
	for( int i=0; i<test->nvec; i++ ){
		x[i] = (double)(i%OVERFLOW_GUIRD);
		y[i] = (double)(i%OVERFLOW_GUIRD);
	}

	/* speed test */
	double t1,t2;
	t1 = get_realtime();
	test->result = test->func( test->nvec, x, y );
	t2 = get_realtime();

	free(x);
	free(y);

	return t2-t1;
}
 
