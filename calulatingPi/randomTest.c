
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

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

int main()
{
    uint16_t * stream=malloc(sizeof(uint16_t)*10);
    randomStream(sizeof(uint16_t)*10,stream);
    
    for (int i = 0; i < 10; i++)
    {
        printf("%d\n", stream[i]);
    }
    // printf("%d %d\n", i, *stream);
    free(stream);
    return 0;
}