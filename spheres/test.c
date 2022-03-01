#include <papi.h>
#include <stdio.h>

int main() {

   static int events[] = { PAPI_TOT_INS, PAPI_L1_DCM };
       long long counters[2];

   PAPI_start_counters(events, 2);

   int sum = 0;
   int i = 0;
   for (i = 0; i < 10; i++) {
      sum += i;
   }   
   
   PAPI_stop_counters(counters, 2);
   
   printf ("L1 data cache misses: %lli\n", counters[0]);
}
