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

#define NUMT1	         1

#ifndef NUMTRIALS
#define NUMTRIALS	10000000
#endif


#ifndef NUMTRIES
#define NUMTRIES	10
#endif


const float XCMIN =	-1.0;
const float XCMAX =	 1.0;
const float YCMIN =	 0.0;
const float YCMAX =	 2.0;
const float RMIN  =	 0.5;
const float RMAX  =	 2.0;


float		Ranf( float, float );
int		Ranf( int, int );
void		TimeOfDaySeed( );


int
main( int argc, char *argv[ ] )
{
#ifndef _OPENMP
	fprintf( stderr, "No OpenMP support!\n" );
	return 1;
#endif

	float tn = tan( (M_PI/180.)*30. );
	TimeOfDaySeed( );

	omp_set_num_threads( NUMT );


	float *xcs = new float [NUMTRIALS];
	float *ycs = new float [NUMTRIALS];
	float * rs = new float [NUMTRIALS];


        for( int n = 0; n < NUMTRIALS; n++ )
        {
                xcs[n] = Ranf( XCMIN, XCMAX );
                ycs[n] = Ranf( YCMIN, YCMAX );
                rs[n] = Ranf(  RMIN,  RMAX );
        }


        float maxPerformance = 0.;
        float currentProb;


        for( int t = 0; t < NUMTRIES; t++ )
        {
                double time0 = omp_get_wtime( );

                int numHits = 0;
		#pragma omp parallel for default(none) shared(xcs,ycs,rs,tn) reduction(+:numHits)
		for( int n = 0; n < NUMTRIALS; n++ )
		{

			float xc = xcs[n];
			float yc = ycs[n];
			float  r =  rs[n];


			float a = 1. + tn*tn;
			float b = -2.*( xc + yc*tn );
			float c = xc*xc + yc*yc - r*r;
			float d = b*b - 4.*a*c;


if(d>0. || d==0.)
{



			d = sqrt( d );
			float t1 = (-b + d ) / ( 2.*a );
			float t2 = (-b - d ) / ( 2.*a );
			float tmin = t1 < t2 ? t1 : t2;

if(tmin>0. || tmin==0.){


			float xcir = tmin;
			float ycir = tmin*tn;


			float nx = xcir - xc;
			float ny = ycir - yc;
			float nxy = sqrt( nx*nx + ny*ny );
			nx /= nxy;
			ny /= nxy;


			float inx = xcir - 0.;
			float iny = ycir - 0.;
			float in = sqrt( inx*inx + iny*iny );
			inx /= in;
			iny /= in;


			float dot = inx*nx + iny*ny;
			float outx = inx - 2.*nx*dot;
			float outy = iny - 2.*ny*dot;


			float tt = ( 0. - ycir ) / outy;
if(tt>0. || tt==0.){numHits=numHits+1;}
      }
    }



		}
		double time1 = omp_get_wtime( );
		double megaTrialsPerSecond = (double)NUMTRIALS / ( time1 - time0 ) / 1000000.;
		if( megaTrialsPerSecond > maxPerformance )
			maxPerformance = megaTrialsPerSecond;
		currentProb = (float)numHits/(float)NUMTRIALS;	
printf("currentProb= %8.8lf \n", currentProb);
}//#Threads
printf("maxPerformance for %d Threads= %8.7lf \n", maxPerformance,NUMT);
  //Thread 1
    omp_set_num_threads( NUMT1 );
      float maxPerformance1 = 0.;
  
        for( int t = 0; t < NUMTRIES; t++ )
          {
                    double time0 = omp_get_wtime( );
  
                              int numHits = 0;
                              #pragma omp parallel for default(none) shared(xcs,ycs,rs,tn) reduction(+:numHits)
                              for( int n = 0; n < NUMTRIALS; n++ )
                              {
  
                              float xc = xcs[n];
                              float yc = ycs[n];
                              float  r =  rs[n];
 
  
                              float a = 1. + tn*tn;
                              float b = -2.*( xc + yc*tn );
                              float c = xc*xc + yc*yc - r*r;
                              float d = b*b - 4.*a*c;
  
  
                             if(d>0. || d==0.)
                              {
  
  
  
                              d = sqrt( d );
                              float t1 = (-b + d ) / ( 2.*a );
                              float t2 = (-b - d ) / ( 2.*a );
                              float tmin = t1 < t2 ? t1 : t2;
  
                              if(tmin>0. || tmin==0.){
  
 
                              float xcir = tmin;
                              float ycir = tmin*tn;
  
  
                              float nx = xcir - xc;
                              float ny = ycir - yc;
                              float nxy = sqrt( nx*nx + ny*ny );
                              nx /= nxy;
                              ny /= nxy;
  
  
                              float inx = xcir - 0.;
                              float iny = ycir - 0.;
                              float in = sqrt( inx*inx + iny*iny );
                              inx /= in;
                              iny /= in;
  
  
                              float dot = inx*nx + iny*ny;
                              float outx = inx - 2.*nx*dot;
                              float outy = iny - 2.*ny*dot;
  
  
                              float tt = ( 0. - ycir ) / outy;
                              if(tt>0. || tt==0.){numHits=numHits+1;}
                              }
                              }
  
  
  
                              }
                              double time1 = omp_get_wtime( );
                              double megaTrialsPerSecond = (double)NUMTRIALS / ( time1 - time0 ) / 1000000.;
                              if( megaTrialsPerSecond > maxPerformance1 )
                              maxPerformance1 = megaTrialsPerSecond;
                              currentProb = (float)numHits/(float)NUMTRIALS;
                             
                              printf("currentProb with 1 Thread= %8.8lf \n", currentProb);
                               }//Thread1	
printf("maxPerformance for 1 Thread = %8.7lf \n", maxPerformance1);
 double S=maxPerformance/maxPerformance1; //Speedup
    printf("Speedup for %d to 1 threads= %8.2lf\n",NUMT,S);
    float NUMTf=(float)NUMT;
    float Fp = (NUMTf / (NUMTf-1)) * (1. - (1. / S));
    printf("Parallel Fraction = %8.2lf \n", Fp);
}
#include <stdlib.h>

float
Ranf( float low, float high )
{
        float r = (float) rand();               // 0 - RAND_MAX
        float t = r  /  (float) RAND_MAX;       // 0. - 1.

        return   low  +  t * ( high - low );
}

int
Ranf( int ilow, int ihigh )
{
        float low = (float)ilow;
        float high = ceil( (float)ihigh );

        return (int) Ranf(low,high);
}

void
TimeOfDaySeed( )
{
	struct tm y2k = { 0 };
	y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
	y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

	time_t  timer;
	time( &timer );
	double seconds = difftime( timer, mktime(&y2k) );
	unsigned int seed = (unsigned int)( 1000.*seconds );    // milliseconds
	srand( seed );
}


