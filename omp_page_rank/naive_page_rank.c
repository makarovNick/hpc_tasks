#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define SWAP(x, y)                                                                                 \
    do                                                                                             \
    {                                                                                              \
        typeof(x) _x = x;                                                                          \
        typeof(y) _y = y;                                                                          \
        x = _y;                                                                                    \
        y = _x;                                                                                    \
    } while (0)
#define epsilon 1e-3

int** generate_adjancenty(int N, double alpha)
{
    srand(time(NULL));
    int** adj = (int**)malloc(sizeof(int*) * N);
    int c = 0;
    for (int i = 0; i < N; ++i)
    {
        adj[i] = (int*)malloc(sizeof(int) * N);
        memset(adj[i], '\0', N * sizeof(int));
    }
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            adj[i][j] = round(1.0 * rand() * alpha / RAND_MAX);
            // adj[j][i] = adj[i][j];
        }
    }

    return adj;
}

double** generate_weighted_adjancenty(int N, double alpha)
{
    int** adj = generate_adjancenty(N, alpha);

    double** n_adj = (double**)malloc(sizeof(double*) * N);
    for (int i = 0; i < N; ++i)
    {
        n_adj[i] = (double*)malloc(sizeof(double) * N);
        memset(n_adj[i], '\0', sizeof(double) * N);
    }

    for (int i = 0; i < N; ++i)
    {
        int c = 0;
        for (int j = 0; j < N; ++j)
        {
            c += adj[j][i];
        }
        if (c != 0)
            for (int j = 0; j < N; ++j)
            {
                n_adj[j][i] = 1.0f * adj[j][i] / c;
            }
    }

    free(adj);

    return n_adj;
}

double** read_from_file(char* file_name, int* size)
{
    int n;
    FILE* fp = fopen(file_name, "r");
    char str[100];
    char ch = getc(fp);
    while (ch == '#')
    {
        fgets(str, 100 - 1, fp);
        sscanf(str, "%*s %d %*s %*d", &n);
        ch = getc(fp);
    }
    (*size) = n;
    int node1, node2;
    ungetc(ch, fp);
    double** adj = malloc(sizeof(double*) * n);
    for (int i = 0; i < n; ++i)
    {
        adj[i] = malloc(sizeof(double) * n);
        memset(adj[i], '\0', n * sizeof(double));
    }
    while (!feof(fp))
    {
        fscanf(fp, "%d %d", &node1, &node2);
        adj[node2][node1] = 1.0;
    }
    fclose(fp);
    int* c = malloc(sizeof(int) * n);
    memset(c, '\0', sizeof(int) * n);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < n; ++j)
        {
            if (adj[i][j] != 0.0)
            {
                c[j]++;
            }
        }
    }
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (c[j] != 0)
                adj[i][j] = adj[i][j] / c[j];
            else
            {
                adj[i][j] = 1.0 / n;
            }
        }
    }

    return adj;
}

int main(int argc, char** argv)
{
    int N = 10;
    double** M;
    if (argc == 2)
    {
        M = read_from_file(argv[1], &N);
    }
    else
    {
        M = generate_weighted_adjancenty(N, 1.5);
        printf("usage : \n%s <path/to/adjancenty_file>\n, DEFAULT : %d vertices, random adjancenty\n", argv[0], N);
    }
    
    int iter = 0;
    double* p = malloc(sizeof(double) * N);
    double* p_new = malloc(sizeof(double) * N);
    // for (int i = 0; i < N; i++)
    // {
    //     for (int j = 0; j < N; j++)
    //     {
    //         printf("%lf ", M[i][j]);
    //     }
    //     printf("\n");
    // }

    for (int i = 0; i < N; i++)
    {
        p[i] = 1.0 / N;
    }
    double d = 0.85;
    double error;
    do
    {
        memset(p_new, '\0', N * sizeof(double));
        error = 0.0;
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
            {
                p_new[i] += M[i][j] * p[j];
            }
        }
        for (int i = 0; i < N; i++)
        {
            p_new[i] = d * p_new[i] + (1.0 - d) / N;
        }
        for (int i = 0; i < N; i++)
        {
            error += fabs(p_new[i] - p[i]);
        }
        SWAP(p_new, p);
        iter++;
    } while (error > epsilon);

    printf("############################################\n");
    printf("PAGE RANK VALUES : \n");
    for (int i = 0; i < N; ++i)
    {
        printf("%lf ", p[i]);
    }
    printf("\n");
    printf("ITERATIONS : \n");
    printf("%d \n", iter);



    free(p);
    free(p_new);
    for (int i = 0; i < N; i++)
    {
        free(M[i]);
    }
    free(M);

    return 0;
}