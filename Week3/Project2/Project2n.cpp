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

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.
float Height( int, int );
int main(int argc, char *argv[ ])
{

#ifndef _OPENMP
	fprintf( stderr, "No OpenMP support!\n" );
	return 1;
#endif



double fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
				( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );
 
 double quarter= fullTileArea/4.0;
  double half=fullTileArea/2.0;

double sum=0.;
float maxPerformance = 0.;
omp_set_num_threads( NUMT );

 for( int t = 0; t < NUMTRIES; t++ )
        {

//double ta0=0.;
double time0 = omp_get_wtime( );
  #pragma omp parallel for collapse(2) shared(fullTileArea,quarter,half) reduction(+:sum)
  for( int iv = 0; iv < NUMNODES; iv++ )
  {
  	for( int iu = 0; iu < NUMNODES; iu++ )
  	{


if((iu==0 && iv==0) || (iu==(NUMNODES-1) && iv==0) || (iu==(NUMNODES-1) && iv==(NUMNODES-1)) || (iu==0 && iv==(NUMNODES-1)) ){
double z = Height( iu, iv );
sum+=z*quarter*2.;
//ta0+=z*quarter*2.;
}//quarter
else if(iu==0 || iv==0 || iu==NUMNODES-1 || iv==NUMNODES-1 ){
double z = Height( iu, iv );
sum+=z*half*2.;
//ta0+=z*half*2.;
}//half
else{
double z = Height( iu, iv );
sum+=z*2.*fullTileArea;
//ta0+=z*2.*fullTileArea;
}//full



//
  //		if(iv==0 || iv==NUMNODES-1){
//
    //    if(iu==0 || iu==NUMNODES-1){
      //    float z = Height( iu, iv );
        //  sum+ =z*quarter;
       // }
     //   else{
   //       float z = Height( iu, iv );
       //   sum+=z*half;
//        }
//
//      }
//      else if(iu==0 || iu==NUMNODES-1){
//        float z = Height( iu, iv );
//
 //       sum+=z*half;
 //     }
//else{
//float z = Height( iu, iv );
// sum+=z*half;
//c++;
//}
  	}//inner for loop
  }//outer for loop

//printf("Volumn with %i t0= %8.8lf \n",ta0);

double time1 = omp_get_wtime( );
double megaNodesPerSecond = (double)(NUMNODES*NUMNODES) / ( time1 - time0 ) / 1000000.;
if( megaNodesPerSecond > maxPerformance )
			maxPerformance = megaNodesPerSecond;
}//NUMTRIES LOOP With # Threads
 printf("Original Volumn with %i Threads= %8.8lf \n", NUMT, sum);
sum=sum/NUMTRIES;
 printf("--V--Volumn with %i Threads= %8.8lf \n", NUMT, sum);
//printf("count = %i \n", c);
printf("---*---maxPerformance for %d Threads= %8.7lf \n", maxPerformance,NUMT);



// 1 Thread
  omp_set_num_threads( NUMT1 );
        float maxPerformance1 = 0.;

sum=0.;
 for( int t = 0; t < NUMTRIES; t++ )
        {
//double ta=0.;
double time0 = omp_get_wtime( );
  #pragma omp parallel for collapse(2) shared(fullTileArea,quarter,half) reduction(+:sum)
  for( int iv = 0; iv < NUMNODES; iv++ )
  {
        for( int iu = 0; iu < NUMNODES; iu++ )
        {


if((iu==0 && iv==0) || (iu==(NUMNODES-1) && iv==0) || (iu==(NUMNODES-1) && iv==(NUMNODES-1)) || (iu==0 && iv==(NUMNODES-1)) ){
double z = Height( iu, iv );
sum+=z*quarter*2.;
//ta+=z*quarter*2.;
}//quarter
else if(iu==0 || iv==0 || iu==NUMNODES-1 || iv==NUMNODES-1 ){
double z = Height( iu, iv );
sum+=z*half*2.;
//ta+=z*half*2.;
}//half
else{
double z = Height( iu, iv );
sum+=z*2.*fullTileArea;
//ta+=z*2.*fullTileArea;
}//full
 }//inner for loop
  }//outer for loop

double time1 = omp_get_wtime( );
double megaNodesPerSecond = (double)(NUMNODES*NUMNODES) / ( time1 - time0 ) / 1000000.;
if( megaNodesPerSecond > maxPerformance1 )
                        maxPerformance1 = megaNodesPerSecond;
//printf("t= %8.8lf \n", ta);



}//NUMTRIES LOOP With 1 Thread



printf("Original Volume= %8.8lf \n", sum);
sum=sum/NUMTRIES;
 printf("--V--Volumn with 1 Thread= %8.8lf \n", sum);
//printf("count = %i \n", c);
printf("------maxPerformance for 1 Thread= %8.7lf \n", maxPerformance1);

double S=maxPerformance/maxPerformance1; //Speedup
printf("Speedup for %d to 1 threads= %8.2lf\n",NUMT,S);
float NUMTf=(float)NUMT;
float Fp = (NUMTf / (NUMTf-1)) * (1. - (1. / S));
printf("--PF--Parallel Fraction = %8.2lf \n", Fp); //Parallel Fraction





}//Main
float Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
{
	float x = -1.  +  2.*(float)iu /(float)(NUMNODES-1);	// -1. to +1.
	float y = -1.  +  2.*(float)iv /(float)(NUMNODES-1);	// -1. to +1.

	float xn = pow( fabs(x), (double)N );
	float yn = pow( fabs(y), (double)N );
	float r = 1. - xn - yn;
	if( r < 0. )
	        return 0.;
	float height = pow( 1. - xn - yn, 1./(float)N );
	return height;
}
