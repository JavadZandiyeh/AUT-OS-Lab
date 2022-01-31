#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

/* m : number of resources */
#define m 6
/* n : number of threads */
#define n 5
int thread_number[n] = { 0, 1, 2, 3, 4 };
/* the available amount of each resource */ 
int available[m];
/* the maximum demand of each thread */
int maximum[n][m] = { {2, 4, 6, 9, 10, 4}, 
                      {4, 8, 4, 5, 2, 0},
                      {10, 6, 7, 4, 10, 2}, 
                      {2, 10, 0, 3, 2, 3}, 
                      {3, 6, 5, 0, 2, 3}
                    };
/* the amount of currently allocated to each thread */
int allocation[n][m] = { {1, 1, 2, 2, 3, 1}, 
                         {2, 2, 1, 5, 0, 0},
                         {3, 2, 2, 1, 3, 0}, 
                         {0, 0, 0, 0, 0, 2}, 
                         {1, 2, 2, 0, 1, 1}
                       };
/* the remaining need of each thread */
int need[n][m];
/* shows that each thread finished or not 
    -2 : thread has exceeded its maximum calim
    -1 : (request <= need) but not (request <= available)
     0 : finish
*/
int finish[n];
/* initialize thread mutex */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int is_safe_request(int tid, int request[]){
    for(int i = 0; i < m; i++){
        if(request[i] > need[tid][i]){
            finish[tid] = -2; // thread has exceeded its maximum calim
            return -2;
        }
    }
    // request <= need
    for(int i = 0; i < m; i++){
        if(request[i] > available[i]){
            finish[tid] = -1;
            return -1; // not request <= available
        }
    }
    // request <= available
    finish[tid] = 0;
    return 0; // success
}


void request_resources(int tid, int request[]){
    pthread_mutex_lock(&mutex);
    int status = is_safe_request(tid, request);
    if(status == 0){ // success
        for(int i = 0; i < m; i++){
            available[i] += allocation[tid][i];
            allocation[tid][i] = 0;
            need[tid][i] = maximum[tid][i] - allocation[tid][i];
        }
        finish[tid] = 0;
        printf("thread id: %d , status: success\n", tid);   
    }
    pthread_mutex_unlock(&mutex);
    return;
}


void* thread_handler(void* args0){
    int* tid = args0;
    while(finish[*tid] != 0){
        int request[m];
        for(int i = 0; i < m; i++)
            request[i] = rand() % 4;
        request_resources(*tid, request);
        if(finish[*tid] == -2){
            printf("thread id: %d , status: has exceeded its maximum calim\n", *tid);
            finish[*tid] = 0;
        }
    }
}

int main(int argc, char** argv){
    srand(time(NULL)); 

    printf("need: \n");
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            need[i][j] = maximum[i][j] - allocation[i][j];
            printf("%d ", need[i][j]);
        }
        printf("\n");
    }
    // no thread finished in start
    for(int i = 0; i < n; i++)
        finish[i] = -1;

    // get available resources from Banker
    if(argc < m+1){
       printf("not enough arguments\n");
       return EXIT_FAILURE;
    }
    for(int i = 0; i < m; i++)
        available[i] = strtol(argv[i+1], NULL, 10);

    // make available update
    printf("available: \n");
    int sum[m];
    for(int j = 0; j < m; j++){
        sum[j] = 0;
        for(int k = 0; k < n; k++)
            sum[j] += allocation[k][j];
    }
    for(int i = 0; i < m; i++){
        available[i] -= sum[i];
        printf("%d ", available[i]);
    }
    printf("\n");

    // create threads
    pthread_t thread[n];
	for(int i = 0; i < n; i++)
		pthread_create(&thread[i], NULL, thread_handler, &thread_number[i]);

    // wait for threads to be finished
    for(int i = 0; i < n; i++)
		pthread_join(thread[i], NULL);

    return 0;
}
