#include <stdlib.h>

#define ALIGN_SIZE  32
#define ALIGN_CHECK 0x1f // 00001111

double fast_ddot(size_t n, const double* x, const double* y)
{
	/*
	    xalign : boundary pattern
	      1)     aligned -> Lower 4 bits ......0000
	      2) NOT aligned -> Lower 4 bits ......1011 etc.

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

	/********************************************************
	   ALIGNMENT CASE 0
	********************************************************/

	if( !align ){
		//ymm12 = 0e0;
		//ymm13 = 0e0;
		//ymm14 = 0e0;
		//ymm15 = 0e0;
		__asm__ __volatile__(
			"vpxor %%ymm12, %%ymm12, %%ymm12 \n\t"
			"vpxor %%ymm13, %%ymm13, %%ymm13 \n\t"
			"vpxor %%ymm14, %%ymm14, %%ymm14 \n\t"
			"vpxor %%ymm15, %%ymm15, %%ymm15 \n\t"
			::);

		n_unroll = (n>>4);  // Unroll by 16 elements

		__asm__ __volatile__(
			"\n\t"
			"vmovapd   0*8(%[x]), %%ymm0 \n\t"
			"vmovapd   4*8(%[x]), %%ymm1 \n\t"
			"vmovapd   8*8(%[x]), %%ymm2 \n\t"
			"vmovapd  12*8(%[x]), %%ymm3 \n\t"
			"vmovapd   0*8(%[y]), %%ymm4 \n\t"
			"vmovapd   4*8(%[y]), %%ymm5 \n\t"
			"vmovapd   8*8(%[y]), %%ymm6 \n\t"
			"vmovapd  12*8(%[y]), %%ymm7 \n\t"
			"\n\t"
			"subq $-16*8, %[x]\n\t"
			"subq $-16*8, %[y]\n\t"
			"\n\t"
			:[x]"=r"(x),[y]"=r"(y)
			:"0"(x),"1"(y)
			);

		n_unroll--;
		while( n_unroll-- ){ 
			//ymm0  = *(x  ); // Load
			//ymm1  = *(x+1); // Load
			//ymm2  = *(x+2); // Load
			//ymm3  = *(x+3); // Load
			//ymm4  = *(y  ); // Load
			//ymm5  = *(y+1); // Load
			//ymm6  = *(y+2); // Load
			//ymm7  = *(y+3); // Load
			//ymm8  = ymm0  * ymm4; // Mul
			//ymm9  = ymm1  * ymm5; // Mul
			//ymm10 = ymm2  * ymm6; // Mul
			//ymm11 = ymm3  * ymm7; // Mul
			//ymm12 = ymm12 + ymm8; // Add
			//ymm13 = ymm13 + ymm9; // Add
			//ymm14 = ymm14 + ymm10;// Add
			//ymm15 = ymm15 + ymm11;// Add
			//x+=16;
			//y+=16;
			__asm__ __volatile__(
				"\n\t"
				"vmulpd      %%ymm0 , %%ymm4 , %%ymm8 \n\t"
				"vmulpd      %%ymm1 , %%ymm5 , %%ymm9 \n\t"
				"vmulpd      %%ymm2 , %%ymm6 , %%ymm10\n\t"
				"vmulpd      %%ymm3 , %%ymm7 , %%ymm11\n\t"
				"vaddpd      %%ymm8 , %%ymm12, %%ymm12\n\t"
				"vaddpd      %%ymm9 , %%ymm13, %%ymm13\n\t"
				"vaddpd      %%ymm10, %%ymm14, %%ymm14\n\t"
				"vaddpd      %%ymm11, %%ymm15, %%ymm15\n\t"
				"vmovapd   0*8(%[x]), %%ymm0 \n\t"
				"vmovapd   4*8(%[x]), %%ymm1 \n\t"
				"vmovapd   8*8(%[x]), %%ymm2 \n\t"
				"vmovapd  12*8(%[x]), %%ymm3 \n\t"
				"vmovapd   0*8(%[y]), %%ymm4 \n\t"
				"vmovapd   4*8(%[y]), %%ymm5 \n\t"
				"vmovapd   8*8(%[y]), %%ymm6 \n\t"
				"vmovapd  12*8(%[y]), %%ymm7 \n\t"
				"\n\t"
				"subq $-16*8, %[x]\n\t"
				"subq $-16*8, %[y]\n\t"
				"\n\t"
				:[x]"=r"(x),[y]"=r"(y)
				:"0"(x),"1"(y)
				);

		}
		__asm__ __volatile__(
			"\n\t"
			"vmulpd      %%ymm0 , %%ymm4 , %%ymm8 \n\t"
			"vmulpd      %%ymm1 , %%ymm5 , %%ymm9 \n\t"
			"vmulpd      %%ymm2 , %%ymm6 , %%ymm10\n\t"
			"vmulpd      %%ymm3 , %%ymm7 , %%ymm11\n\t"
			"vaddpd      %%ymm8 , %%ymm12, %%ymm12\n\t"
			"vaddpd      %%ymm9 , %%ymm13, %%ymm13\n\t"
			"vaddpd      %%ymm10, %%ymm14, %%ymm14\n\t"
			"vaddpd      %%ymm11, %%ymm15, %%ymm15\n\t"
			"\n\t"
			::);


		if( n & 8 ){
			//ymm0  = *(x  ); // Load
			//ymm1  = *(x+1); // Load
			//ymm4  = *(y  ); // Load
			//ymm5  = *(y+1); // Load
			//ymm8  = ymm0  * ymm4; // Mul
			//ymm9  = ymm1  * ymm5; // Mul
			//ymm12 = ymm12 + ymm8; // Add
			//ymm13 = ymm13 + ymm9; // Add
			//x+=8;
			//y+=8;
			__asm__ __volatile__(
				"\n\t"
				"vmovapd   0*8(%[x]), %%ymm0 \n\t"
				"vmovapd   4*8(%[x]), %%ymm1 \n\t"
				"vmovapd   0*8(%[y]), %%ymm4 \n\t"
				"vmovapd   4*8(%[y]), %%ymm5 \n\t"
				"vmulpd      %%ymm0 , %%ymm4 , %%ymm8 \n\t"
				"vmulpd      %%ymm1 , %%ymm5 , %%ymm9 \n\t"
				"vaddpd      %%ymm8 , %%ymm12, %%ymm12\n\t"
				"vaddpd      %%ymm9 , %%ymm13, %%ymm13\n\t"
				"\n\t"
				"subq $-8*8, %[x]\n\t"
				"subq $-8*8, %[y]\n\t"
				"\n\t"
				:[x]"=r"(x),[y]"=r"(y)
				:"0"(x),"1"(y)
				);
		}
		if( n & 4 ){
			//ymm0  = *x; // Load
			//ymm4  = *y; // Load
			//ymm8  = ymm0  * ymm4; // Mul
			//ymm12 = ymm12 + ymm8; // Add
			//x+=4;
			//y+=4;
			__asm__ __volatile__(
				"\n\t"
				"vmovapd   0*8(%[x]), %%ymm0 \n\t"
				"vmovapd   0*8(%[y]), %%ymm4 \n\t"
				"vmulpd      %%ymm0 , %%ymm4 , %%ymm8 \n\t"
				"vaddpd      %%ymm8 , %%ymm12, %%ymm12\n\t"
				"\n\t"
				"subq $-4*8, %[x]\n\t"
				"subq $-4*8, %[y]\n\t"
				"\n\t"
				:[x]"=r"(x),[y]"=r"(y)
				:"0"(x),"1"(y)
				);
		}
		if( n & 2 ){
			//ymm0  = *x; // Load
			//ymm4  = *y; // Load
			//ymm8  = ymm0  * ymm4; // Mul
			//ymm12 = ymm12 + ymm8; // Add
			//x+=2;
			//y+=2;
			__asm__ __volatile__(
				"\n\t"
				"movapd   0*8(%[x]), %%xmm0 \n\t"
				"movapd   0*8(%[y]), %%xmm4 \n\t"
				"mulpd      %%xmm0 , %%xmm4 \n\t"
				"addpd      %%xmm4 , %%xmm12\n\t"
				"\n\t"
				"subq $-2*8, %[x]\n\t"
				"subq $-2*8, %[y]\n\t"
				"\n\t"
				:[x]"=r"(x),[y]"=r"(y)
				:"0"(x),"1"(y)
				);
		}
		if( n & 1 ){
			//ymm0  = *x; // Load
			//ymm4  = *y; // Load
			//ymm8  = ymm0  * ymm4; // Mul
			//ymm12 = ymm12 + ymm8; // Add
			//x++;
			//y++;
			__asm__ __volatile__(
				"\n\t"
				"movsd    0*8(%[x]), %%xmm0 \n\t"
				"movsd    0*8(%[y]), %%xmm4 \n\t"
				"mulsd      %%xmm0 , %%xmm4 \n\t"
				"addsd      %%xmm4 , %%xmm12\n\t"
				"\n\t"
				"subq $-1*8, %[x]\n\t"
				"subq $-1*8, %[y]\n\t"
				"\n\t"
				:[x]"=r"(x),[y]"=r"(y)
				:"0"(x),"1"(y)
				);

		}
		//value = ymm12 + ymm13 + ymm14 + ymm15;
		__asm__ __volatile__(
			"\n\t"
			"vaddpd            %%ymm12, %%ymm13, %%ymm13\n\t"
			"vaddpd            %%ymm14, %%ymm15, %%ymm15\n\t"
			"vaddpd            %%ymm13, %%ymm15, %%ymm15\n\t"
			"vperm2f128 $0x01, %%ymm15, %%ymm15, %%ymm14\n\t" // exchange |a|b|c|d| -> |c|d|a|b|
			"vhaddpd           %%ymm14, %%ymm15, %%ymm15\n\t"
			"vhaddpd           %%ymm15, %%ymm15, %%ymm15\n\t"
			"movsd             %%xmm15, %[v] \n\t"
			"\n\t"
			:[v]"=m"(value)
			);


	/********************************************************
	   ALIGNMENT CASE 1, 2, 3
	********************************************************/

	}else{
		//ymm12 = 0e0;
		//ymm13 = 0e0;
		//ymm14 = 0e0;
		//ymm15 = 0e0;
		__asm__ __volatile__(
			"vpxor %%ymm12, %%ymm12, %%ymm12 \n\t"
			"vpxor %%ymm13, %%ymm13, %%ymm13 \n\t"
			"vpxor %%ymm14, %%ymm14, %%ymm14 \n\t"
			"vpxor %%ymm15, %%ymm15, %%ymm15 \n\t"
			::);

		n_unroll = (n>>4);  // Unroll by 16 elements

		__asm__ __volatile__(
			"\n\t"
			"vmovupd   0*8(%[x]), %%ymm0 \n\t"
			"vmovupd   4*8(%[x]), %%ymm1 \n\t"
			"vmovupd   8*8(%[x]), %%ymm2 \n\t"
			"vmovupd  12*8(%[x]), %%ymm3 \n\t"
			"vmovupd   0*8(%[y]), %%ymm4 \n\t"
			"vmovupd   4*8(%[y]), %%ymm5 \n\t"
			"vmovupd   8*8(%[y]), %%ymm6 \n\t"
			"vmovupd  12*8(%[y]), %%ymm7 \n\t"
			"\n\t"
			"subq $-16*8, %[x]\n\t"
			"subq $-16*8, %[y]\n\t"
			"\n\t"
			:[x]"=r"(x),[y]"=r"(y)
			:"0"(x),"1"(y)
			);

		n_unroll--;
		while( n_unroll-- ){ 
			//ymm0  = *(x  ); // Load
			//ymm1  = *(x+1); // Load
			//ymm2  = *(x+2); // Load
			//ymm3  = *(x+3); // Load
			//ymm4  = *(y  ); // Load
			//ymm5  = *(y+1); // Load
			//ymm6  = *(y+2); // Load
			//ymm7  = *(y+3); // Load
			//ymm8  = ymm0  * ymm4; // Mul
			//ymm9  = ymm1  * ymm5; // Mul
			//ymm10 = ymm2  * ymm6; // Mul
			//ymm11 = ymm3  * ymm7; // Mul
			//ymm12 = ymm12 + ymm8; // Add
			//ymm13 = ymm13 + ymm9; // Add
			//ymm14 = ymm14 + ymm10;// Add
			//ymm15 = ymm15 + ymm11;// Add
			//x+=16;
			//y+=16;
			__asm__ __volatile__(
				"\n\t"
				"vmulpd      %%ymm0 , %%ymm4 , %%ymm8 \n\t"
				"vmulpd      %%ymm1 , %%ymm5 , %%ymm9 \n\t"
				"vmulpd      %%ymm2 , %%ymm6 , %%ymm10\n\t"
				"vmulpd      %%ymm3 , %%ymm7 , %%ymm11\n\t"
				"vaddpd      %%ymm8 , %%ymm12, %%ymm12\n\t"
				"vaddpd      %%ymm9 , %%ymm13, %%ymm13\n\t"
				"vaddpd      %%ymm10, %%ymm14, %%ymm14\n\t"
				"vaddpd      %%ymm11, %%ymm15, %%ymm15\n\t"
				"vmovupd   0*8(%[x]), %%ymm0 \n\t"
				"vmovupd   4*8(%[x]), %%ymm1 \n\t"
				"vmovupd   8*8(%[x]), %%ymm2 \n\t"
				"vmovupd  12*8(%[x]), %%ymm3 \n\t"
				"vmovupd   0*8(%[y]), %%ymm4 \n\t"
				"vmovupd   4*8(%[y]), %%ymm5 \n\t"
				"vmovupd   8*8(%[y]), %%ymm6 \n\t"
				"vmovupd  12*8(%[y]), %%ymm7 \n\t"
				"\n\t"
				"subq $-16*8, %[x]\n\t"
				"subq $-16*8, %[y]\n\t"
				"\n\t"
				:[x]"=r"(x),[y]"=r"(y)
				:"0"(x),"1"(y)
				);

		}
		__asm__ __volatile__(
			"\n\t"
			"vmulpd      %%ymm0 , %%ymm4 , %%ymm8 \n\t"
			"vmulpd      %%ymm1 , %%ymm5 , %%ymm9 \n\t"
			"vmulpd      %%ymm2 , %%ymm6 , %%ymm10\n\t"
			"vmulpd      %%ymm3 , %%ymm7 , %%ymm11\n\t"
			"vaddpd      %%ymm8 , %%ymm12, %%ymm12\n\t"
			"vaddpd      %%ymm9 , %%ymm13, %%ymm13\n\t"
			"vaddpd      %%ymm10, %%ymm14, %%ymm14\n\t"
			"vaddpd      %%ymm11, %%ymm15, %%ymm15\n\t"
			"\n\t"
			::);


		if( n & 8 ){
			//ymm0  = *(x  ); // Load
			//ymm1  = *(x+1); // Load
			//ymm4  = *(y  ); // Load
			//ymm5  = *(y+1); // Load
			//ymm8  = ymm0  * ymm4; // Mul
			//ymm9  = ymm1  * ymm5; // Mul
			//ymm12 = ymm12 + ymm8; // Add
			//ymm13 = ymm13 + ymm9; // Add
			//x+=8;
			//y+=8;
			__asm__ __volatile__(
				"\n\t"
				"vmovupd   0*8(%[x]), %%ymm0 \n\t"
				"vmovupd   4*8(%[x]), %%ymm1 \n\t"
				"vmovupd   0*8(%[y]), %%ymm4 \n\t"
				"vmovupd   4*8(%[y]), %%ymm5 \n\t"
				"vmulpd      %%ymm0 , %%ymm4 , %%ymm8 \n\t"
				"vmulpd      %%ymm1 , %%ymm5 , %%ymm9 \n\t"
				"vaddpd      %%ymm8 , %%ymm12, %%ymm12\n\t"
				"vaddpd      %%ymm9 , %%ymm13, %%ymm13\n\t"
				"\n\t"
				"subq $-8*8, %[x]\n\t"
				"subq $-8*8, %[y]\n\t"
				"\n\t"
				:[x]"=r"(x),[y]"=r"(y)
				:"0"(x),"1"(y)
				);
		}
		if( n & 4 ){
			//ymm0  = *x; // Load
			//ymm4  = *y; // Load
			//ymm8  = ymm0  * ymm4; // Mul
			//ymm12 = ymm12 + ymm8; // Add
			//x+=4;
			//y+=4;
			__asm__ __volatile__(
				"\n\t"
				"vmovupd   0*8(%[x]), %%ymm0 \n\t"
				"vmovupd   0*8(%[y]), %%ymm4 \n\t"
				"vmulpd      %%ymm0 , %%ymm4 , %%ymm8 \n\t"
				"vaddpd      %%ymm8 , %%ymm12, %%ymm12\n\t"
				"\n\t"
				"subq $-4*8, %[x]\n\t"
				"subq $-4*8, %[y]\n\t"
				"\n\t"
				:[x]"=r"(x),[y]"=r"(y)
				:"0"(x),"1"(y)
				);
		}
		if( n & 2 ){
			//ymm0  = *x; // Load
			//ymm4  = *y; // Load
			//ymm8  = ymm0  * ymm4; // Mul
			//ymm12 = ymm12 + ymm8; // Add
			//x+=2;
			//y+=2;
			__asm__ __volatile__(
				"\n\t"
				"movupd   0*8(%[x]), %%xmm0 \n\t"
				"movupd   0*8(%[y]), %%xmm4 \n\t"
				"mulpd      %%xmm0 , %%xmm4 \n\t"
				"addpd      %%xmm4 , %%xmm12\n\t"
				"\n\t"
				"subq $-2*8, %[x]\n\t"
				"subq $-2*8, %[y]\n\t"
				"\n\t"
				:[x]"=r"(x),[y]"=r"(y)
				:"0"(x),"1"(y)
				);
		}
		if( n & 1 ){
			//ymm0  = *x; // Load
			//ymm4  = *y; // Load
			//ymm8  = ymm0  * ymm4; // Mul
			//ymm12 = ymm12 + ymm8; // Add
			//x++;
			//y++;
			__asm__ __volatile__(
				"\n\t"
				"movsd    0*8(%[x]), %%xmm0 \n\t"
				"movsd    0*8(%[y]), %%xmm4 \n\t"
				"mulsd      %%xmm0 , %%xmm4 \n\t"
				"addsd      %%xmm4 , %%xmm12\n\t"
				"\n\t"
				"subq $-1*8, %[x]\n\t"
				"subq $-1*8, %[y]\n\t"
				"\n\t"
				:[x]"=r"(x),[y]"=r"(y)
				:"0"(x),"1"(y)
				);

		}
		//value = ymm12 + ymm13 + ymm14 + ymm15;
		__asm__ __volatile__(
			"\n\t"
			"vaddpd            %%ymm12, %%ymm13, %%ymm13\n\t"
			"vaddpd            %%ymm14, %%ymm15, %%ymm15\n\t"
			"vaddpd            %%ymm13, %%ymm15, %%ymm15\n\t"
			"vperm2f128 $0x01, %%ymm15, %%ymm15, %%ymm14\n\t" // exchange |a|b|c|d| -> |c|d|a|b|
			"vhaddpd           %%ymm14, %%ymm15, %%ymm15\n\t"
			"vhaddpd           %%ymm15, %%ymm15, %%ymm15\n\t"
			"movsd             %%xmm15, %[v] \n\t"
			"\n\t"
			:[v]"=m"(value)
			);


	}
	return value;
}
