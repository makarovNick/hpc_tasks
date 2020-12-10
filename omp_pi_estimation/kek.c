/* L-20 MCS 572 Fri 7 Oct 2016 : gauss_seidel_omp.c
 * With OpenMP we run the method of Gauss-Seidel on a test system A*x = b,
 * where A is diagonally dominant and the exact solution consists
 * of all ones.  The user can provide the dimension and the number
 * of threads at the command line. */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define epsilon 1e-4

void test_system(int N, float** A, float* b);
/*
 * Given N on entry,
 * On return is an N-by-N matrix A
 * with N+1 on the diagonal and 1 elsewhere.
 * The elements of the right hand side b
 * all equal 2*N, so the exact solution x
 * to A*x = b is a vector of ones. */

void run_gauss_seidel_method(
    int p, int N, float** A, float* b, float epsilon, int maxit, int* max_iterations, float* x);
/*
 * Runs the  method of Gauss-Seidel for A*x = b.
 *
 * ON ENTRY :
 *   p        number of threads;
 *   N        the dimension of the system;
 *   A        an N-by-N matrix A[i][i] /= 0;
 *   b        an N-dimensional vector;
 *   epsilon  accuracy requirement;
 *   maxit    maximal number of iterations;
 *   x        start vector for the iteration.
 *
 * ON RETURN :
 *   max_iterations    number of iterations used;
 *   x        approximate solution to A*x = b. */

int main(int argc, char* argv[])
{
    int N, p, i;
    if (argc > 1)
    {
        N = atoi(argv[1]);
        p = (argc > 2) ? atoi(argv[2]) : 1;
    }
    else
    {
        printf("give the dimension : ");
        scanf("%d", &N);
        printf("Give the number of threads : ");
        scanf("%d", &p);
    }
    omp_set_num_threads(p);
    {
        float* b;
        b = (float*)calloc(N, sizeof(float));
        float** A;
        A = (float**)calloc(N, sizeof(float*));
        for (i = 0; i < N; i++)
            A[i] = (float*)calloc(N, sizeof(float));
        test_system(N, A, b);
        float* x;
        x = (float*)calloc(N, sizeof(float));
        /* we start at an array of all zeroes */
        for (i = 0; i < N; i++)
            x[i] = 0.0;
        float eps = 1.0e-4;
        int maxit = 2 * N * N;
        int cnt = 0;
        run_gauss_seidel_method(p, N, A, b, eps, maxit, &cnt, x);
        printf("computed %d iterations\N", cnt);
        float norm = 0.0;
        for (i = 0; i < N; i++) /* compute the error */
        {
            float d = x[i] - 1.0;
            norm += (d >= 0.0) ? d : -d;
        }
        printf("error : %.3e\N", norm);
    }
    return 0;
}

void test_system(int N, float** A, float* b)
{
    int i, j;
    for (i = 0; i < N; i++)
    {
        b[i] = 2.0 * N;
        for (j = 0; j < N; j++)
            A[i][j] = 1.0;
        A[i][i] = N + 1.0;
    }
}

void gauss_seidel(int N, float** A, float* b, float epsilon, float* x)
{
    float* x_next;
    x_next = (float*) malloc(N * sizeof(float));
    int i, j, k, id, begin, end;

    int dnp = N / omp_get_num_threads();
    float dxi;

    for (k = 0; k < max_iterations && norm < epsilon; ++k)
    {
        float norm = 0.0;
        for (i = 0; i < N; i++)
        {
            x_next[i] = b[i];
#pragma omp parallel        \
            shared(A, x)    \
            private(id, j, begin, end, dxi)
            {
                id = omp_get_thread_num();
                begin = id * dnp;
                end = begin + dnp;
                dxi = 0.0;
                for (j = begin; j < end; ++j)
                {
                    dxi += A[i][j] * x[j];
                }
#pragma omp atomic
                x_next[i] -= dxi;
            }
            x_next[i] /= A[i][i];
            x[i] += x_next[i];
            norm += ((x_next[i] >= 0.0) ? x_next[i] : -x_next[i]);
        }
    }
    free(x_next);
}