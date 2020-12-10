#ifndef MATRIX
#define MATRIX

#include <math.h>
#include <stdlib.h>
#include <memory.h>

float **mat_init(int N, int M);
void mat_destroy(float **m);
float **mat_gen_random(int N, int M);
float **mat_transpose(int N, int M, float ** a);

#endif