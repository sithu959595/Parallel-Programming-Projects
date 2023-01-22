#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>


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

float *T=new float[20];
for(int i=0;i<10;i++){
T[i]=T[i+10]=(float)i;
}

//for( int t = 0; t <NUMTRIES; t++ )
        {
                double time0 = omp_get_wtime( );



#pragma omp parallel for default(none) shared(Sums,A,Size,T)
for( int shift = 0; shift < 10; shift++ )
{
	float sum = 0.;
	for( int i = 0; i < 10; i++ )
	{
		sum += T[i] * T[i + shift];
         printf("%8.6lf X %8.6lf = %8.6lf\n",T[i], T[i + shift],T[i] * T[i + shift]);
	}
	Sums[shift] = sum;	// note the "fix #2" from false sharing if you are using OpenMP
printf("The sum for shift %d is %8.6lf",shift,sum);
}//for Calculation

double time1 = omp_get_wtime( );
double megaTrialsPerSecond = (double)Size / ( time1 - time0 ) / 1000000.;
if( megaTrialsPerSecond > maxPerformance )
maxPerformance = megaTrialsPerSecond;

}// for NUMTRIES
printf("maxPerformance for %d Threads= %8.7lf \n", maxPerformance,NUMT);

/*
for(int i=0;i<512;i+=32){
printf("Output for index %d = %8.6lf \n",i, Sums[i]);
}
*/
printf("Output for index 4 = %8.6lf \n", Sums[4]);

}    //main
