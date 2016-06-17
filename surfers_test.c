/* Implement your test code here. DO NOT remove function signatures below. You may add other functions as needed. */
#include "surfers.h" /* has dataT, NSURFERS */
#include <stdio.h>

void surf(dataT *d) {
     int j = 0;
     int value = 0;
     /* put thread to sleep while it is surfing 
        wake it after rand time or dusk */
     /*while (j < rand(10) && !value){
           //sleep(1);
           j++;
           sem_getvalue(&dusk, &value);
     }*/
     printf("s%d is ready to leave\n", d->id);
}

void leave(dataT *d) {
     d->state = LEAVE;
     printf("s%d leaves\n", d->id);
}

void getReady(dataT *d) {
     d->state = READY;
     printf("s%d arrives\n", d->id);
}

void monitor(void * x) {
     int value = 0;
     dataT **d=(dataT **)x;
     int j, Rcount, Scount, Lcount, Xcount;
     
     do {
        Xcount = 0;
        Rcount = 0;
        Scount = 0;
        Lcount = 0;
        sem_wait(&monitor_mutex);
        for (j=0; j<NSURFERS; j++) { 
            if(d[j]->state == READY) Rcount++;
            if(d[j]->state == SURFING) Scount++;
            if(d[j]->state == LEAVE) Lcount++;
            if(d[j]->state == -1) Xcount++;
        }
        
        if (Rcount == 1 && (Lcount == (NSURFERS - 1))){ //If one surfer just arrive and all surfers have leaved
           value = 1;
           printf("|----------------- Dusk -------------------|\n");
           sem_post(&dusk);
           //printf("Ready = %d Surfing = %d Leaved = %d UNprocessed = %d Dusk = %d\n", Rcount, Scount, Lcount, Xcount,value);
        }
        else if (Lcount == NSURFERS){
             value = 1;
        }
        //else printf("%d %d %d %d %d %d\n", Rcount, Scount, Lcount, Xcount,value, delta_usec);
        
        sem_post(&monitor_mutex);
        
     }while(!value);
     
}

