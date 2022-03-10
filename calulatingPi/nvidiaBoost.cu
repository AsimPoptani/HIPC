#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <cuda.h>
#include <cuda_runtime.h>
#define EVENT_LENGTH 4
#include <string.h>
// #define NUM_OF_DATA_POINTS 2000000000
// #define BLOCKS (NUM_OF_DATA_POINTS/2)*2
const uint64_t NUM_OF_DATA_POINTS =550000000;
 void randomStream(int N,void * stream) {

//   Define N times char array
 char *copyStream = (char *) malloc(sizeof(char) * (N));
    printf("Address of copyStream: %p\n", copyStream);
  for (uint64_t i = 0; i < N; i++)
  {
    //   printf("%d\n",i);
    copyStream[i] = rand ();
    // printf("%d\n", copyStream[i]);
  }
  printf("Memory allocated\n");
    memcpy(stream, copyStream, N);
    free(copyStream);
}

__global__ void square(float * data){
    //  Blocks: 10000,20000
    // 0,0
    // 0,1
    // 0,2
    uint64_t idx = (threadIdx.x + (blockIdx.x * blockDim.x))*5;
    // int idx = blockIdx.x * blockDim.x + threadIdx.x;
    
    if (idx < NUM_OF_DATA_POINTS*2)
    {
        for (int index=idx;index<idx+5;index++)
        {
        float x = data[idx] * data[idx];
        // printf(" Data: %f Squared: %f ID:%d, Thread :%d , Block: %d , Block Dim: %d \n",data[idx],x, idx,threadIdx.x,blockIdx.x,blockDim.x);
        data[idx] = x;
        }
        

    }
}

int main(){
    // Amount of data we want to get from urandom
    printf("Setting up the array of random numbers\n");
    uint16_t *random_data=(uint16_t *) malloc(NUM_OF_DATA_POINTS*2*sizeof(uint16_t));
    // if (!random_data) { perror("malloc arr"); exit(EXIT_FAILURE); };
    printf("Address of random_data: %p\n", random_data);
    printf("Getting data from urandom...\n");
    uint64_t bytesToBeRead = sizeof(uint16_t)*2*NUM_OF_DATA_POINTS;
    printf("Bytes to be read: %llu\n", bytesToBeRead);
    // Read data from urandom
   // int fd = open("/dev/urandom", O_RDONLY);
    //if (fd < 0) { perror("open"); exit(EXIT_FAILURE); };
    //ssize_t bytesRead = read(fd, random_data, bytesToBeRead);
    //if (bytesRead < 0) { perror("read"); exit(EXIT_FAILURE); };
    randomStream(bytesToBeRead,random_data);
    // printf("Bytes read: %zu\n", bytesRead);
    // close(fd);
    printf("Closed urandom\n");

    // arc4random_buf(random_data, bytesToBeRead);
    
    // Count
    uint64_t inside_circle = 0;
    uint64_t outside_circle = 0;

    // float *positive_random_data;
    // Convert the data into a float between 0 and 1
    float * positive_random_data=(float *) malloc(NUM_OF_DATA_POINTS*2*sizeof(float));
    // cudaMallocManaged(&positive_random_data, NUM_OF_DATA_POINTS*2*sizeof(float));

    for(uint64_t i = 0; i < NUM_OF_DATA_POINTS*2; i++){
        positive_random_data[i] =(long double)random_data[i] / UINT16_MAX;
    }
    free(random_data);
    printf("Finsihed converting data\n");

    printf("Copying data to GPU\n");
    float *copy;
    cudaMalloc((void**)&copy, NUM_OF_DATA_POINTS*2*sizeof(float));
    cudaMemcpy(copy,positive_random_data,NUM_OF_DATA_POINTS*2*sizeof(float),cudaMemcpyHostToDevice);
    square<<<3906250,1024>>>(copy);
    cudaMemcpy(positive_random_data,copy,NUM_OF_DATA_POINTS*2*sizeof(float),cudaMemcpyDeviceToHost);
    // cudaDeviceSynchronize();
    printf("Finished squaring y\n");

    // Loop through the data and count the number of points inside the circle
    for (uint64_t i=0; i<NUM_OF_DATA_POINTS; i++) {
        // printf("Indexs: %d, %d\n",i,i+NUM_OF_DATA_POINTS);
        if (positive_random_data[i] + positive_random_data[i+NUM_OF_DATA_POINTS] <= 1) {
            inside_circle++;
        } else {
            outside_circle++;
        }
    }
    printf("Finished adding x\n");




    printf("Inside circle: %llu\n", inside_circle);
    printf("Outside circle: %llu\n", outside_circle);
    printf("Pi is: %f\n", 4.0*(float)(inside_circle)/(float)(inside_circle + outside_circle));



    return 0;
}
