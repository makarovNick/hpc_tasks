#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

static float sum = 0;

void dotprod(float * a, float * b, size_t N)
{
    int i, tid;

    tid = omp_get_thread_num();

#pragma omp for reduction(+:sum)
    for (i = 0; i < N; ++i)
    {
        sum += a[i] * b[i];
        printf("tid = %d i = %d\n", tid, i);
    }
}

int main (int argc, char *argv[])
{
    const size_t N = 100;
    int i;
    float a[N], b[N];
    for (i = 0; i < N; ++i)
    {
        a[i] = b[i] = (float)i;
    }

#pragma omp parallel
    dotprod(a, b, N);

    printf("Sum = %f\n",sum);

    return 0;
}