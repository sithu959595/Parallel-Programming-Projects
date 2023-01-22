
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>
#include <xmmintrin.h>

#ifndef NUMT
#define NUMT		1
#endif

#define NUMNODES        1000	// you decide
#define N              4
#define NUMTRIES              10
#define NUMT1              1

#ifndef NUMTRIES
#define NUMTRIES	2
#endif


#define SSE_WIDTH		4


float SimdMulSum( float *, float *, int ); 

int
main( int argc, char *argv[ ] )
{
#ifndef _OPENMP
	fprintf( stderr, "No OpenMP support!\n" );
	return 1;
#endif

FILE *fp = fopen( "signal.txt", "r" );
if( fp == NULL )
{
	fprintf( stderr, "Cannot open file 'signal.txt'\n" );
	exit( 1 );
}
int Size;
fscanf( fp, "%d", &Size );
float *A =     new float[ 2*Size ];
float *Sums  = new float[ 1*Size ];
for( int i = 0; i < Size; i++ )
{
	fscanf( fp, "%f", &A[i] );
	A[i+Size] = A[i];		// duplicate the array
}
fclose( fp );
//printf("Output = %d \n", Size);
//printf("Output = %8.6lf \n", A[0]);

float maxPerformance = 0.;
omp_set_num_threads( NUMT );



for( int t = 0; t <NUMTRIES; t++ )       //for NUMTRIES
{
                double time0 = omp_get_wtime( );



#pragma omp parallel for default(none) shared(Sums,A,Size)
for( int shift = 0; shift < Size; shift++ )
{

		Sums[shift] = SimdMulSum( &A[0], &A[0+shift], Size );
	
	
}//for Calculation

double time1 = omp_get_wtime( );
double megaTrialsPerSecond = ((double)Size*(double)Size) / ( time1 - time0 ) / 1000000.;
if( megaTrialsPerSecond > maxPerformance )
maxPerformance = megaTrialsPerSecond;

}// for NUMTRIES
printf("maxPerformance for %d Threads= %8.7lf \n", maxPerformance,NUMT);


/*  for(int i=0;i<512;i+=32){
printf("Output for index %d = %8.6lf \n",i, Sums[i]);
}  */

for(int i=1;i<513;i++){
printf("%8.6lf,", Sums[i]);
}
printf("\n");

}    //main







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
