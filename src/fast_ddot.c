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

	/*
	    YMM regiser variables
	*/
	double ymm0,ymm1,ymm2,ymm3;
	double ymm4,ymm5,ymm6,ymm7;
	double ymm8,ymm9,ymm10,ymm11;
	double ymm12,ymm13,ymm14,ymm15;

	/*
	    Unrolled Indeces
	*/
	size_t n_unroll;

	double value = 0e0;
	if( !align ){
		/* aligiment case 0 */
		ymm12 = 0e0;
		ymm13 = 0e0;
		ymm14 = 0e0;
		ymm15 = 0e0;
		n_unroll = (n>>2);  // Unroll by 4 elements
		while( n_unroll-- ){ 
			ymm0  = *(x  ); // Load
			ymm1  = *(x+1); // Load
			ymm2  = *(x+2); // Load
			ymm3  = *(x+3); // Load
			ymm4  = *(y  ); // Load
			ymm5  = *(y+1); // Load
			ymm6  = *(y+2); // Load
			ymm7  = *(y+3); // Load
			ymm8  = ymm0  * ymm4; // Mul
			ymm9  = ymm1  * ymm5; // Mul
			ymm10 = ymm2  * ymm6; // Mul
			ymm11 = ymm3  * ymm7; // Mul
			ymm12 = ymm12 + ymm8; // Add
			ymm13 = ymm13 + ymm9; // Add
			ymm14 = ymm14 + ymm10;// Add
			ymm15 = ymm15 + ymm11;// Add
			x+=4;
			y+=4;
		}
		value = ymm12 + ymm13 + ymm14 + ymm15;
		ymm12 = 0e0;
		ymm13 = 0e0;
		if( n & 2 ){
			ymm0  = *(x  ); // Load
			ymm1  = *(x+1); // Load
			ymm4  = *(y  ); // Load
			ymm5  = *(y+1); // Load
			ymm8  = ymm0  * ymm4; // Mul
			ymm9  = ymm1  * ymm5; // Mul
			ymm12 = ymm12 + ymm8; // Add
			ymm13 = ymm13 + ymm9; // Add
			x+=2;
			y+=2;
		}
		value = value + ymm12 + ymm13;
		ymm12 = 0e0;
		if( n & 1 ){
			ymm0  = *x; // Load
			ymm4  = *y; // Load
			ymm8  = ymm0  * ymm4; // Mul
			ymm12 = ymm12 + ymm8; // Add
			x++;
			y++;
		}
		value = value + ymm12;
		//value = ymm12 + ymm13 + ymm14 + ymm15;
		//value = ymm12;
	}else{
		/* aligiment case 1,2 or 3 */
		ymm12 = 0e0;
		ymm13 = 0e0;
		ymm14 = 0e0;
		ymm15 = 0e0;
		n_unroll = (n>>2);  // Unroll by 4 elements
		while( n_unroll-- ){ 
			ymm0  = *(x  ); // Load
			ymm1  = *(x+1); // Load
			ymm2  = *(x+2); // Load
			ymm3  = *(x+3); // Load
			ymm4  = *(y  ); // Load
			ymm5  = *(y+1); // Load
			ymm6  = *(y+2); // Load
			ymm7  = *(y+3); // Load
			ymm8  = ymm0  * ymm4; // Mul
			ymm9  = ymm1  * ymm5; // Mul
			ymm10 = ymm2  * ymm6; // Mul
			ymm11 = ymm3  * ymm7; // Mul
			ymm12 = ymm12 + ymm8; // Add
			ymm13 = ymm13 + ymm9; // Add
			ymm14 = ymm14 + ymm10;// Add
			ymm15 = ymm15 + ymm11;// Add
			x+=4;
			y+=4;
		}
		value = ymm12 + ymm13 + ymm14 + ymm15;
		ymm12 = 0e0;
		ymm13 = 0e0;
		if( n & 2 ){
			ymm0  = *(x  ); // Load
			ymm1  = *(x+1); // Load
			ymm4  = *(y  ); // Load
			ymm5  = *(y+1); // Load
			ymm8  = ymm0  * ymm4; // Mul
			ymm9  = ymm1  * ymm5; // Mul
			ymm12 = ymm12 + ymm8; // Add
			ymm13 = ymm13 + ymm9; // Add
			x+=2;
			y+=2;
		}
		value = value + ymm12 + ymm13;
		ymm12 = 0e0;
		if( n & 1 ){
			ymm0  = *x; // Load
			ymm4  = *y; // Load
			ymm8  = ymm0  * ymm4; // Mul
			ymm12 = ymm12 + ymm8; // Add
			x++;
			y++;
		}
		value = value + ymm12;
		//value = ymm12 + ymm13 + ymm14 + ymm15;
		//value = ymm12;
	}
	return value;
}
