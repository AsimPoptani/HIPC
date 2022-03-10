#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>
#include <cuda.h>
#include <cuda_runtime.h>
 #include "device_launch_parameters.h"

#define N 1e+10
#define MAX_ERR 1e-6


__global__ void multiply(int * a,int b,int n)
{
    int idx = threadIdx.x+ blockIdx.x * blockDim.x;
    if (idx < n)
        a[idx] = a[idx] * b;
    // a[idx] *= b;
}

int main(){
    int *a, *b, *out;
    int *d_a, *d_b, *d_out; 

    // Allocate host memory
    a   = (int*)calloc( N,sizeof(int));


    // Allocate device memory 
    cudaMalloc((void**)&d_a, sizeof(int) * N);
    // int counter=1000;
    while (1){
        // counter--;
        // Transfer data from host to device memory
        cudaMemcpy(d_a, a, sizeof(int) * N, cudaMemcpyHostToDevice);
        multiply<<<1,N>>>(d_a,2,N);
        cudaMemcpy(a, d_a, sizeof(int) * N, cudaMemcpyDeviceToHost);
    }

}
