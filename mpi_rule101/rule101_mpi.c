#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>
#include <string.h>
#include <stdio.h>

#define SWAP(x, y)                                                                                 \
    do                                                                                             \
    {                                                                                              \
        typeof(x) _x = x;                                                                          \
        typeof(y) _y = y;                                                                          \
        x = _y;                                                                                    \
        y = _x;                                                                                    \
    } while (0)

#define CELLS_PER_PROCESSOR 50000
#define ITERATIONS 100000
#define RANDOM_INITIALIZATION 0
#define PRINT_EACH 0

// uncomment for omp
// #define MULTITHREAD

char value_by_rule(char* a, int rule)
{
    return (rule & (1 << (0 | (a[0] << 2) | (a[1] << 1) | (a[2] << 0)))) ? 1 : 0;
}

int main(int argc, char** argv)
{
    int rank, size;
    double start, end;
    int rule = 0b01101110;
    if (argc == 2)
    {
        rule = atoi(argv[1]);
    }
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    srand(time(NULL) ^ rank);

    char* temp = malloc(CELLS_PER_PROCESSOR * sizeof(char) + 2);
    char* chunk = malloc(CELLS_PER_PROCESSOR * sizeof(char) + 2);
    char * res = malloc(CELLS_PER_PROCESSOR * size * sizeof(char));

    MPI_Barrier(MPI_COMM_WORLD); 
    start = MPI_Wtime();


    if (RANDOM_INITIALIZATION)
    {
        for (int i = 0; i < CELLS_PER_PROCESSOR + 1; i++)
        {
            chunk[i] = rand() % 2;
        }
    }
    else
    {
        for (int i = 1; i < CELLS_PER_PROCESSOR + 1; i++)
        {
            chunk[i] = 0;
        }
        if (rank == size - 1)
        {
            chunk[CELLS_PER_PROCESSOR] = 1;
        }
    }

    for (int i = 0; i < ITERATIONS; ++i)
    {
        if (PRINT_EACH)
        {
            MPI_Gather(chunk + 1, CELLS_PER_PROCESSOR, MPI_CHAR, res, CELLS_PER_PROCESSOR, MPI_CHAR, 0, MPI_COMM_WORLD);
            if (rank == 0)
            {
                for (int z = 0; z < CELLS_PER_PROCESSOR * size; z++)
                {
                    printf("%d", (int) res[z]);
                }
                printf("\n");
            }
        }

        if (rank % 2 != 0)
        {
            MPI_Recv(chunk + CELLS_PER_PROCESSOR + 1, 1, MPI_CHAR, (rank == size - 1 ? 0 : rank + 1), MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(chunk + CELLS_PER_PROCESSOR , 1, MPI_CHAR, (rank == size - 1 ? 0 : rank + 1), 1, MPI_COMM_WORLD);
            MPI_Recv(chunk, 1, MPI_CHAR, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(chunk + 1, 1, MPI_CHAR, rank - 1, 1, MPI_COMM_WORLD);
        }
        else
        {
            MPI_Send(chunk + 1, 1, MPI_CHAR, (rank == 0 ? size - 1 : rank - 1), 1, MPI_COMM_WORLD);
            MPI_Recv(chunk, 1, MPI_CHAR,     (rank == 0 ? size - 1 : rank - 1), MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(chunk + CELLS_PER_PROCESSOR , 1, MPI_CHAR, rank + 1, 1, MPI_COMM_WORLD);
            MPI_Recv(chunk + CELLS_PER_PROCESSOR + 1, 1, MPI_CHAR, rank + 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
#ifdef MULTITHREAD
#pragma omp parallel for 
#endif
        for (int j = 0; j < CELLS_PER_PROCESSOR + 1; j++)
        {
            temp[j + 1] = value_by_rule(chunk + j, rule);
        }
        SWAP(temp, chunk);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    end = MPI_Wtime();

    if (rank == 0 )
    {
        printf("TIME ELAPSED : %lf\n", end - start);
    }

    free(chunk);
    free(res);
    free(temp);
    MPI_Finalize();

    return 0;
}