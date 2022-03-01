#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <papi.h>
#include <omp.h>

#define EVENT_LENGTH 4



uint64_t NUM_OF_DATA_POINTS =1000000;
int main(){
    static int events[] = { PAPI_L1_TCM, PAPI_DP_OPS,PAPI_TOT_IIS,PAPI_TOT_INS };
    long long counters[EVENT_LENGTH];
    PAPI_start_counters(counters, EVENT_LENGTH);
    // Amount of data we want to get from urandom
    printf("Setting up the array of random numbers\n");
    uint16_t *random_data=malloc(NUM_OF_DATA_POINTS*2*sizeof(uint16_t));
    // if (!random_data) { perror("malloc arr"); exit(EXIT_FAILURE); };
    printf("Address of random_data: %p\n", random_data);
    printf("Getting data from urandom...\n");
    size_t bytesToBeRead = sizeof(uint16_t)*2*NUM_OF_DATA_POINTS;
    printf("Bytes to be read: %zu\n", bytesToBeRead);
    // Read data from urandom
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) { perror("open"); exit(EXIT_FAILURE); };
    ssize_t bytesRead = read(fd, random_data, bytesToBeRead);
    if (bytesRead < 0) { perror("read"); exit(EXIT_FAILURE); };
    printf("Bytes read: %zu\n", bytesRead);
    close(fd);
    printf("Closed urandom\n");

    // arc4random_buf(random_data, bytesToBeRead);
    
    // Count
    uint64_t inside_circle = 0;
    uint64_t outside_circle = 0;

    // Convert the data into a float between 0 and 1
    float * positive_random_data=malloc(NUM_OF_DATA_POINTS*2*sizeof(float));
    #pragma omp parallel for
    for(int i = 0; i < NUM_OF_DATA_POINTS*2; i++){
        positive_random_data[i] =(long double)random_data[i] / UINT16_MAX;
    }
    free(random_data);
    printf("Finsihed converting data\n");
    float *x_data= malloc(NUM_OF_DATA_POINTS*sizeof(float));
    float *y_data= malloc(NUM_OF_DATA_POINTS*sizeof(float));

    // Square the data
    #pragma omp parallel for
    for (int x=0; x<NUM_OF_DATA_POINTS; x++) {
        x_data[x] = positive_random_data[x] * positive_random_data[x];
    }
    printf("Finished squaring x\n");
    #pragma omp parallel for
    for (int y=0; y<NUM_OF_DATA_POINTS; y++) {
        y_data[y] = positive_random_data[y+NUM_OF_DATA_POINTS] * positive_random_data[y+NUM_OF_DATA_POINTS];
    }
    free(positive_random_data);
    printf("Finished squaring y\n");

    // Loop through the data and count the number of points inside the circle
    #pragma omp parallel for 
    for (int i=0; i<NUM_OF_DATA_POINTS; i++) {
        if (x_data[i] + y_data[i] <= 1) {
            #pragma omp critical
            inside_circle++;
        } else {
            #pragma omp critical
            outside_circle++;
        }
    }
    printf("Finished adding x\n");




    printf("Inside circle: %llu\n", inside_circle);
    printf("Outside circle: %llu\n", outside_circle);
    printf("Pi is: %f\n", 4.0*(float)(inside_circle)/(float)(inside_circle + outside_circle));
    PAPI_stop_counters(counters, EVENT_LENGTH);
    printf ("L1 data cache misses: %llu\n", counters[0]);
    printf ("Data cache misses: %llu\n", counters[1]);
    printf ("Instruction cache misses: %llu\n", counters[2]);
    printf ("Instructions: %llu\n", counters[3]);



    return 0;
}