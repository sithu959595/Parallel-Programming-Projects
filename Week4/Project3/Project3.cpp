#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>

#ifndef NUMT
#define NUMT		4
#endif


//#define YMAX      1.
//float Height( int, int );

int	NowYear=2020 ;		// 2020 - 2025
int	NowMonth=0 ;		// 0 - 11

float	NowPrecip=0.;		// inches of rain per month
float	NowTemp=0.;		// temperature this month
float	NowHeight=50.;		// grain height in inches
int	NowNumDeer=150 ;		// number of deer in the current population

const float GRAIN_GROWS_PER_MONTH =		25.0;   //*****change 9.0 to 25.0
const float ONE_DEER_EATS_PER_MONTH =		0.04;   //********change 1.0 to 0.04 

const float AVG_PRECIP_PER_MONTH =		7.0;	// average
const float AMP_PRECIP_PER_MONTH =		6.0;	// plus or minus
const float RANDOM_PRECIP =			2.0;	// plus or minus noise

const float AVG_TEMP =				60.0;	// average
const float AMP_TEMP =				20.0;	// plus or minus
const float RANDOM_TEMP =			10.0;	// plus or minus noise

const float MIDTEMP =				40.0;
const float MIDPRECIP =				10.0;
unsigned int seed = 0;
bool outbreak=false;         //*****
float immune=0.;            //******
int Nowideer=0;              //******
float spread=0.;             //*****
bool recover=false;         //*****
int NextNumDeer=0;          //******

void Virus();
void GrainDeer();
void Grain();
void Watcher();
float SQR( float );
float Ranf(unsigned int *,float,float);
int Ranf(unsigned int *,int,int);
float FtoC(float);
float intocm(float);


int main(int argc, char *argv[ ])
{

#ifndef _OPENMP
	fprintf( stderr, "No OpenMP support!\n" );
	return 1;
#endif


omp_set_num_threads( 4 );	// same as # of sections
#pragma omp parallel sections
{

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
        #pragma omp section
        {
                Virus( );
        }

	
	
}       // implied barrier -- all functions must return in order
	// to allow any of them to get past 

}


void Virus(){
while( NowYear < 2026 ){
if(NowYear==2023){          //******      outbreak starts
outbreak=true;               //******
spread=0.1;               //******
}                       //******

if(immune==50.){                           //****** immune system is perfect
recover=true;                        //******
spread=0.;                              //******
outbreak=false;                           //******
}

int Nextideer=Nowideer;                                          //******

if(outbreak){                                                    //******
Nextideer=Nextideer-(int)(ceil((float)Nextideer*0.03));              //****** die of the virus
float a=floor(NowNumDeer*spread);
Nextideer=(int)(floor((float)NowNumDeer*spread))+Nextideer; //****** adding new infected deer

immune=immune+4.;                            //******       immune increase
if(immune>50.){                                              //******    
immune=50.;                                                 //******
}                                              //******

}

#pragma omp barrier
if(recover){                                                                   //******
NextNumDeer=NextNumDeer+Nowideer;                                                //******   all recover
Nextideer=0;                                                //*****Infection Reset
}
#pragma omp barrier              
//Copy Next Values to Now Values
Nowideer=Nextideer;                   //******
#pragma omp barrier
//Wait for the Watcher
#pragma omp barrier
//

}


}

void GrainDeer(){
while( NowYear < 2026 )
{
NextNumDeer=NowNumDeer;         
//Compute Next States
if(NowHeight>NowNumDeer){
printf("...............IIIIIIIIncrese\n");
NextNumDeer=NowNumDeer+5;
}
else if(NowHeight<NowNumDeer){
NextNumDeer=NowNumDeer-1;
}
if(NextNumDeer<0){
NextNumDeer=0;
}


NextNumDeer=NextNumDeer-((int)(floor((float)NowNumDeer*spread))); 

#pragma omp barrier
//for Virus
#pragma omp barrier
//Copy Next Values to Now Values
NowNumDeer=NextNumDeer;
//Nowideer=Nextideer;                   //******
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
nextHeight=0.;
}
#pragma omp barrier
//for Virus
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
//for Virus
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
printf("(HDr)The Number of Deer is %d\n",NowNumDeer );
                         
if(outbreak){                        //*******
printf("Outbreak is on\n");                       //*******
}                                                     //*******
printf("(I)Immune is %8.8lf\n",immune );                        //*******
printf("(IDr)The number of infected deer is %d\n",Nowideer );                       //*******
                                          //*******
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

float intocm(float v){
return (v*2.54);
}

float FtoC(float f){
float re=(5./9.)*(f-32);
return re;
}
