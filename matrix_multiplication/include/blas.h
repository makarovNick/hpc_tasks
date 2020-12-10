#ifndef BLAS
#define BLAS

#include "matrix.h"
#include <OpenBLAS/cblas.h>

void sdot_mm(int N, int M, float **a, int M2, float **b, float** c);
void sgemm_mm(int N, int M, float **a, int M2, float **b, float** c);

#endif