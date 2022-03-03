#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <cuda.h>
#include <cuda_runtime.h>

#define N 1024*256
#define MAX_ERR 1e-6


int main(){
    float *a, *b, *out;
    float *d_a, *d_b, *d_out; 

    // Allocate host memory
    a   = (float*)calloc(sizeof(float) * N);


    // Allocate device memory 
    cudaMalloc((void**)&d_a, sizeof(float) * N);

    while (1){
        // Transfer data from host to device memory
        cudaMemcpy(d_a, a, sizeof(float) * N, cudaMemcpyHostToDevice);
        cudaMemcpy(a, d_a, sizeof(float) * N, cudaMemcpyDeviceToHost);
    }

}