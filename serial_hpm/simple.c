/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

#include <stdio.h>
#include <stdlib.h>

// add extern "C" for C++ codes
void HPM_Init(void);
void HPM_Start(char *);
void HPM_Stop(char *);
void HPM_Print(void);

int main(int argc, char * argv[])
{
   double sum, * x, * y;
   int i, n;

   n = 100000000;
   x = (double *) malloc(n*sizeof(double));
   y = (double *) malloc(n*sizeof(double));

#pragma omp parallel for
   for (i=0; i<n; i++) x[i] = 1.0  + (double) (i%10);
#pragma omp parallel for
   for (i=0; i<n; i++) y[i] = 1.0  + (double) (i%20);

   HPM_Init();

   HPM_Start("loop");
   sum = 0;
#pragma omp parallel for reduction(+:sum)
   for (i=0; i<n; i++) sum += x[i]*y[i];

   HPM_Stop("loop");

   printf("sum = %.6lf\n", sum);

   HPM_Print();

   return 0;
}
