#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <papi.h>
#include <omp.h>
#define MAX_RAND 10000

struct sphere_t {
	double x;
	double y;
	double z;
	double r;
};

double random_number() {
	// returns a random floating point number between 0.0 and MAX_RAND
	return fmod(rand() * ((double) rand() / RAND_MAX), MAX_RAND); 
}

int main(int argc, char *argv[]) {
    int EVENT_LENGTH=4;
//    PAPI_library_init(PAPI_VER_CURRENT);
   static int events[] = { PAPI_L1_TCM, PAPI_DP_OPS,PAPI_TOT_IIS,PAPI_TOT_INS };
 //   long long int vals[4] = {};
  //  PAPI_start_counters(events, 4);
//    static int events[] = { PAPI_TOT_INS, PAPI_L1_DCM };
       long long counters[EVENT_LENGTH];
 PAPI_start_counters(counters, EVENT_LENGTH);

	// read N from the first command line argument
	int N = atoi(argv[1]);

	struct sphere_t * sphere = malloc(sizeof(struct sphere_t) * N);
	// fill with random numbers
    #pragma omp parallel for
	for (int i = 0; i < N; i++) {
		sphere[i].x = random_number();
		sphere[i].y = random_number();
		sphere[i].z = random_number();
		sphere[i].r = random_number() / 4.0;
	}

	// calculate areas
	double * area = calloc(N, sizeof(double));
    #pragma omp parallel for
	for (int i = 0; i < N; i++) {
		area[i] = 4.0 * M_PI * pow(sphere[i].r, 2.0);
	}

	// calculate volume
	double * volume = calloc(N, sizeof(double));
    #pragma omp parallel for
	for (int i = 0; i < N; i++) {
		volume[i] = (4.0 / 3.0) * M_PI * pow(sphere[i].r, 3.0);
	}
	
	// calculate the number of spheres each sphere intersects
	int * intersects = calloc(N, sizeof(int));
    #pragma omp parallel for collapse(2)
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) continue; // same circle

			// calculate distance between two spheres
			double d = sqrt(pow(sphere[j].x - sphere[i].x, 2.0) + pow(sphere[j].y - sphere[i].y, 2.0) + pow(sphere[j].z - sphere[i].z, 2.0));
			// if the distance is less than the sum of the radii, they intersect
			if (d < (sphere[j].r + sphere[i].r)) intersects[i]++;
		}
	}

	// print out all information to the screen (consider piping this to a file)
	fprintf(stderr,"x, y, z, r, area, volume, intersects\n");
    #pragma omp parallel for
	for (int i = 0; i < N; i++) {
		fprintf(stderr,"%lf, %lf, %lf, %lf, %lf, %lf, %d\n", sphere[i].x, sphere[i].y, sphere[i].z, sphere[i].r, area[i], volume[i], intersects[i]);
	}
    PAPI_read_counters(counters, EVENT_LENGTH);
    printf("PAPI L1_TCM: %lld\n", counters[0]);
    printf("PAPI DP_OPS: %lld\n", counters[1]);
   printf("PAPI TOT_IIS: %lld\n", counters[2]);
   printf("PAPI TOT_INS: %lld\n", counters[3]);
    return 0;
}
