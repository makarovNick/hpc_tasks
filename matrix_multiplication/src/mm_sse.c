#include "../include/mm_sse.h"

float sdot_sse(int n, float* x, float* y)
{
	int i, n8 = n>>3<<3;
	__m128 vs1, vs2;
	float s, t[4];
	vs1 = _mm_setzero_ps();
	vs2 = _mm_setzero_ps();
	for (i = 0; i < n8; i += 8) {
		__m128 vx1, vx2, vy1, vy2;
		vx1 = _mm_loadu_ps(&x[i]);
		vx2 = _mm_loadu_ps(&x[i+4]);
		vy1 = _mm_loadu_ps(&y[i]);
		vy2 = _mm_loadu_ps(&y[i+4]);
		vs1 = _mm_add_ps(vs1, _mm_mul_ps(vx1, vy1));
		vs2 = _mm_add_ps(vs2, _mm_mul_ps(vx2, vy2));
	}
	for (s = 0.0f; i < n; ++i) s += x[i] * y[i];
	_mm_storeu_ps(t, vs1);
	s += t[0] + t[1] + t[2] + t[3];
	_mm_storeu_ps(t, vs2);
	s += t[0] + t[1] + t[2] + t[3];
	return s;
}

void mm_mul(int N, int M, float **a, int M2, float **b, float** c)
{
	int x = 16;
	float **bT;
	bT = mat_transpose(M, M2, b);
	for (int i = 0; i < N; i += x) {
		for (int j = 0; j < M2; j += x) {
			int je = M2 < j + x? M2 : j + x;
			int ie = N < i + x? N : i + x;
			for (int ii = i; ii < ie; ++ii)
				for (int jj = j; jj < je; ++jj)
					c[ii][jj] += sdot_sse(M, a[ii], bT[jj]);
		}
	}
	mat_destroy(bT);
}