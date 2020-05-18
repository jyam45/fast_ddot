#include "fast_ddot.h"

double fast_ddot(size_t n, const double* x, const double* y)
{
	double value = 0e0;
	while( n-- ){ value += (*(x++))*(*(y++)); }
	return value;
}
