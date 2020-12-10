#ifndef STRASSEM
#define STRASSEN
#include "matrix.h"
#include "mm_sse.h"
  
void strassen_mm(int N, int M, float **a, int M2, float **b, float** c);

#endif