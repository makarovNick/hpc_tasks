#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

#include "mt19937_64.h"

#define epsilon 1e-4
#define alpha 1e-5

#define n_threads 4

double squared_loss(double* y, double* x, double a, double b, int N)
{
    double diff = 0.0;

#pragma omp parallel for reduction(+:diff) num_threads(n_threads)
    for (int i = 0; i < N; ++i)
    {
        diff += (y[i] - a * x[i] - b) * (y[i] - a * x[i] - b);
    }
    
    return diff;
}

int main()
{
    omp_set_num_threads(4);
    u_int64_t N = 100000000;

    init_genrand64(time(NULL));

    double* x = (double*)malloc(sizeof(double) * N);
    double* y = (double*)malloc(sizeof(double) * N);
    double A = genrand64_real2() * rand();
    double B = genrand64_real2() * rand();
    double a;
    double b;
    
    double sum_x = 0.0;
    double sum_x_2 = 0.0;
    double sum_xy = 0.0;
    double sum_y = 0.0;
    double sum_y_2 = 0.0;

#pragma omp parallel for shared(x,y,A,B) reduction(+:sum_x,sum_x_2,sum_xy,sum_y,sum_y_2) num_threads(n_threads)
    for (u_int64_t i = 0; i < N; ++i)
    {
        x[i] = i;
        y[i] = A * x[i] + B + genrand64_real2();
        sum_x += x[i];
        sum_x_2 += x[i] * x[i];
        sum_xy += x[i] * y[i];
        sum_y += y[i];
        sum_y_2 += y[i] * y[i];
    }

    double denom = (N * sum_x_2 - sum_x * sum_x);
    if (denom == 0)
    {
        a = 0.0 / 0.0;
        b = 0.0 / 0.0;
    }
    else
    {
        a = (N * sum_xy - sum_x * sum_y) / denom;
        b = (sum_y * sum_x_2 - sum_x * sum_xy) / denom;
    }

    // sum (y - (ax + b))^2
    // sum y^2 - 2y(ax +b) + (ax + b)^2
    // sum_y_2 - 2 a yx + 2b y + aaxx + 2ab x + bb
    // sum_y_2 - 2 * a * sum_yx + 2 * b * sum_y + a * a * sum_x_2 + 2 * a * b * sum_x + b * b;
    // sum_y_2 - 2 * a * sum_x_y + 2 * N * b * sum_y + N * a * a * sum_x_2 + 2 * a * b * sum_x + N * b * b
    // double loss = sum_y_2 - 2 * a * sum_xy + 2 * b * sum_y + a * a * sum_x_2 + 2 * a * b * sum_x + b * b;

    printf("POINTS : %lld\nTHREADS : %d\nREAL :      a = %lf  B = %lf\nCOMPUTED :  a = %lf  B = %lf\nLoss : %lf  \n \n", N, n_threads, A, B, a, b, squared_loss(y, x, a, b, N));
    free(x);
    free(y);

    return 0;
}