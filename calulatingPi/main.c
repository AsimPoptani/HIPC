#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#define EVENT_LENGTH 4
#include <string.h>
#define NUM_OF_DATA_POINTS 200000000

 void randomStream(int N,void * stream) {

//   Define N times char array
 char *copyStream = malloc(sizeof(char) * (N));

  for (int i = 0; i < N; i++)
  {
    copyStream[i] = rand ();
    // printf("%d\n", copyStream[i]);
  }
    memcpy(stream, copyStream, N);
    free(copyStream);
}


int main(){
    // Amount of data we want to get from urandom
    printf("Setting up the array of random numbers\n");
    uint16_t *random_data=malloc(NUM_OF_DATA_POINTS*2*sizeof(uint16_t));
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

    // Convert the data into a float between 0 and 1
    float * positive_random_data=malloc(NUM_OF_DATA_POINTS*2*sizeof(float));
    for(int i = 0; i < NUM_OF_DATA_POINTS*2; i++){
        positive_random_data[i] =(long double)random_data[i] / UINT16_MAX;
    }
    free(random_data);
    // printf("Finsihed converting data\n");
    // float *x_data= malloc(NUM_OF_DATA_POINTS*sizeof(float));
    // float *y_data= malloc(NUM_OF_DATA_POINTS*sizeof(float));

    for(int i = 0; i < NUM_OF_DATA_POINTS*2; i++){
        positive_random_data[i] = positive_random_data[i] * positive_random_data[i];
    }
    // // Square the data
    // for (int x=0; x<NUM_OF_DATA_POINTS; x++) {
    //     x_data[x] = positive_random_data[x] * positive_random_data[x];
    // }
    // printf("Finished squaring x\n");
    // for (int y=0; y<NUM_OF_DATA_POINTS; y++) {
    //     y_data[y] = positive_random_data[y+NUM_OF_DATA_POINTS] * positive_random_data[y+NUM_OF_DATA_POINTS];
    // }
    // free(positive_random_data);
    printf("Finished squaring y\n");

    // Loop through the data and count the number of points inside the circle
    for (int i=0; i<NUM_OF_DATA_POINTS; i++) {
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
