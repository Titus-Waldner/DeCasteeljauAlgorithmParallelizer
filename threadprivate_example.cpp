#include <omp.h>
#include <stdio.h>
#include <windows.h>
int globalVar = 5;
#pragma omp threadprivate(globalVar)

int main() {
    #pragma omp parallel num_threads(4)
    {
        globalVar += omp_get_thread_num()+1;  // Each thread modifies its own copy

        Sleep(1+omp_get_thread_num()*100);
        printf("Thread %d has globalVar value: %d\n", omp_get_thread_num(), globalVar);
    }
    printf("GlobalVar value: %d\n", globalVar);
    #pragma omp parallel num_threads(4)
    {
        globalVar += omp_get_thread_num()+1;  // Each thread modifies its own copy

        Sleep(1+omp_get_thread_num()*100);
        printf("Thread %d has globalVar value: %d\n", omp_get_thread_num(), globalVar);
    }
    printf("GlobalVar value: %d\n", globalVar);
    return 0;
}