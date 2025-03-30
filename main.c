#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "is_prime.h"

#define MAX_NUMBER 1000000
#define MAX_THREAD 4

typedef struct{
    int first;
    int last;
}Data;

void *thread_find_primes(void *arg){
    Data *data = (Data *)arg;
    for(int i = data->first; i <= data->last; i++){
        if(is_prime(i)){
            printf("%d\n", i);
        }
    }
    return NULL;
}

int main(int argc, char *argv[]){
    pthread_t threads[MAX_THREAD];
    Data threads_datas[MAX_THREAD];
    int range = MAX_NUMBER / MAX_THREAD;

    // assign number range to each thread
    for(int i = 0; i < MAX_THREAD; i++){
        threads_datas[i].first = i * range + 1; // first number
        // last number
        if(i == MAX_THREAD - 1){
            threads_datas[i].last = MAX_NUMBER;
        }
        else{
            threads_datas[i].last = (i + 1) * range;    
        }
        pthread_create(&threads[i], NULL, thread_find_primes, &threads_datas[i]);
    }

    // waiting thread execution
    for(int i = 0; i < MAX_THREAD; i++){
        pthread_join(threads[i], NULL);
    }

    return 0;
}