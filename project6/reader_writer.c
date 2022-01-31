#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
# include <sys/shm.h>
# include <sys/ipc.h>

int max_count = 3;

/* without synchronization, processes may be
   in race condition */
void do_read_write(int proc_task, int* count){
    if(proc_task == 0){ // write into shared memory(count)
        *count = *count + 1;
        pid_t pid = getpid();
        printf("Task: Write, PID: %d, count: %d\n", pid, *count);
    }
    else{ //read from shared memory(count)
        pid_t pid = getpid();
        printf("Task: Read , PID: %d, count: %d\n", pid, *count);
    }
}

void without_sync(int* count){
    // three processes
    if(fork()){ //reader1
        while(*count < max_count)
            do_read_write(1, count);
        exit(0);
    }else{
        if(fork()){ //reader2
            while(*count < max_count)
                do_read_write(1, count);
            exit(0);
        }else{ //writer
            while(*count < max_count)
                do_read_write(0, count);
            wait(NULL);
        }
    }
}

// --------------------------------------------------

/* processes which access to a shared memory
   must be sync to prevent race condition */
void do_write(int* count, int* turn){
    *turn = 0;

    while(*turn == 1);
    // critical section        
    *count = *count + 1;
    pid_t pid = getpid();
    printf("Task: Write, PID: %d, count: %d\n", pid, *count);

    *turn = 1;
}

void do_read(int* count, int* turn){
    *turn = 1;

    while(*turn == 0);
    // critical section        
    pid_t pid = getpid();
    printf("Task: Read , PID: %d, count: %d\n", pid, *count);

    *turn = 0;
}

void with_sync(int* count, int* turn){
    // three processes
    if(fork()){ //reader1
        while(*count < max_count)
            do_read(count, turn);
        exit(0);
    }else{
        if(fork()){ //writer
            while(*count < max_count)
                do_write(count, turn);
            exit(0);
        }else{ //reader2
            while(*count < max_count)
                do_read(count, turn);
            wait(NULL);
        }
    }
}

int main(int argc, char const *argv[]){
    // count = shm[0], 
    int shm_id = shmget(IPC_PRIVATE, 20, IPC_CREAT| SHM_R | SHM_W);
    int* shm = (int *) shmat(shm_id, 0, 0);
    shm[0] = 0; // count
    shm[1] = 0; // turn 0 for writer and 1 for readers

    printf("\nincreasing count without synchronization:\n");
    without_sync(&shm[0]);

    printf("\nincreasing count with synchronization:\n");
    with_sync(&shm[0], &shm[1]);
    
    shmctl(shm_id, IPC_RMID, NULL);
    return 0;
}
