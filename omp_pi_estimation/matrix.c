#include "matrix.h"

float **mat_init(int N, int M)
{
	float **m;
	int i;
	m = (float**)malloc(N * sizeof(float*));
	m[0] = (float*)calloc(N * M, sizeof(float));
    memset(m[0], '\0', N * M * sizeof(float));
	for (i = 1; i < N; ++i)
		m[i] = m[i-1] + M;
	return m;
}

void mat_destroy(float **m)
{
	free(m[0]); free(m);
}

float **mat_gen_random(int N, int M)
{
	float **m;
	int i, j;
	m = mat_init(N, M);
	for (i = 0; i < N; ++i)
		for (j = 0; j < M; ++j)
			m[i][j] = 0.9147123f * (rand() % 10000);
	return m;
}

float **mat_transpose(int N, int M, float ** a)
{
	int i, j;
	float **m;
	m = mat_init(M, N);
	for (i = 0; i < N; ++i)
		for (j = 0; j < M; ++j)
			m[j][i] = a[i][j];
	return m;
}