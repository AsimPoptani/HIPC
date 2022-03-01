#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

clock_t start, end;


#define MAX_RAND 10000

struct sphere_t {
	double x;
	double y;
	double z;
	double r;
};

double random_number() {
	// returns a random floating point number between 0.0 and MAX_RAND
	// return (rand()-(rand()/MAX_RAND));
	return fmod(rand() * ((double) rand() / RAND_MAX), MAX_RAND);
	// return (double)((rand()/RAND_MAX) *2.0); 
}

int main(int argc, char *argv[]) {
	start = clock();
	// read N from the first command line argument
	int N = atoi(argv[1]);

	struct sphere_t * sphere = (sphere_t *) malloc(sizeof(struct sphere_t) * N);
	// fill with random numbers
	for (int i = 0; i < N; i++) {
		sphere[i].x = random_number();
		sphere[i].y = random_number();
		sphere[i].z = random_number();
		sphere[i].r = random_number() / 4.0;
	}

	// calculate areas
	double * area = (double *) calloc(N, sizeof(double));
	double constantPI = 4.0 * M_PI;


	// calculate volume
	double * volume = (double *) calloc(N, sizeof(double));
	double constantVolume=(4.0 / 3.0) * M_PI;


	for (int i = 0; i < N; i++) {
		area[i] = constantPI * pow(sphere[i].r, 2.0);
		volume[i] = constantVolume* pow(sphere[i].r, 3.0);
	}

	
	// calculate the number of spheres each sphere intersects
	int * intersects = (int *) calloc(N, sizeof(int));
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < N; j++) {
			if (i == j) continue; // same circle

			// calculate distance between two spheres
			double d = sqrt(pow(sphere[j].x - sphere[i].x, 2.0) + pow(sphere[j].y - sphere[i].y, 2.0) + pow(sphere[j].z - sphere[i].z, 2.0));
			// if the distance is less than the sum of the radii, they intersect
			if (d < (sphere[j].r + sphere[i].r)) intersects[i]++;
		}
	}
	FILE * f = fopen("output.txt", "w");


	// print out all information to the screen (consider piping this to a file)
	printf("x, y, z, r, area, volume, intersects\n");
	for (int i = 0; i < N; i++) {
		fprintf(f,"%lf, %lf, %lf, %lf, %lf, %lf, %d\n", sphere[i].x, sphere[i].y, sphere[i].z, sphere[i].r, area[i], volume[i], intersects[i]);
	}
	fclose(f);
	
	end = clock();
	printf("Time: %lf\n", (double) (end - start) / CLOCKS_PER_SEC);
	return 0;
}