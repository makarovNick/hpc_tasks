#include "../include/strassen.h"

#define SPLIT_SIZE 1024

void strassen_mm(int N, int M, float **a, int M2, float **b, float** c) 
{   
    if (N <= SPLIT_SIZE || M <= SPLIT_SIZE || M2 <= SPLIT_SIZE)  
    {
        mm_mul(N, M, a, M2, b, c);
        return;
    } 

  
    int adjN = (N >> 1) + (N & 1); 
    int adjL = (M >> 1) + (M & 1); 
    int adjM = (M2 >> 1) + (M2 & 1); 
  
    float** As[2][2];
    float** Bs[2][2]; 
    for (int x = 0; x < 2; x++) 
    { 
        for (int y = 0; y < 2; y++) 
        {
            Bs[x][y] = mat_init(adjN, adjM); 
            As[x][y] = mat_init(adjN, adjL); 
            for (int i = 0; i < adjN; i++) 
            { 
                for (int j = 0; j < adjL; j++) 
                { 
                    int I = i + (x & 1) * adjN; 
                    int J = j + (y & 1) * adjL;
                    int I1 = i + (x & 1) * adjL; 
                    int J2 = j + (y & 1) * adjM; 
                    Bs[x][y][i][j] = (I1 < M && J2 < M2) ? b[I1][J2] : 0; 
                    As[x][y][i][j] = (I < N && J < M) ? a[I][J] : 0; 
                } 
            } 
        } 
    } 
  
    float** s[10];
    s[1] = mat_init(adjN, adjL);
    s[2] = mat_init(adjN, adjL);
    s[4] = mat_init(adjN, adjL);
    s[6] = mat_init(adjN, adjL);
    s[8] = mat_init(adjN, adjL); 
    s[0] = mat_init(adjL, adjM);
    s[3] = mat_init(adjL, adjM);
    s[5] = mat_init(adjL, adjM);
    s[7] = mat_init(adjL, adjM);
    s[9] = mat_init(adjL, adjM); 
    for (int j = 0; j < adjL; j++) 
    { 
        for (int k = 0; k < adjM; k++) 
        { 
            s[0][j][k] = Bs[0][1][j][k] - Bs[1][1][j][k];
            s[3][j][k] = Bs[1][0][j][k] - Bs[0][0][j][k];
            s[5][j][k] = Bs[0][0][j][k] + Bs[1][1][j][k];
            s[7][j][k] = Bs[1][0][j][k] + Bs[1][1][j][k];
            s[9][j][k] = Bs[0][0][j][k] + Bs[0][1][j][k]; 
        }
        for (int k = 0; k < adjL; k++) 
        { 
            s[1][k][j] = As[0][0][k][j] + As[0][1][k][j];
            s[2][k][j] = As[1][0][k][j] + As[1][1][k][j];
            s[4][k][j] = As[0][0][k][j] + As[1][1][k][j];
            s[6][k][j] = As[0][1][k][j] - As[1][1][k][j];
            s[8][k][j] = As[0][0][k][j] - As[1][0][k][j];   
        } 
    }   
  
    float** p[7];
    for (int i = 0; i < 7; ++i)
    {
        p[i] = mat_init(adjN, adjM);
    }
    
    strassen_mm(adjN, adjL, As[0][0], adjM, s[0], p[0]); 
    strassen_mm(adjN, adjL, s[1],adjM, Bs[1][1],  p[1]); 
    strassen_mm(adjN, adjL, s[2],  adjM, Bs[0][0], p[2]); 
    strassen_mm(adjN, adjL, As[1][1], adjM, s[3], p[3]); 
    strassen_mm(adjN, adjL, s[4], adjM, s[5],  p[4]); 
    strassen_mm(adjN, adjL, s[6], adjM, s[7],  p[5]); 
    strassen_mm(adjN, adjL, s[8], adjM, s[9],  p[6]); 
  
    for (int i = 0; i < adjN; i++) 
    { 
        for (int j = 0; j < adjM; j++) 
        { 
            c[i][j] = p[4][i][j] + p[3][i][j] - p[1][i][j] + p[5][i][j]; 
            if (j + adjM < M2) 
                c[i][j + adjM] = p[0][i][j] + p[1][i][j]; 
            if (i + adjN < N) 
                c[i + adjN][j] = p[2][i][j] + p[3][i][j]; 
            if (i + adjN < N && j + adjM < M2) 
                c[i + adjN][j + adjM] = p[4][i][j] + p[0][i][j] - p[2][i][j] - p[6][i][j]; 
        } 
    } 
  
    for (int x = 0; x < 2; x++) 
    { 
        for (int y = 0; y < 2; y++) 
        { 
            mat_destroy(As[x][y]);
        } 
    } 
  
    for (int x = 0; x < 2; x++) 
    { 
        for (int y = 0; y < 2; y++) 
        { 
            mat_destroy(Bs[x][y]);
        } 
    } 
  
    for (int i = 0; i < 10; i++) 
    { 
        mat_destroy(s[i]);
    } 

    for (int i = 0; i < 7; i++) 
    { 
        mat_destroy(p[i]);
    } 
} 