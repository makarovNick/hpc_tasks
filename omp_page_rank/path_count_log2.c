#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define min(a, b) ((a < b) ? (a) : (b))

void mul(int** a, int** b, int N, int** res)
{
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
        {
            res[i][j] = 0;
            for (int k = 0; k < N; k++)
                res[i][j] += a[i][k] * b[k][j];
        }
}


int** generate_adjancenty(int N, double alpha)
{
    srand(time(NULL));
    int** adj = (int**) malloc(sizeof(int*) * N);
    int c = 0;
    for (int i = 0; i < N; ++i)
    {
        adj[i] = (int*) malloc(sizeof(int) * N);
        memset(adj[i], '\0', N * sizeof(int));
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < i; j++)
        {
            adj[i][j] = round(1.0 * rand() * alpha / RAND_MAX);
            adj[j][i] = adj[i][j];
        }
        
    }
    
    return adj;
}

void mm_pow(int** a, int pow, int N)
{
    int** one = (int**) malloc(sizeof(int*) * N);
    int** res = (int**) malloc(sizeof(int*) * N);

    for (int i = 0; i < N; ++i)
    {
        one[i] = (int*) malloc(sizeof(int) * N);
        res[i] = (int*) malloc(sizeof(int) * N);
        memset(one[i], '\0', N * sizeof(int));       
        one[i][i] = 1;
    }
    
    while (pow > 0) 
    {
        if (pow % 2 == 0)
        {
            mul(a, a, N, res);
            pow /= 2;
            for (int i = 0; i < N; ++i)
            {
                memmove(a[i], res[i], sizeof(int) * N);
            }
        }
        else
        {
            if (pow != 1)
            {
                mul(one, a, N, res);
                for (int i = 0; i < N; ++i)
                {
                    memmove(one[i], res[i], sizeof(int) * N);
                }
            }
            pow--;
        }
        for (int i = 0; i < N; ++i)
        {
            memset(res[i], '\0', N * sizeof(int));
        }
    }
    mul(a, one, N, res);
    for (int i = 0; i < N; ++i)
    {
        memmove(a[i], res[i], sizeof(int) * N);
    }
    free(one);
    free(res);
}

int main()
{
    int N = 4;

    int** graph = generate_adjancenty(N, 1.0);

    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            printf("%ld ", graph[i][j]);
        }
        putchar('\n');

    }
    putchar('\n');
    mm_pow(graph, 2, N);
    for (int i = 0; i < N; ++i)
    {
        for (int j = 0; j < N; ++j)
        {
            printf("%ld ", graph[i][j]);
        }
        putchar('\n');

    }

    free(graph);


    return 0;
}