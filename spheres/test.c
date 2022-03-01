// #include <papi.h>
#include <stdio.h>
#include <omp.h>

int addNTimes(int n) {
   __uint128_t i;
   for (int x = 0; x < n; x++) {
      i=(x+1)*(i+1);
      
   }
   return i;
}

int main() {

   // static int events[] = { PAPI_TOT_INS, PAPI_L1_DCM };
   //     long long counters[2];

   // PAPI_start_counters(events, 2);

   __uint128_t sum = 0;
   int i = 0;
   #pragma omp parallel for reduction(+:sum)
   for (i = 0; i < 100000; i++) {
      __uint128_t x = addNTimes(i);
      printf("Thread %d\n", omp_get_thread_num());
      sum += x;
   } 
   printf("%lld\n", sum);  
   
   // PAPI_stop_counters(counters, 2);
   
   // printf ("L1 data cache misses: %lli\n", counters[0]);
}
