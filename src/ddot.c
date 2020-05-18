#include "ddot.h"

double ddot(size_t n, const double* x, const double* y)
{
	double value = 0e0;
	while( n-- ){ value += (*(x++))*(*(y++)); }
	return value;
}
