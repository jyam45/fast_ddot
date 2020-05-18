#ifndef CHECK_SPEED_HEADER_GUIRD
#define CHECK_SPEED_HEADER_GUIRD

#include <stdlib.h>

typedef double (*func_t)( size_t, const double*, const double*);

typedef struct {
	func_t func;
	size_t nvec;
	double result;
} test_t;
 
extern double check_speed( test_t* test ); // return elapse time


#endif//CHECK_SPEED_HEADER_GUIRD

