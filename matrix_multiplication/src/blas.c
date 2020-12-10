#include "../include/blas.h"

void sdot_mm(int N, int M, float **a, int M2, float **b, float** c)
{
	int i, j, n_b_rows = M;
	float **bT;
	// m = mat_init(N, M2);
	bT = mat_transpose(n_b_rows, M2, b);
	for (i = 0; i < N; ++i)
		for (j = 0; j < M2; ++j)
			c[i][j] = cblas_sdot(M, a[i], 1, bT[j], 1);
	mat_destroy(bT);
}

void sgemm_mm(int N, int M, float **a, int M2, float **b, float** c)
{
	cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, N, M2, M, 1.0f, a[0], N, b[0], M, 0.0f, c[0], N);
}
