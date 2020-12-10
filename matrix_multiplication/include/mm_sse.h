#ifndef MM_SSE
#define MM_SSE

#include "matrix.h"

#include <memory.h>
#include <xmmintrin.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

float sdot_sse(int n, float* x, float* y);
void mm_mul(int N, int M, float **a, int M2, float **b, float **c);

#endif