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
#define NUMT	         4
#endif

#ifndef NUMTRIES
#define NUMTRIES	15
#endif

#ifndef ARRAYS
#define ARRAYS         1000000
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


printf("----------------------------------------------------------\n");   
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
double maxPerformanceC = 0.;
 for( int t = 0; t < NUMTRIES; t++ ){
double timeC0 = omp_get_wtime( );
r1=0.;
	for(int i=0;i<ARRAYS;i++){
		r1+=a[i]*b[i];
	}
double timeC1 = omp_get_wtime( );
double exeC=timeC1-timeC0;
double performanceC=(double)ARRAYS/exeC/1000000.;
if( performanceC > maxPerformanceC )
maxPerformanceC = performanceC;    //Performance
}	
printf("*****result with C++= %8.8lf \n",r1 );
// End of C++


//SIMD alone


double result=0.;
double maxPerformanceS = 0.;
for( int t = 0; t < NUMTRIES; t++ ){
double timeS0 = omp_get_wtime( );
	result=SimdMulSum(a,b,ARRAYS );
double timeS1 = omp_get_wtime( );
double exeS=timeS1-timeS0;
double performanceS=(double)ARRAYS/exeS/1000000.;
if( performanceS > maxPerformanceS )
maxPerformanceS = performanceS;     //Performance


}

	printf("******result with SIMD= %8.8lf \n",result );
  

double S=maxPerformanceS/maxPerformanceC; //Speedup of C++ and SIMD
//printf("Speedup of 1 Thread and SIMD= %8.8lf \n",S );
//End of SIMD alone




//SIMD with 2 threads
        omp_set_num_threads( 2 );
        double sum=0.;
        double maxPerformanceST1 = 0.;
         for (int t = 0; t < NUMTRIES; t++)
           {
           sum=0.;

           int s=ARRAYS/2;
           double timeST0 = omp_get_wtime();    //Timer start
           #pragma omp parallel reduction(+:sum)
           {
           int f=omp_get_thread_num( );
           int first=f*s;
           sum=sum+SimdMulSum(&a[first],&b[first],s);
           } //+++Using threads together with SIMD
           int even=s*2;
           int che=ARRAYS-even;
           if(che!=0){
           for(int u=even;u<ARRAYS;u++){
           sum+=a[u]*b[u];
           }// +++Calculating the rest
           } //+++Handling the rest
            double timeST1 = omp_get_wtime();
            double exeST=timeST1-timeST0;
            double performanceST=(double)ARRAYS/exeST/1000000.;
            if( performanceST > maxPerformanceST1 )
            maxPerformanceST1 = performanceST;      //Performance

            }
            printf("*****result with SIMD with 2 Threads= %8.8lf \n",sum );



            double S2=maxPerformanceST1/maxPerformanceC;  //Speedup of C++ and SIMD with threads
         // printf("Speedup of 1 Thread and SIMD with 2 threads= %8.8lf \n",S2 );


//End of SIMD with 2 threads
            











//SIMD with 4 threads
	omp_set_num_threads( 4 );
sum=0.;
//int s=ARRAYS/4;
double maxPerformanceST = 0.;
 for (int t = 0; t < NUMTRIES; t++)
  {
sum=0.;

int s=ARRAYS/4;
double timeST0 = omp_get_wtime();    //Timer start
#pragma omp parallel reduction(+:sum)
{
int f=omp_get_thread_num( );

int first=f*s;


sum=sum+SimdMulSum(&a[first],&b[first],s);

} //+++Using threads together with SIMD


int even=s*4;
int che=ARRAYS-even;
if(che!=0){
for(int u=even;u<ARRAYS;u++){

sum+=a[u]*b[u];

}// +++Calculating the rest
} //+++Handling the rest
 double timeST1 = omp_get_wtime();
double exeST=timeST1-timeST0;
double performanceST=(double)ARRAYS/exeST/1000000.;
if( performanceST > maxPerformanceST )
maxPerformanceST = performanceST;         //Performance

}

printf("*****result with SIMD with 4 Threads= %8.8lf \n",sum );

double S1=maxPerformanceST/maxPerformanceC;  //Speedup of C++ and SIMD with threads



//End of SIMD with 4 threads






//SIMD with 8 threads
 omp_set_num_threads(8 );
         sum=0.;
        double maxPerformanceST2 = 0.;
         for (int t = 0; t < NUMTRIES; t++)
           {
           sum=0.;

           int s=ARRAYS/8;
           double timeST0 = omp_get_wtime();    //Timer start
           #pragma omp parallel reduction(+:sum)
           {
           int f=omp_get_thread_num( );
           int first=f*s;
           sum=sum+SimdMulSum(&a[first],&b[first],s);
           } //+++Using threads together with SIMD
           int even=s*8;
           int che=ARRAYS-even;
           if(che!=0){
           for(int u=even;u<ARRAYS;u++){
           sum+=a[u]*b[u];
           }// +++Calculating the rest
           } //+++Handling the rest
            double timeST1 = omp_get_wtime();
            double exeST=timeST1-timeST0;
            double performanceST=(double)ARRAYS/exeST/1000000.;
            if( performanceST > maxPerformanceST2 )
            maxPerformanceST2 = performanceST;      //Performance

            }
 printf("*****result with SIMD with 8 Threads= %8.8lf \n",sum );



            double S3=maxPerformanceST2/maxPerformanceC;  //Speedup of C++ and SIMD with threads






//End of SIMD with 8 threads




printf("Speedup of 1 Thread and SIMD(SS)= %8.8lf \n",S );
printf("Speedup of 1 Thread and SIMD with 2 threads(SS2)= %8.8lf \n",S2 );
printf("Speedup of 1 Thread and SIMD with 4 threads(SS4)= %8.8lf \n",S1 );
printf("Speedup of 1 Thread and SIMD with 8 threads(SS8)= %8.8lf \n",S3 );
printf("Performance of 1 Thread(P1)=%8.8lf\n",maxPerformanceC);
printf("Performance of SIMD alone(PS)=%8.8lf\n",maxPerformanceS);
printf("Performance of SIMD with 2 Threads(PS2)=%8.8lf\n",maxPerformanceST1);
printf("Performance of SIMD with 4 Threads(PS4)=%8.8lf\n",maxPerformanceST);
printf("Performance of SIMD with 8 Threads(PS8)=%8.8lf\n",maxPerformanceST2);







printf("----------------------------------------------------------\n");                                                                                                                                                                                                                                                                                                                                                                                                                 
//printf("*****result with Threads= %8.8lf \n",sum );


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





