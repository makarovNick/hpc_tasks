#include <math.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
#include <stdio.h>

#define MAX_NAME_SIZE 12

int main(int argc, char** argv)
{
    int rank, size;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    char* message = malloc(sizeof(char) * size * MAX_NAME_SIZE);
    int* order = malloc(sizeof(int) * size);
    char name[100];

    sprintf(name, "player_%d\n", rank);

    if (rank == 0 && size != 1)
    {
        memset(message, '\0', sizeof(char) * size * MAX_NAME_SIZE);
        memset(order + 1, '\0', size * sizeof(int));
        order[0] = 1;

        int dest = 0;
        while(!dest)
        {
            dest = rand() % size;
        }
        strcat(message, name);
        order[dest] = 2;
        MPI_Ssend(message, size * MAX_NAME_SIZE, MPI_CHAR, dest, 1, MPI_COMM_WORLD);
        MPI_Ssend(order, size, MPI_INT, dest, 1, MPI_COMM_WORLD);
    }
    else
    {
        MPI_Recv(message, size * MAX_NAME_SIZE, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(order, size, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        int is_last = 1;
        for (int i = 0; i < size; i++)
        {
            if (order[i] == 0)
            {
                is_last = 0;
                break;
            }
        }
        strcat(message, name);
        if (!is_last)
        {
            int dest;
            do
            {
                dest = rand() % size;
            } while(order[dest]);
            order[dest] = 1 + order[rank];
            MPI_Ssend(message, size * MAX_NAME_SIZE, MPI_CHAR, dest, 1, MPI_COMM_WORLD);
            MPI_Ssend(order, size, MPI_INT, dest, 1, MPI_COMM_WORLD);
        }
        else
        {
            printf("MESSAGE : \n");
            printf("%s", message);
            printf("\nORDER : \n");
            for (int i = 0; i < size; i++)
            {
                printf("process #%d was %d%s\n", i, order[i], (order[i] % 10 == 1 ? "st" : (order[i] % 10 == 2 ? "nd" : (order[i] % 10 == 3 ? "rd" : "th"))));
            }
        }
    }

    free(message);
    free(order);

    MPI_Finalize();
    
    return 0;
}