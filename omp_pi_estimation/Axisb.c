#include "mm_sse.h"
#include "mt19937_64.h"
#include "matrix.h"

#define epsilon 1e-4
#define MAX_ITER 1000


int main(int argc, char** argv)
{
    int N = 1000;

    float** A = mat_gen_random(N, N);
    float* x = (float*) malloc(sizeof(float) * N);
    float* b = (float*) malloc(sizeof(float) * N);

    init_genrand64(time(NULL));
    // for (int i = 0; i < N; i++)
    // {
    //     for (int j = 0; j < N; j++)
    //     {
    //         scanf("%f", &A[i][j]);
    //     }
    // }
    // for (int j = 0; j < N; j++)
    // {
    //     scanf("%f", &b[j]);
    // }
    for (int i = 0; i < N; ++i)
    {
        x[i] = genrand64_real3();
        b[i] = genrand64_real3() * (rand() / 4);
    }

    float norm = epsilon * 2;
    int i;
    float temp = 0.0f;

    for (int z = 0; z < MAX_ITER && norm > epsilon; ++z)
    {
#pragma omp parallel shared(x, A, b) private(norm) num_threads(1)
        {
            norm = 0.0f;
#pragma omp parallel for shared(x, norm) private(temp) num_threads(1)
            for (i = 0; i < N; ++i)
            {
                for (int j = 0; j < N; ++j)
                {
                    temp += x[j] * (i == j ? 0 : - A[i][j] / A[i][i]);
                }
                temp += b[i] / A[i][i];
#pragma omp atomic
                norm += (temp - x[i]) * (temp - x[i]); 
                x[i] = temp;
                temp = 0.0f;
            }
            norm = sqrtf(norm);
        }
    } 

    // for (size_t j = 0; j < N; j++)
    // {
    //     printf("%f ", x[j]);
    // }

    mat_destroy(A);
    free(x);
    free(b);

    return 0;
}