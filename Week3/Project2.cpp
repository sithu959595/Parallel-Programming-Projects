// Project0.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _USE_MATH_DEFINES
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <stdio.h>

#define NUMT	         4
#define SIZE       	30000	// you decide
#define NUMNODES        100	// you decide
#define N              2

#define XMIN     -1.
#define XMAX      1.
#define YMIN     -1.
#define YMAX      1.

float Height( int, int );

float A[SIZE];
float B[SIZE];
float C[SIZE];

int main( int argc, char *argv[ ] )
{
	

	// the area of a single full-sized tile:

	float fullTileArea = (  ( ( XMAX - XMIN )/(float)(NUMNODES-1) )  *
				( ( YMAX - YMIN )/(float)(NUMNODES-1) )  );
  float quarter= fullTileArea/4.0;
  float half=fullTileArea/2.0;

	// sum up the weighted heights into the variable "volume"
	// using an OpenMP for loop and a reduction:
double sum=0.;
  #pragma omp parallel for collapse(2) default(none),reduction(+:sum)
  for( int iv = 0; iv < NUMNODES; iv++ )
  {
  	for( int iu = 0; iu < NUMNODES; iu++ )
  	{
  		if(iv==0 || iv==NUMNODES-1){

        if(iu==0 || iu==NUMNODES-1){
          float z = Height( iu, iv );
          sum+=z*quarter;
        }
        else{
          float z = Height( iu, iv );
          sum+=z*half;
        }

      }
      else if(iu==0 || iu==NUMNODES-1){
        float z = Height( iu, iv );
        sum+=z*half;
      }
  	}//inner for loop
  }//outer for loop

}//Main

float
Height( int iu, int iv )	// iu,iv = 0 .. NUMNODES-1
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



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
