/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <papi.h>

#define NUM_COUNTERS 6

static int eventHandle = PAPI_NULL;

static int use_event_list = 1;
static char envname[NUM_COUNTERS][128];


int main(int argc, char * argv[])
{
   int j, numcounters, group, nranks, rc, ic;
   char * ptr = NULL;
   char * list_ptr;
   char delimiters[] = {" ,"};
   char counter_name[128];

   list_ptr = getenv("HPM_EVENT_LIST");
   if (list_ptr != NULL) {
      use_event_list = 1;
      ptr = strtok(list_ptr, delimiters);
      j = 0;
      while(ptr != NULL) {
         strcpy(envname[j], ptr);
         ptr = strtok(NULL, delimiters);
         j++;
         if (j == NUM_COUNTERS) break;
      }
      numcounters = j;
      group = 1000;
   }
   else {
     printf("you must set env variable HPM_EVENT_LIST=event1,event2,etc. ... exiting\n");
     return 0;
   }

   printf("checking event list : ");
   for (ic=0; ic<numcounters; ic++) printf("%s ", envname[ic]);
   printf("\n");

   // for now counting is done by just the master thread
   rc = PAPI_library_init(PAPI_VER_CURRENT);
   if (rc < 0) {
      fprintf(stderr, "PAPI initialization failed  ... counters not enabled\n");
      return 0;
   }

   rc = PAPI_create_eventset(&eventHandle);
   if (rc != PAPI_OK) {
      fprintf(stderr, "PAPI failed to create an event set\n");
      return 0;
   }

   for (ic = 0; ic<numcounters; ic++) {
      memset(counter_name, '\0', sizeof(counter_name));
#ifdef __PPC64__
      if (use_event_list) sprintf(counter_name, "%s", envname[ic]);
#else
      if (use_event_list) sprintf(counter_name, "%s", envname[ic]);
#endif
      rc = PAPI_add_named_event(eventHandle, counter_name);
      if (rc != PAPI_OK) {
         fprintf(stderr, "PAPI failed to add event %s ... counters not enabled\n",  counter_name);
         return 0;
      }
   }

   rc = PAPI_start(eventHandle);
   if (rc != PAPI_OK) {
      fprintf(stderr, "PAPI failed to start ... counters not enabled\n");
      return 0;
   }

   printf("OK : PAPI reported no conflicts or problems with this list\n");
   return 0;
}
