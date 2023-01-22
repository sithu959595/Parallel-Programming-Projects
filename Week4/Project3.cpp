#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef NUMT
#define NUMT		4
#endif


//#define YMAX      1.
//float Height( int, int );

int	NowYear=2020 ;		// 2020 - 2025
int	NowMonth=0 ;		// 0 - 11

float	NowPrecip=0.;		// inches of rain per month
float	NowTemp=0.;		// temperature this month
float	NowHeight=1.;		// grain height in inches
int	NowNumDeer=1 ;		// number of deer in the current population

const float GRAIN_GROWS_PER_MONTH =		9.0;
const float ONE_DEER_EATS_PER_MONTH =		1.0;

const float AVG_PRECIP_PER_MONTH =		7.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				60.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;
unsigned int seed = 0;

void GrainDeer();
void Grain();
void Watcher();
float SQR( float );
float Ranf(unsigned int *,float,float);
int Ranf(unsigned int *,int,int);
float FtoC(float);
float intocm(int);

int main(int argc, char *argv[ ])
{

#ifndef _OPENMP
	fprintf( stderr, "No OpenMP support!\n" );
	return 1;
#endif


omp_set_num_threads( 3 );	// same as # of sections
#pragma omp parallel sections
{
printf("Newwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n");
	#pragma omp section
	{
		GrainDeer( );
	}

	#pragma omp section
	{
		Grain( );
	}

	#pragma omp section
	{
		Watcher( );
	}

	
	
}       // implied barrier -- all functions must return in order
	// to allow any of them to get past 

}

void GrainDeer(){
while( NowYear < 2026 )
{
int NextNumDeer=NowNumDeer;
//Compute Next States
if(NowHeight>NowNumDeer){
NextNumDeer=NowNumDeer+1;
}
else if(NowHeight<NowNumDeer){
NextNumDeer=NowNumDeer-1;
}
if(NextNumDeer<0){
NextNumDeer=0;
}
#pragma omp barrier
//Copy Next Values to Now Values
NowNumDeer=NextNumDeer;
#pragma omp barrier
//Wait for the Watcher
#pragma omp barrier
}
}

void Grain(){
while( NowYear < 2026 )
{
float nextHeight=0.;
//Compute Next States
float tempFactor = exp(   -SQR(  ( NowTemp - MIDTEMP ) / 10.  )   );

float precipFactor = exp(   -SQR(  ( NowPrecip - MIDPRECIP ) / 10.  )   );
nextHeight = NowHeight;
 nextHeight += tempFactor * precipFactor * GRAIN_GROWS_PER_MONTH;
 nextHeight -= (float)NowNumDeer * ONE_DEER_EATS_PER_MONTH;
if(nextHeight<0.){
//printf("*********\n");
nextHeight=0.;
}
#pragma omp barrier
//Copy Next Values to Now Values
NowHeight=nextHeight;
#pragma omp barrier
//Wait for the Watcher
#pragma omp barrier
}
}

void Watcher(){
while( NowYear < 2026 )
{
//Compute Next States
#pragma omp barrier
//Copy Next Values to Now Values
#pragma omp barrier
//Wait for the Watcher
printf("---*---1\n");
printf("(Mon)The Month is %d\n", NowMonth);
printf("(Yr)The Year is %d\n",NowYear);
float prr=intocm(NowPrecip);
printf("(Precip)The Precipitation is %8.8lf\n",prr);
float te=FtoC(NowTemp);
printf("(Temp)The Temperature is %8.8lf\n",te);
float gr=intocm(NowHeight);
printf("(Gr)The Grain Height is %8.8lf\n",gr);
printf("(Dr)The Number of Deer is %d\n",NowNumDeer);
printf("---*---\n");
if(NowMonth==11){
NowYear=NowYear+1;
NowMonth=0;
}
else
{
NowMonth=NowMonth+1;
}

float ang = (  30.*(float)NowMonth + 15.  ) * ( M_PI / 180. );

float temp = AVG_TEMP - AMP_TEMP * cos( ang );
NowTemp = temp + Ranf( &seed, -RANDOM_TEMP, RANDOM_TEMP );



float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin( ang );
NowPrecip = precip + Ranf( &seed,  -RANDOM_PRECIP, RANDOM_PRECIP );
if( NowPrecip < 0. )
	NowPrecip = 0.;

#pragma omp barrier
}
}

float
SQR( float x )
{
        return x*x;
}

float
Ranf( unsigned int *seedp,  float low, float high )
{
        float r = (float) rand_r( seedp );              // 0 - RAND_MAX

        return(   low  +  r * ( high - low ) / (float)RAND_MAX   );
}


int
Ranf( unsigned int *seedp, int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = (float)ihigh + 0.9999f;

      return (int)(  Ranf(seedp, low,high) );
}

float intocm(int v){
return (v*2.54);
}

float FtoC(float f){
float re=(5./9.)*(f-32);
return re;
}
