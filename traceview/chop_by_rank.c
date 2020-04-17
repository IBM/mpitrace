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
   int  minrank, maxrank;
   FILE * fh1, * fh2;
   char outfile[80];

   if (argc != 2) {
     printf("Syntax: chop_by_rank infile\n");
     exit(0);
   }

   printf("enter minrank maxrank : ");
   scanf("%d %d", &minrank, &maxrank);

   fh1 = fopen(argv[1], "r");

   if (fh1 == NULL) {
     printf("failed to open input file %s ... exiting\n", argv[1]);
     exit(0);
   }

   if (maxrank == minrank) sprintf(outfile, "mpirank_%d.trc", minrank);
   else                    sprintf(outfile, "mpirank_%d_to_%d.trc", minrank, maxrank);

   fh2 = fopen(outfile, "w");

   fread(&event, EVENT_SIZE, 1, fh1);

   while (event.taskid < minrank) fread(&event, EVENT_SIZE, 1, fh1);

   while (event.taskid <= maxrank) {
     fwrite(&event, EVENT_SIZE, 1, fh2);
     fread(&event, EVENT_SIZE, 1, fh1);
   }

   fclose(fh1);
   fclose(fh2);

   printf("wrote file %s\n", outfile);
  
   return 0;
}
