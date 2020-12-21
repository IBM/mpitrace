/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

// utility to parse my standard MPI wrappers
// and add nvtx labeled ranges for all 

#include <stdio.h>
#include <string.h>

int main(int argc, char * argv[])
{
   char * str, line[256];
   FILE * srcfile;
   FILE * idfile;
   FILE * ofile;
   const int npts = 146;
   int i, j;
   char idname[npts][80];
   char addline[256];
   char * ptr = NULL, * tvp = NULL, * logp = NULL;

   // instrument C wrappers
   idfile = fopen("cids.txt", "r");
   if (idfile == NULL) {
     printf("can't open cids.txt ... exiting\n");
     exit(0);
   }

   for (i=0; i<npts; i++) {
      fscanf(idfile, "%s", idname[i]);
   }

   srcfile = fopen("ref_mpitrace.c", "r");
   if (srcfile == NULL) {
     printf("can't open ref_mpitrace.c ... exiting\n");
     exit(0);
   }

   ofile   = fopen("mpitrace.c", "w");
   if (ofile == NULL) {
     printf("can't open mpitrace.c ... exiting\n");
     exit(0);
   }

   // echo 90 lines
   for (i=0; i<90; i++) {
      ptr = fgets(line, sizeof(line), srcfile);
      if (ptr != NULL) fprintf(ofile, "%s", line);
   }

   j = 0;
   while (NULL != fgets(line, sizeof(line), srcfile) ) {
      // check to see if the line contains "struct timeval" or LogEvent
      tvp = strstr(line, "struct timeval");
      logp = strstr(line, "LogEvent");
      if (tvp != NULL) {
         sprintf(addline, "   nvtxRangeId_t range = nvtxRangeStartA(label[%s]);\n", idname[j]);
         str = strcat(line, addline);
         fprintf(ofile, "%s", str);
         j++;
      }
      else if (logp != NULL) {
         sprintf(addline, "   nvtxRangeEnd(range);\n");
         str = strcat(addline, line);
         fprintf(ofile, "%s", addline);
      }
      else fprintf(ofile, "%s", line);
   }

   fclose(srcfile);
   fclose(idfile);
   fclose(ofile);

   // now instrument Fortran wrappers
   idfile = fopen("fids.txt", "r");
   if (idfile == NULL) {
     printf("can't open cids.txt ... exiting\n");
     exit(0);
   }

   for (i=0; i<npts; i++) {
      fscanf(idfile, "%s", idname[i]);
   }

   srcfile = fopen("ref_fortran_wrappers.c", "r");
   if (srcfile == NULL) {
     printf("can't open ref_mpitrace.c ... exiting\n");
     exit(0);
   }

   ofile   = fopen("fortran_wrappers.c", "w");
   if (ofile == NULL) {
     printf("can't open mpitrace.c ... exiting\n");
     exit(0);
   }

   // echo 246 lines for fortran_wrappers.c
   for (i=0; i<246; i++) {
      ptr = fgets(line, sizeof(line), srcfile);
      if (ptr != NULL) fprintf(ofile, "%s", line);
   }

   j = 0;
   while (NULL != fgets(line, sizeof(line), srcfile) ) {
      // check to see if the line contains "struct timeval" or LogEvent
      tvp = strstr(line, "struct timeval");
      logp = strstr(line, "LogEvent");
      if (tvp != NULL) {
         sprintf(addline, "   nvtxRangeId_t range = nvtxRangeStartA(label[%s]);\n", idname[j]);
         str = strcat(line, addline);
         fprintf(ofile, "%s", str);
         j++;
      }
      else if (logp != NULL) {
         sprintf(addline, "   nvtxRangeEnd(range);\n");
         str = strcat(addline, line);
         fprintf(ofile, "%s", addline);
      }
      else fprintf(ofile, "%s", line);
   }

   fclose(srcfile);
   fclose(idfile);
   fclose(ofile);

   printf("done\n");

   return 0;
}
