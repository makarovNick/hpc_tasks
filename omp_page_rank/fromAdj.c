#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

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

    double** n_adj = (double**) malloc(sizeof(double*) * N);
    for (int i = 0; i < N; ++i)
    {
        n_adj[i] = (double*) malloc(sizeof(double) * N);
        memset(n_adj[i], '\0', sizeof(double) * N);
    }
    
    for (int i = 0; i < N; ++i)
    {
        int c = 0;
        for (int j = 0; j < N; ++j)
        {
            c += adj[j][i];
        }
        if (c!= 0)
        for (int j = 0; j < N; ++j)
        {
            n_adj[j][i] = 1.0f * adj[j][i] / c;
        }
    }
    
    free(adj);

    return n_adj;
}

int main(int argc, char** argv)
{
    char* f = "adjancenty.txt";
    int N = 10;
    if (argc == 3)
    {
        f = argv[1];
        N = atoi(argv[2]);
    }
    else if (argc == 2)
    {
        N = atoi(argv[1]);
    }
    else
    {
        printf("usage : \n%s <path/to/adjancenty_file> <vert_count>\nDEFAULT : %d vertices, %s\n", argv[0], N, f);
        printf("%s <vert_count>\nDEFAULT : %s\n", argv[0], f);
    }
    
    FILE* file = fopen(f, "w+");
    double** M = generate_weighted_adjancenty(N, 1.5);
    int c = 0;
    for (int i = 0; i < N; i++)
    {
        for (int h = 0; h < N; h++)
        {
            if (M[i][h] != 0) c++;
            printf("M[%d][%d]=%lf; ", i, h, M[i][h]);
        }
        printf("\n");
    }
    fprintf(file, "# Nodes: %d Edges: %d\n", N, c);
    
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; ++j)
        {
            if (M[i][j] != 0)
            {
                fprintf(file, "%d %d\n", i, j);
            }
        }
    }
    
    fclose(file);
    



    return 0;
}