#include "check_error.h"

#include <stdio.h>
#include <float.h>
#include <math.h>

/* a = original, b = tuned, no value check if corr=NAN */
int check_error( double a, double b, double corr ){

	double absa = fabs(a);
	double absb = fabs(b);
	double diff = fabs(absa-absb);;
	double dmax = (absa > absb ? absa : absb );

	int error = 0;

	if( !isnan(corr) ){
		if( fabs( a - corr ) > DBL_EPSILON * corr ){
			printf("Invalid Result : correct = %G, original = %G\n",corr,a);
			error++;
		}
		if( fabs( b - corr ) > DBL_EPSILON * corr ){
			printf("Invalid Result : correct = %G, tuned = %G\n",corr,b);
			error++;
		}
	}
	if( diff > DBL_EPSILON * dmax ){
		printf("Difference Error : original = %G, tuned = %G, diff = %G, eps = %G\n",a,b,diff,DBL_EPSILON*dmax);
		error++;
	}

	return error;
}

