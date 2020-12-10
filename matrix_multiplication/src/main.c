#include "../include/strassen.h"
#include "../include/mm_sse.h"
#include "../include/matrix.h"
#include "../include/blas.h"

#include <time.h>
#include <getopt.h>
#include <stdio.h>

void mul(int N, int M, float **a, int M2, float **b, float** c)
{
	for (int k = 0; k < N; ++k)
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < M; ++j)
			{
				c[i][j]+= a[i][k] * b[k][j];	
			}
		}
}

int main(int argc, char** argv)
{
    srand(time(NULL));
    int N = 1024;
    int algo = 0;
    int M = -1;
    int c;
	while ((c = getopt(argc, argv, "N:m:a:h")) >= 0) {
		if (c == 'N') N = atoi(optarg);
        if (c == 'M') M = atoi(optarg);
		else if (c == 'a') algo = atoi(optarg);
		else if (c == 'h') {
			fprintf(stderr, "Usage: mm_benchmark [options]\n");
			fprintf(stderr, "Options:\n");
			fprintf(stderr, "  -N INT    number of rows in the matrix [%d]\n", N);
            fprintf(stderr, "  -M INT    number of columns in the matrix [%d]\n", N);
			fprintf(stderr, "  -a INT    matrix multiplication implementation [%d]\n", algo);
			fprintf(stderr, "            0: naive with SSE and matrix transposing - \n");
            fprintf(stderr, "            1: Strassen algorithm - \n");
			fprintf(stderr, "            2: with sdot() from an external CBLAS library\n");
			fprintf(stderr, "            3: with sgemm() from an external CBLAS library\n");
			fprintf(stderr, "  -h        this help message\n");
			return EXIT_FAILURE;
		}
	}
    if (M == -1)
    {
        M = N;
    }
    float** A = mat_gen_random(N, M);
    float** B = mat_gen_random(N, M);
    float** C = mat_init(N, M);

    clock_t start = clock();
    mul(N, M, A, N, B, C);
    // switch (algo)
    // {
    // case 0:
    //     mm_mul(N, M, A, N, B, C);
    //     break;
    
    // case 1:
    //     strassen_mm(N, M, A, N, B, C);
    //     break;
    
    // case 2:
    //     sdot_mm(N, M, A, N, B, C);
    //     break;
    
    // case 3:
    //     sgemm_mm(N, M, A, N, B, C);
    //     break;

    // default:
    //     fprintf(stderr, "Wrong algorithm!\n");
    //     return EXIT_FAILURE;
    // }
    clock_t end = clock();

    printf("TIME ELAPSED :  %f\n", 1.0 * (end - start)/CLOCKS_PER_SEC);

    return EXIT_SUCCESS;
}