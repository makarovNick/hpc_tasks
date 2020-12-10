#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>

#define SWAP(x, y)                                                                                 \
    do                                                                                             \
    {                                                                                              \
        typeof(x) _x = x;                                                                          \
        typeof(y) _y = y;                                                                          \
        x = _y;                                                                                    \
        y = _x;                                                                                    \
    } while (0)

char value_by_rule(char a, char b, char c, int rule)
{
    return (rule & (1 << (0 | (a << 2) | (b << 1) | (c << 0)))) ? 1 : 0;
}

#define SIZE 100000
#define ITERATION 50000

int main(int argc, char** argv)
{
    char* arr = malloc(sizeof(char) * SIZE);
    char* temp = malloc(sizeof(char) * SIZE);
    int rule = 0b01101110;
    memset(arr, '\0', SIZE * sizeof(char));
    arr[SIZE - 1] = 1;
    int start = clock();
    for (int j = 0; j < ITERATION; j++)
    {

        temp[0] = value_by_rule(arr[SIZE - 1], arr[0], arr[1], rule);
        for (int i = 1; i < SIZE - 1; i++)
        {
            temp[i] = value_by_rule(arr[i - 1], arr[i], arr[i + 1], rule);
        }
        temp[SIZE - 1] = value_by_rule(arr[SIZE - 2], arr[SIZE - 1], arr[0], rule);

        SWAP(temp, arr);
        // for (int z = 0; z < SIZE; z++)
        // {
        //     printf("%d", (int) arr[z]);
        // }
        
        // printf("\n");
        // usleep(50000);
    }
    int end = clock();

    printf("TIME ELAPSED : %lf\n", (end - start) * 1.0 / CLOCKS_PER_SEC);
    free(arr);
    free(temp);


    return 0;
}