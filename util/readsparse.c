/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
  struct connStruct {
                      float bytes;
                      int   count;
                      int   rank;
                      int   pad;
                    };
  struct connStruct * connection;
  FILE * fd;
  int nranks, i, k, nconn, minconn, maxconn;
  int minconn_rank, maxconn_rank, minbytes_rank, maxbytes_rank;
  float totbytes, minbytes, maxbytes, avgbytes, avgconn;


  fd = fopen(argv[1], "r");

  if (fd == NULL) {
    printf("failed to open the input file %s ... exiting\n", argv[1]);
    exit(0);
  }

  fread(&nranks, sizeof(int), 1, fd);

  printf("got nranks = %d\n\n", nranks);

  connection = (struct connStruct *) malloc(nranks*sizeof(struct connStruct));

  maxbytes = 0.0;
  minbytes = 1.0e20;

  maxconn = 0;
  minconn = 10000000;

  maxbytes_rank = 0;
  minbytes_rank = 0;

  maxconn_rank = 0;
  minconn_rank = 0;

  avgconn = 0.0;
  avgbytes = 0.0;

  for (i=0; i<nranks; i++) {
     fread(&nconn, sizeof(int), 1, fd);
     fread(connection, sizeof(struct connStruct), nconn, fd);
     avgconn += (float) nconn;
     totbytes = 0.0;
     printf("rank %d has %d connections\n", i, nconn);
     for (k=0; k<nconn; k++) {
       totbytes += connection[k].bytes;
       printf("rank %d sent to rank %d : messages = %d, bytes = %.0f\n", i, connection[k].rank, connection[k].count, connection[k].bytes);
     }
     avgbytes += totbytes;
     printf("rank %d sent a total of %.4e bytes\n", i, totbytes);
     if (totbytes > maxbytes) {
       maxbytes = totbytes;
       maxbytes_rank = i;
     }
     if (totbytes < minbytes) {
       minbytes = totbytes;
       minbytes_rank = i;
     }
     if (nconn > maxconn) {
       maxconn = nconn;
       maxconn_rank = i;
     }
     if (nconn < minconn) {
       minconn = nconn;
       minconn_rank = i;
     }
     printf("\n");
  }

  avgconn  = avgconn  / ((float) nranks);
  avgbytes = avgbytes / ((float) nranks);

  printf("average connections = %.3f\n", avgconn);

  printf("min connections = %d for rank %d\n", minconn, minconn_rank);
  printf("max connections = %d for rank %d\n", maxconn, maxconn_rank);

  printf("average bytes sent = %.4e\n", avgbytes);

  printf("min bytes sent = %.4e for rank %d\n", minbytes, minbytes_rank);
  printf("max bytes sent = %.4e for rank %d\n", maxbytes, maxbytes_rank);

  fclose(fd);

  return 0;
}
