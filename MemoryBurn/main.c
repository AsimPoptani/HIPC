#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <cuda.h>
#include <cuda_runtime.h>

#define N 10240*2560
#define MAX_ERR 1e-6


int main(){
    float *a, *b, *out;
    float *d_a, *d_b, *d_out; 

    // Allocate host memory
    a   = (float*)calloc( N,sizeof(float));


    // Allocate device memory 
    cudaMalloc((void**)&d_a, sizeof(float) * N);
    // int counter=1000;
    while (1){
        // counter--;
        // Transfer data from host to device memory
        cudaMemcpy(d_a, a, sizeof(float) * N, cudaMemcpyHostToDevice);
        cudaMemcpy(a, d_a, sizeof(float) * N, cudaMemcpyDeviceToHost);
    }

}