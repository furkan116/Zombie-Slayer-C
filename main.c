//
//  main.c
//  zombieSlayerC
//
//  Created by Furkan Yıldırım on 10.05.2021.
//

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
  
/*number of zombies in the game:
you should synchronize threads editing this variable*/
int zombieCounter = 0;
int killedZombies = 0;

/*time*/
struct timespec tim;

/*Mutex for synchronized numbers*/
static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* Keeps track of number of zombies entered.*/
void zombieEntered(){
    pthread_mutex_lock(&mutex);
    zombieCounter++;
    pthread_mutex_unlock(&mutex);
    
}
/* Keeps track of number of zombies killed.*/
void zombieKilled(){
    pthread_mutex_lock(&mutex);
    killedZombies++;
    zombieCounter--;
    pthread_mutex_unlock(&mutex);
}
/* Returns true if number of zombies in the room are
greater than or equal to 100.*/
int tooManyZombiesInTheRoom(){

    return (zombieCounter >= 100);
}

/*Returns true if more than 100 zombies have been killed.*/
int killed100Zombies(){

    return (killedZombies >= 100);
}
/* Returns true if there is at least one zombies in the room.*/
int zombiesExist(){
    if(zombieCounter >= 1) {
        return 1;
    }
    return 0;
}
/*Returns the number of zombies killed.*/
int getKilledCount(){
    return killedZombies;
}

/* Returns the number of zombies in the room.*/
int getInTheRoomCount(){
    return zombieCounter;
}
/*doorman thread*/
void *doorMan(void *p){
    
    while (!killed100Zombies() && !tooManyZombiesInTheRoom()) {

        if (rand()%2 == 1) {
            zombieEntered();
        }
        nanosleep(&tim, NULL);
    }
    return NULL;
}

/*slayer thread*/
void *slayer(void *p){
    
    while (!killed100Zombies() && !tooManyZombiesInTheRoom()) {
        
        if (zombiesExist()) {
            zombieKilled();
        }
        nanosleep(&tim, NULL);
    }
    return NULL;
}

/*simulator main thread*/
int main(int argc, char **argv){
    /*Timer settings*/
    tim.tv_sec  = 0;
    tim.tv_nsec = 20000000L;

    int number;
    printf("How many doorman do you want: ");
    scanf("%d", &number);

    pthread_t slayerPlayer, doormanPlayer;

    for(int i = 0; i < 3; i++) {
        pthread_create(&slayerPlayer, NULL, &slayer, NULL);
    }
    for(int j = 0; j < number; j++) {
        pthread_create(&doormanPlayer, NULL, &doorMan, NULL);
    }

    pthread_join(slayerPlayer, NULL);
    pthread_join(doormanPlayer, NULL);

    /*Win or Lose*/
    if(getKilledCount() >= 100) {
        printf("Win");
    }
    else if(getInTheRoomCount() >= 100) {
        printf("Lose");
    }
    else {
        printf("ERROR");
    }
    return 0;
}
