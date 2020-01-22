/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

#include <stdio.h>
#include <math.h>

void HPM_Prof_init(void); 
void HPM_Prof_start(void);
void HPM_Prof_stop(void);
void HPM_Prof_write(void);

double sub(double);

int main(int argc, char * argv[])
{
   double x, val;

   HPM_Prof_init();
   HPM_Prof_start();

   x = 3.14159;
   val = sub(x);

   HPM_Prof_stop();
   HPM_Prof_write();

   printf("got val = %.4le\n", val);

   return 0;
}
