#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
# include <sys/shm.h>
# include <sys/ipc.h>


void printHistogram(int* hist){
    int i, j;
    for(i = 0; i < 25; i++){
        for(j = 0; j < hist[i]; j++)
            printf("*");
        printf("\n");
    }
}

void writeResualt(clock_t begin, clock_t end, int* hist, int iter){
    double time_spent = 0.0;
    time_spent += (double)(end - begin) / CLOCKS_PER_SEC;
    printf("The elapsed time for %d iteration is %f seconds.\n", iter ,time_spent);

    // printHistogram(hist);
    printf("Normal Distribution history: ");
    for(int j = 0; j < 25; j++)
        printf("%d ", hist[j]);
    printf("\n");
}

void do_iteration(int iter, int* shm_hist){
    for(int j = 0; j < iter; j++){
        int counter = 0;
        for(int i = 0; i < 12; i++){
            int r = rand() % 100;
            if(r >= 49)
                counter++;
            else
                counter--;
        }
        shm_hist[counter + 12]++;
    }
}

int main(int argc, char const *argv[]){
    // shared memory id
    int shm_id = shmget(IPC_PRIVATE, 20, IPC_CREAT| SHM_R | SHM_W);
    // shared memory attach
    int* shm_hist = (int *)shmat(shm_id, 0, 0);
    // initialize shared memory with 0
    for(int i = 0; i < 25; i++)
        shm_hist[i] = 0;

    // set number of iteration for all
    int iter = 5000;
    // set number of itteration for each children
    // we have 4 processes thus we split iteration into 4 parts
    int child_iter = iter/4;

    // for generating random number we have to call it one in our code
    srand(time(NULL));

    // start of normal distribution computation
    clock_t begin = clock();

    // four process
    if(fork()){ //child1
        do_iteration(child_iter, shm_hist);
        exit(EXIT_SUCCESS);
    }else{
        if(fork()){ //child2
            do_iteration(child_iter, shm_hist);
            exit(EXIT_SUCCESS);
        }else{
            if(fork()){ //child3
                do_iteration(child_iter, shm_hist);
                exit(EXIT_SUCCESS);
            }else{ //parent
                do_iteration(child_iter, shm_hist);
                for(int c = 0; c < 3; c++)
                    wait(NULL);
            }
        }
    }
    
    clock_t end = clock();

    writeResualt(begin, end, shm_hist, iter);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
