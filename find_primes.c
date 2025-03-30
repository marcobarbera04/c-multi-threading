#include <stdio.h>
#include <time.h>
#include "is_prime.h"

#define MAX_NUMBER 1000000

int main(){
    // initial time
    clock_t start_time = clock();

    int prime_numbers[MAX_NUMBER / 2];
    int prime_numbers_counter = 0;

    for(int i = 1; i <= MAX_NUMBER; i++){
        if(is_prime(i)){
            prime_numbers[prime_numbers_counter] = i;
            prime_numbers_counter++;
        }
    }
    prime_numbers[prime_numbers_counter] = -1;

    int k = 0;
    while (prime_numbers[k] != -1){
        printf("%d is prime\n", prime_numbers[k]);
        k++;
    }
    

    // finish time
    clock_t finish_time = clock();

    double execution_time = (double)(finish_time - start_time) / CLOCKS_PER_SEC;
    printf("Execution time: %.6f seconds\n", execution_time);
}