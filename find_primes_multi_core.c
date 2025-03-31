#define _GNU_SOURCE // has to be defined to use CPU_ZERO and CPU_SET
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <sched.h>
#include <time.h>
#include "is_prime.h"

#define MAX_NUMBER 1000000
#define MAX_THREAD 4

typedef struct{
    int first;
    int last;
    int prime_numbers[MAX_NUMBER / MAX_THREAD];
    int core;
}Data;

void *thread_find_primes(void *arg){
    Data *data = (Data *)arg;

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(data->core, &cpuset);
    printf("Thread assigned to core %d\n", data->core);



    if(pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset) != 0) {
        perror("Affinity");
        return NULL;
    }

    int prime_counter = 0;
    for(int i = data->first; i <= data->last; i++){
        if(is_prime(i)){
            data->prime_numbers[prime_counter] = i;
            prime_counter++;
        }
    }
    data->prime_numbers[prime_counter] = -1; // terminator signal
    return NULL;
}

int main(int argc, char *argv[]){
    // initial time
    clock_t start_time = clock();

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
        threads_datas[i].core = i;
        pthread_create(&threads[i], NULL, thread_find_primes, &threads_datas[i]);
    }

    // waiting thread execution
    for(int i = 0; i < MAX_THREAD; i++){
        pthread_join(threads[i], NULL);
    }

    int prime_numbers[MAX_NUMBER / 2];
    int prime_numbers_counter = 0;
    for(int i = 0; i < MAX_THREAD; i++){
        int j = 0;
        while(threads_datas[i].prime_numbers[j] != -1){
            prime_numbers[prime_numbers_counter] = threads_datas[i].prime_numbers[j];
            prime_numbers_counter++;
            j++;
        }
    }
    prime_numbers[prime_numbers_counter] = -1; // terminator signal

    int k = 0;
    while(prime_numbers[k] != -1){
        printf("%d is prime\n", prime_numbers[k]);
        k++;
    }

    // finish time
    clock_t finish_time = clock();

    double execution_time = (double)(finish_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.6f seconds\n", execution_time);

    return 0;
}