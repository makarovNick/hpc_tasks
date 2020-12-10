#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include "mt19937_64.h"


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "usage:\n%s <number_of_samples>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    u_int64_t iterations = strtoull(argv[1], NULL, 10);

    double start_time, end_time;
    u_int64_t i;
    double x, y;
    u_int64_t count;
    start_time = omp_get_wtime();
#pragma omp parallel
{
        init_genrand64(time(NULL) ^ omp_get_thread_num());
#pragma omp for private(x, y)       \
                reduction(+:count) 
        for (i = 0; i < iterations; ++i)
        {
            double x = genrand64_real1();
            double y = genrand64_real1();
            count += (x * x + y * y < 1.0);
        }
    }
    long double pi = 4.0 * count / iterations;

    end_time = omp_get_wtime();

    printf("####################################################################\n");
    printf("*       COMPUTED PI       |             %12.10Lf               *\n", pi);
    printf("*       DIFF WITH M_PI    |             %12.10Lf               *\n", fabsl(pi - M_PI));
    printf("*       TIME              |             %12.10f seconds       *\n", end_time - start_time);
    printf("####################################################################\n");

    return EXIT_SUCCESS;
}