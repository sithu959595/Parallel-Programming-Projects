// System includes
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <math.h>
#include <stdlib.h>

// CUDA runtime
#include <cuda_runtime.h>

// Helper functions and utilities to work with CUDA
#include "helper_functions.h"
#include "helper_cuda.h"

// setting the number of trials in the monte carlo simulation:
#ifndef NUMTRIALS
#define NUMTRIALS	( 1024*1024 )
#endif


#ifndef BLOCKSIZE
#define BLOCKSIZE		32     // number of threads per block
#endif

#define NUMBLOCKS		( NUMTRIALS / BLOCKSIZE )


// ranges for the random numbers:
const float XCMIN =	 0.0;
const float XCMAX =	 2.0;
const float YCMIN =	 0.0;
const float YCMAX =	 2.0;
const float RMIN  =	 0.5;
const float RMAX  =	 2.0;

// function prototypes:
float		Ranf( float, float );
int		Ranf( int, int );
void		TimeOfDaySeed( );

__global__  void MonteCarlo( float *Xcs, float *Ycs, float *Rs, int *Hits )
{
	unsigned int wgNumber      = blockIdx.x;
	unsigned int wgDimension   = blockDim.x;
	unsigned int threadNum     = threadIdx.x;
	unsigned int gid           = wgNumber*wgDimension + threadNum;

	// all the monte carlo stuff goes in here
	// if we make it all the way through, then Hits[gid] = 1

	// randomize the location and radius of the circle:
	float xc = Xcs[gid];
	float yc = Ycs[gid];
	float  r =  Rs[gid];

	float tn = tanf( (float)( (M_PI/180.) * 30. ) );
	Hits[gid] = 0;

	// solve for the intersection using the quadratic formula:

float a = 1. + tn*tn;
	float b =-2.*( xc + yc*tn );
	float c =xc*xc + yc*yc - r*r;
	float d = b*b - 4.*a*c;

	// cascading if-statements:
	//	if you used "continue;" in project #1, change to this style because,
	//	if there is no for-loop, then there is nowhere to continue to



if(d>=0.)
	{
		d = sqrt( d );
			float t1 = (-b + d ) / ( 2.*a );
			float t2 = (-b - d ) / ( 2.*a );
			float tmin = t1 < t2 ? t1 : t2;

		if(tmin>=0.)
		{
			

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


			//float tt = ( 0. - ycir ) / outy;

			// find out if it hits the infinite plate:
			float t = ( 0. - ycir ) / outy;
			if( t >= 0. )
			{
				Hits[gid] = 1;
			}
                        else{
Hits[gid] = 0;
}
		}
                else{
              Hits[gid] = 0;
                }
	}
else{
Hits[gid] = 0;}

}

// main program:

int
main( int argc, char* argv[ ] )
{
	TimeOfDaySeed( );

int dev = findCudaDevice(argc, (const char **)argv);

	// allocate host memory:

	float *hXcs  = new float[NUMTRIALS];
	float *hYcs  = new float[NUMTRIALS];
	float * hRs  = new float[NUMTRIALS];
	int   *hHits = new   int[NUMTRIALS];

	// fill the random-value arrays:
	for( int n = 0; n < NUMTRIALS; n++ )
	{
		hXcs[n] = Ranf( XCMIN, XCMAX );
		hYcs[n] = Ranf( YCMIN, YCMAX );
 		hRs[n]  = Ranf(  RMIN,  RMAX );

}

	// allocate device memory:

	float *dXcs, *dYcs, *dRs;
	int *dHits;

	dim3 dimsXcs(  NUMTRIALS, 1, 1 );
	dim3 dimsYcs(  NUMTRIALS, 1, 1 );
	dim3 dimsRs(   NUMTRIALS, 1, 1 );
	dim3 dimsHits( NUMTRIALS, 1, 1 );

cudaError_t status;
	status = cudaMalloc( (void **)(&dXcs), NUMTRIALS*sizeof(float) );
	checkCudaErrors( status );

	status = cudaMalloc( (void **)(&dYcs), NUMTRIALS*sizeof(float) );
	checkCudaErrors( status );

	status = cudaMalloc( (void **)(&dRs), NUMTRIALS*sizeof(float) );
	checkCudaErrors( status );

	status = cudaMalloc( (void **)(&dHits), NUMTRIALS *sizeof(int) );
	checkCudaErrors( status );

// copy host memory to the device:

	status = cudaMemcpy( dXcs, hXcs, NUMTRIALS*sizeof(float), cudaMemcpyHostToDevice );
	checkCudaErrors( status );

	status = cudaMemcpy( dYcs, hYcs, NUMTRIALS*sizeof(float), cudaMemcpyHostToDevice );
	checkCudaErrors( status );

	status = cudaMemcpy( dRs, hRs, NUMTRIALS*sizeof(float), cudaMemcpyHostToDevice );
	checkCudaErrors( status );

// setup the execution parameters:

	dim3 threads(BLOCKSIZE, 1, 1 );
	dim3 grid(NUMBLOCKS, 1, 1 );

	// create and start timer

	cudaDeviceSynchronize( );

	// allocate CUDA events that we'll use for timing:

	cudaEvent_t start, stop;
	status = cudaEventCreate( &start );
	checkCudaErrors( status );
	status = cudaEventCreate( &stop );
	checkCudaErrors( status );

// record the start event:

	status = cudaEventRecord( start, NULL );
	checkCudaErrors( status );

	// execute the kernel:

	MonteCarlo<<< grid, threads >>>( dXcs, dYcs, dRs, dHits );

	// record the stop event:

	status = cudaEventRecord( stop, NULL );
	checkCudaErrors( status );

	// wait for the stop event to complete:

status = cudaEventSynchronize( stop );
	checkCudaErrors( status );

	float msecTotal = 0.0f;
	status = cudaEventElapsedTime( &msecTotal, start, stop );
	checkCudaErrors( status );

	// compute and print the performance

	double secondsTotal = 0.001 * (double)msecTotal;
	double trialsPerSecond = (float)NUMTRIALS / secondsTotal;
	double megaTrialsPerSecond = trialsPerSecond / 1000000.;
fprintf( stderr, "Blocksize is %d and Number of trials is %d\n", BLOCKSIZE, NUMTRIALS );
	fprintf( stderr, "Number of Trials = %10d, MegaTrials/Second = %10.4lf\n", NUMTRIALS, megaTrialsPerSecond );
        
// copy result from the device to the host:

	status = cudaMemcpy( hHits, dHits, NUMTRIALS *sizeof(int), cudaMemcpyDeviceToHost );
	checkCudaErrors( status );
	cudaDeviceSynchronize( );

	// compute the probability:

	int numHits = 0;
	for(int i = 0; i < NUMTRIALS; i++ )
	{
		numHits += hHits[i];
	}

float probability = 100.f * (float)numHits / (float)NUMTRIALS;
	fprintf(stderr, "\nProbability = %6.3f %%\n", probability );

	// clean up memory:
	delete [ ] hXcs;
	delete [ ] hYcs;
	delete [ ] hRs;
	delete [ ] hHits;

	status = cudaFree( dXcs );
	status = cudaFree( dYcs );
	status = cudaFree( dRs );
	status = cudaFree( dHits );
	checkCudaErrors( status );

	return 0;
}

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
