/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

struct intPair { int src; int dest; };

typedef union { long offset; struct intPair ranks; } unType;

struct eventstruct {
                     double tbeg;
                     double tend;
                     int taskid;
                     int eventid;
                     unType UN;  
                     int bytes;
                     int parent;
                     int grandparent;
                     int ioflag;
                   };

struct eventstruct event;

#define EVENT_SIZE 48

int main(int argc, char * argv[])
{
   double tmin, tmax;
   FILE * fh1, * fh2;
   char outfile[80];

   if (argc != 2) {
     printf("Syntax: chop_by_time infile\n");
     exit(0);
   }

   printf("enter tmin tmax : ");
   scanf("%lf %lf", &tmin, &tmax);

   fh1 = fopen(argv[1], "r");

   if (fh1 == NULL) {
     printf("failed to open input file %s ... exiting\n", argv[1]);
     exit(0);
   }

   sprintf(outfile, "chopped.trc");

   fh2 = fopen(outfile, "w");

   fread(&event, EVENT_SIZE, 1, fh1);

   while (1 == fread(&event, EVENT_SIZE, 1, fh1)) {
      if (event.tbeg > tmin && event.tend < tmax) fwrite(&event, EVENT_SIZE, 1, fh2);
   }


   fclose(fh1);
   fclose(fh2);

   printf("wrote file %s\n", outfile);
  
   return 0;
}
