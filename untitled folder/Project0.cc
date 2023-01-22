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
#define NUMTRIES        100	// you decide
#define NUMT1	         1

float A[SIZE];
float B[SIZE];
float C[SIZE];

int
main()
{
#ifndef _OPENMP
    fprintf(stderr, "OpenMP is not supported here -- sorry.\n");
    return 1;
#endif

    // inialize the arrays:
    for (int i = 0; i < SIZE; i++)
    {
        A[i] = 1.;
        B[i] = 2.;
    }

    omp_set_num_threads(NUMT);
    fprintf(stderr, "Using %d threads\n", NUMT);

    double maxMegaMults = 0.;

    for (int t = 0; t < NUMTRIES; t++)
    {
        double time0 = omp_get_wtime();

#pragma omp parallel for
        for (int i = 0; i < SIZE; i++)
        {
            C[i] = A[i] * B[i];
        }

        double time1 = omp_get_wtime();
        double megaMults = (double)SIZE / (time1 - time0) / 1000000.;
        if (megaMults > maxMegaMults)
            maxMegaMults = megaMults;
    }
    printf("Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults);

//Thread 1
    double T = 1.;
    fprintf(stderr, "Using %d threads\n", NUMT1);
    omp_set_num_threads(NUMT1);
    double maxMegaMults1 = 0.;
    for (int t = 0; t < NUMTRIES; t++)
    {
        double time0 = omp_get_wtime();

#pragma omp parallel for
        for (int i = 0; i < SIZE; i++)
        {
            C[i] = A[i] * B[i];
        }

        double time1 = omp_get_wtime();
        T = time1 - time0;
        double megaMults = (double)SIZE / (time1 - time0) / 1000000.;
        if (megaMults > maxMegaMults1)
            maxMegaMults1 = megaMults;
    }


    printf("Peak Performance = %8.2lf MegaMults/Sec\n", maxMegaMults1);
    double S = maxMegaMults / maxMegaMults1;
    printf("S = %8.2lf\n", S);
    float Fp = (4. / 3.) * (1. - (1. / S));
    printf("Parallel Fraction = %8.2lf \n", Fp);
    printf("Time=%8.8lf\n", T);

    // note: %lf stands for "long float", which is how printf prints a "double"
    //        %d stands for "decimal integer", not "double"

    return 0;
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
