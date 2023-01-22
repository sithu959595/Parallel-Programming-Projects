#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <xmmintrin.h>

#ifndef NUMT
#define NUMT		4
#endif

#ifndef NUMTRIES
#define NUMTRIES	10
#endif

#ifndef ARRAYS
#define ARRAYS      6
#endif

#define SSE_WIDTH		4
#define NUM_ELEMENTS_PER_CORE     ARRAYS/NUMT




float SimdMulSum( float *, float *, int);






	int
main( int argc, char *argv[ ] )
{
#ifndef _OPENMP
	fprintf( stderr, "No OpenMP support!\n" );
	return 1;
#endif


	float *a=new float[ARRAYS];
	float *b=new float[ARRAYS];
	float *c=new float[ARRAYS];
	float r=0.;

	for(int n=0;n<ARRAYS;n++){
		a[n]=(float)n;
		b[n]=(float)n;
		c[n]=0.;
	}


//C++
double r1=0.;
	for(int i=0;i<ARRAYS;i++){
		r1+=a[i]*b[i];
	}
	printf("*****r= %8.8lf \n",r1 );



//SIMD
	double result=SimdMulSum(a,b,ARRAYS );
	printf("******result= %8.8lf \n",result );
	printf("size= %d \n",NUM_ELEMENTS_PER_CORE );








//SIMD with Multithreads
	omp_set_num_threads( NUMT );
double sum=0.;
#pragma omp parallel reduction(+:sum)
{
int first = omp_get_thread_num( ) * NUM_ELEMENTS_PER_CORE;
sum=sum+SimdMulSum(&a[first],&b[first],NUM_ELEMENTS_PER_CORE);
printf("result from %d is %8.8lf \n",omp_get_thread_num(),sum );
}
printf("just= %8.8lf \n",sum);

int even=NUM_ELEMENTS_PER_CORE*NUMT;
int che=ARRAYS-even;
if(che!=0){
for(int u=even;u<ARRAYS;u++){
//printf("add=%8.8lf\n",a[u]*b[u]);
printf("before add=%8.8lf\n",sum);
sum+=a[u]*b[u];
printf("after add=%8.8lf\n",sum);
}
}

printf("*****result with Threads= %8.8lf \n",sum );






/*
//printf("A= %d \n",a );
printf("a[0]= %d \n",a[0] );
printf("a[2]= %d \n",a[2] );
printf("a[3]= %d \n",a[3] );
printf("a[2]= %8.8lf \n",a[2] );
printf("&a[2]= %d \n",&a[2] );
printf("&a[3]= %d \n",&a[3] );
printf("a[1]= %d \n",a[1] );
printf("a+3= %d \n",a+3 );
printf("a= %d \n",a );
printf("&a[0]= %d \n",&a[0] );
printf("a[2]= %8.8lf \n",a[2] );
//printf("C= %d \n",&a[0] );


//float D[4]={1,2,4,8};
//printf("A= %d \n",&a[0] );
//printf("B= %8.8lf \n",*(D+3) );
//printf("C= %d \n",D+3 );
//printf("D= %d \n",D );
//for(int n=0;n<4;n++){
//D[n]=n;
//}
//printf("A= %8.8lf \n",D[2] );
//printf("B= %8.8lf \n",D[3] );

*/









}//Main




float
SimdMulSum( float *a, float *b, int len )
{
	float sum[4] = { 0., 0., 0., 0. };
	int limit = ( len/SSE_WIDTH ) * SSE_WIDTH;
	register float *pa = a;
	register float *pb = b;

	__m128 ss = _mm_loadu_ps( &sum[0] );
	for( int i = 0; i < limit; i += SSE_WIDTH )
	{
		ss = _mm_add_ps( ss, _mm_mul_ps( _mm_loadu_ps( pa ), _mm_loadu_ps( pb ) ) );
		pa += SSE_WIDTH;
		pb += SSE_WIDTH;
	}
	_mm_storeu_ps( &sum[0], ss );

	for( int i = limit; i < len; i++ )
	{
		sum[0] += a[i] * b[i];
	}

	return sum[0] + sum[1] + sum[2] + sum[3];
}





