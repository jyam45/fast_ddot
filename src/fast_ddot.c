#include <stdlib.h>

#define ALIGN_SIZE  32
#define ALIGN_CHECK 0x1f // 00001111

double fast_ddot(size_t n, const double* x, const double* y)
{
	/*
	    xalign : boundary pattern
	      1)     aligned -> Lower 4 bits ......0000
	      2) NOT aligned -> Lower 4 bits ......1011

	    align : alignment aase number
	      0 = y     aligned and x     aligned
	      1 = y     aligned and x not aligned
	      2 = y not aligned and x     aligned
	      3 = y not aligned and x not aligned
	*/
	uint64_t xalign = ((uint64_t)x) & ALIGN_CHECK;
	uint64_t yalign = ((uint64_t)y) & ALIGN_CHECK;
	uint64_t align  = ((((yalign>0)?1:0)<<1) | ((xalign>0)?1:0));

	double value = 0e0;
	if( !align ){
	/* aligiment case 0 */
		while( n-- ){ value += (*(x++))*(*(y++)); }
	}else{
	/* aligiment case 1,2 or 3 */
		while( n-- ){ value += (*(x++))*(*(y++)); }
	}
	return value;
}
