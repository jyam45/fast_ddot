#include "ddot.h"
#include "fast_ddot.h"
#include "check_error.h"
#include <stdio.h>

#define VALUE  1e0
#define DIM    1023

int main( int argc, char** argv ){

	double x[DIM],y[DIM];

	for(int i=0; i<DIM; i++ ){
		x[i] = VALUE;
		y[i] = VALUE;
	}

	double a = ddot( DIM, x, y );
	double b = fast_ddot( DIM, x, y );

	int error = check_error(a,b,VALUE*VALUE*DIM);
	if( !error ){
		printf("OK : ddot = %G, fast_ddot = %G, diff = %G\n",a,b,a-b);
	} 

	return check_error(a,b,VALUE*VALUE*DIM);

}

