/* DO NOT remove includes */
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "surfers.h"

/* Declare synchronization variables */
pthread_mutex_t count_mutex;
sem_t arrived_1, arrived_2, ready_to_leave_1, ready_to_leave_2, dusk;
int surfer_in_water = 0, wait_on_beach = 0, ready_to_leave_count = 0;

/* Add code to surfer's thread. Surfer MUST call getReady, surf, and leave (in that order) */
void surfer(void *dptr) {
  dataT *d=(dataT *)dptr;
  int value, value2;

  sem_wait(&monitor_mutex); //block, so surfer_in_water and wait_on_beach don't change
      
  getReady(d);
  
  if (surfer_in_water >= 2){ //Two surfer in the water
     //Then it can just enter the water
     surfer_in_water ++;
     printf("s%d is surfing\n", d->id);
     d->state = SURFING;
     sem_post(&monitor_mutex);
  }
  else if ( wait_on_beach >= 1){ //If no one in the water and one surfer waiting on beach
       //Signal the waiting surfer, unlock after both sufer are in the water
       sem_post(&arrived_2);
       surfer_in_water ++;
       printf("s%d is surfing\n", d->id);
       d->state = SURFING;
       
       sem_wait(&arrived_1);
       sem_post(&monitor_mutex);
  }
  else { //If no one in the water and no surfer waiting on beach
       wait_on_beach ++; //increament waiting surfer count
       

       //First unlock so it doesn't block other surfer, then wait on another surfer or wait til dusk
       sem_post(&monitor_mutex);
       
       do{
         sem_getvalue(&dusk, &value); 
         sem_getvalue(&arrived_2, &value2);
       }while(!value && !value2);
       if (value) { //If dusk, then leave without surf
          leave(d);
          exit(0);
       }
       
       sem_wait(&arrived_2);
       
       wait_on_beach --;
       surfer_in_water ++;
       printf("s%d is surfing\n", d->id);
       d->state = SURFING;
       sem_post(&arrived_1);
  }
  
  surf(d);

  sem_wait(&monitor_mutex); //block, so surfer_in_water and ready_to_leave don't change
  if (surfer_in_water > 2){ //more than 2 surfer in water
     //then it can just leave
     surfer_in_water --;
     leave(d);
     sem_post(&monitor_mutex);
  }
  else if ( ready_to_leave_count >= 1){ //2 surfer in water and another surfer is ready to leave
       
       sem_post(&ready_to_leave_2); //signal the other surfer that it is ready to leave and unlcok after both surfers leaved
       surfer_in_water --;
       leave(d);
       sem_wait(&ready_to_leave_1);
       sem_post(&monitor_mutex);
  }
  else { //2 surfer in water and other surfer is not ready to leave
       ready_to_leave_count ++; //increament ready to leave count
       sem_post(&monitor_mutex);//unlock so it doesn't block
       
       //wait for the other surfer done surfing
       sem_wait(&ready_to_leave_2);
       ready_to_leave_count --;
       
       surfer_in_water --;
       
       leave(d);
       sem_post(&ready_to_leave_1);
  }
}

/* Add code to main (DO NOT remove initialization code) */
int main() {
  int j=0;

  /* Initialize synchronization variables */
  if (sem_init(&dusk, 0, 0) == -1) { perror("sem_init"); } // THIS HAS CHANGED
  if (sem_init(&monitor_mutex, 0, 1) == -1) { perror("monitor_mutex"); }
  if (pthread_mutex_init(&count_mutex, NULL) == -1) { perror("count_mutex"); }
  if (sem_init(&arrived_1, 0, 0) == -1) { perror("arrived_1"); }
  if (sem_init(&arrived_2, 0, 0) == -1) { perror("arrived_2"); }
  if (sem_init(&ready_to_leave_1, 0, 0) == -1) { perror("ready_to_leave_1"); }
  if (sem_init(&ready_to_leave_2, 0, 0) == -1) { perror("ready_to_leave_2"); }
  
  /* Initialize thread data structures */
  pthread_t t[NSURFERS];
  dataT **ds = malloc(sizeof(dataT) * NSURFERS);
  for (j=0; j<NSURFERS; j++) { ds[j] = malloc(sizeof(struct data));}
  for (j=0; j<NSURFERS; j++) { ds[j]->id = j; ds[j]->state = -1;}
  
  /* s1 and s2 start surfing */ //THIS HAS CHANGED
  /*surf(ds[0]);  //THIS HAS CHANGED
    surf(ds[1]);*/ //THIS HAS CHANGED

  printf("The sharks are in the water\n");
  
  /* Create monitor */
  pthread_t mon;
  pthread_create(&mon, NULL, (void *)&monitor, (void *)ds);

  /* Create surfers */
  for (j=0; j<NSURFERS; j++) { 
      pthread_create(&t[j], NULL, (void *)&surfer, (void *)ds[j]);
  }

  /* Wait for surfers to finish */
  for (j=0; j<NSURFERS; j++) { 
      pthread_join(t[j], NULL);
  }

  /* Wait for monitor to finish */
  pthread_join(mon, NULL);

  /* Clean up synchronization variables */
  sem_destroy(&dusk);
  sem_destroy(&monitor_mutex);
  sem_destroy(&arrived_1);
  sem_destroy(&arrived_2);
  sem_destroy(&ready_to_leave_1);
  sem_destroy(&ready_to_leave_2);
  pthread_mutex_destroy(&count_mutex);

  return 0;
}

