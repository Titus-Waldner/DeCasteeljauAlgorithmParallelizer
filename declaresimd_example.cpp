#include <stdio.h>
#include <omp.h>

#pragma omp declare simd
float add(float a, float b) 
{
    return a + b;
}

void compute(float* A, float* B, float* C, int N) 
{
    #pragma omp for simd
    for(int i=0; i<N; i++) {
        C[i] = add(A[i], B[i]);
    }
}

int main() 
{
    const int N = 10;
    float A[N], B[N], C[N];
    for (int i = 0; i < N; i++) 
    {
        A[i] = i;
        B[i] = N - i;
    }

    compute(A, B, C, N);

    for (int i = 0; i < N; i++) 
    {
        printf("%f + %f = %f\n", A[i], B[i], C[i]);
    }

    return 0;
}
