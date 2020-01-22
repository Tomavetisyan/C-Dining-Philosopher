#include <stdio.h> 
#include <stdlib.h>
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h>
//declare your global variables
const char *a[2] = {"Socrates", "Plato"};
const char *topics[5] = {"boats", "the economy", "astronomy", "what to eat", "McDonald's Szechuan Sauce"};

sem_t mutex; 

void* eat(void *arg) { 
    /* allow the first philosopher to access their critical section 
    (aka: “Socrates has grabbed the chopstick and started eating”). 
    Have this philosopher eat for a random amount of time, but max 10 seconds*/
    char* name = (char *)arg;
    if (sem_trywait(&mutex) == 0) {
        printf("%s grabbed the chopsticks and started eating\n", name);
        sleep(rand() % 10);
    } 
    /*For the philosopher that gets trapped by the semaphore have a print statement 
    such as “Socrates cannot grab the chopstick because 
    it is in use”*/
    else {
        printf("%s cannot grab the chopticks because it is in use\n", name);
        sem_wait(&mutex);
    }
    //At the end of the critical section don’t forget to release the trapped philosopher!!
    printf("%s has finished eating, and put the chopsticks down\n", name);
    sem_post(&mutex); 
}
void think(int phil){
    //have the chosen philosopher think about something. 
    printf("%s is thinking about %s\n", a[phil], topics[(rand() % 5)]);
}
int main() { 
    int currentPhilosopher;
    //initialize your semaphore
    sem_init(&mutex, 0, 1); 
    //initialize your thread variables
    pthread_t Eat1, Eat2;
    time_t t;
    srand((unsigned) time(&t));
    while(1){
        // randomly decide which philosopher’s turn it is
        currentPhilosopher=(rand() % 2);
        //randomly decide what action that philosopher will take
        if((rand() % 2) == 1){
            think(currentPhilosopher);
        }else{
            //this will be the “eat” block 
            if(a[currentPhilosopher] == "Plato"){
                //create a thread passing Plato’s name to eat function
                pthread_create(&Eat1, NULL, eat, (void*)a[1])  != 0;
                //create a thread passing Socrates name to eat function
                pthread_create(&Eat2, NULL, eat, (void*)a[0])  != 0;
            }else{
                //create a thread passing Socrates name to eat function
                pthread_create(&Eat2, NULL, eat, (void*)a[0])  != 0;
                //create a thread passing Plato’s name to eat function
                pthread_create(&Eat1, NULL, eat, (void*)a[1])  != 0;
            }
        }
        pthread_join(Eat1,NULL); 
        pthread_join(Eat2,NULL); 
        sem_destroy(&mutex);
    }
    //should never reach here.
    return 0;
}
