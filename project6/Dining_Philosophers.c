#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define n 5 // number of philosophers
int phil_number[n] = { 0, 1, 2, 3, 4 };

// state of philosophers
enum state_phil{eating, hungry, thinking};
int phil_state[n];

// mutex is used such that no two philosophers 
// may access the pickup or putdown at the same time
sem_t mutex;
sem_t phil_sem[n];

// eat food by philosopher phil_num
void eat(int phil_num){
    
    int left_phil_num = (phil_num + 4) % n;
    int right_phil_num = (phil_num + 1) % n;

	if (
        phil_state[phil_num] == hungry
		&& 
        phil_state[left_phil_num] != eating
		&& 
        phil_state[right_phil_num] != eating
    ){
		// start eating by philosopher phil_num
		phil_state[phil_num] = eating;

		printf("philosopher %d is eating using chopstick[%d] and chopstick[%d]\n", 
        phil_num + 1, left_phil_num + 1, phil_num + 1);

		// in put_chopsticks used for alarm hungry philosophers
        sem_post(&phil_sem[phil_num]);
	}
}

// take chopsticks by philosopher phil_num
void take_chopsticks(int phil_num){
	
    // begining of critical section
    sem_wait(&mutex);

	// when he want to take chopsticks means that he is hungary
	phil_state[phil_num] = hungry;
	// he can eat if his neighbours are not eating
	eat(phil_num);

    // end of critical section
	sem_post(&mutex);

	// if unable to eat wait to be signalled
	sem_wait(&phil_sem[phil_num]);
}

// put chopsticks by philosopher phil_num
void put_chopsticks(int phil_num){

    // begining of critical section
    sem_wait(&mutex);

	// after putting chopsticks he is not
    // hungary or eating but is thinking
	phil_state[phil_num] = thinking;

    printf("philosopher %d finished eating\n", phil_num + 1);
	// printf("Philosopher %d is thinking\n", phil_num + 1);

    int left_phil_num = (phil_num + 4) % n;
    int right_phil_num = (phil_num + 1) % n;

	eat(left_phil_num);
	eat(right_phil_num);

    // end of critical section
	sem_post(&mutex);
}

void* phil_thread_handler(void* phil_num){
	// while (1) {
		int* i = phil_num;
		take_chopsticks(*i);
		put_chopsticks(*i);
	// }
}

int main(){
	int i;
	pthread_t phil_thread[n];


	// initialize the semaphores
	sem_init(&mutex, 0, 1);

	for(i = 0; i < n; i++)
		sem_init(&phil_sem[i], 0, 0);

	// create philosopher processes
	for (i = 0; i < n; i++) {
		pthread_create(&phil_thread[i], NULL, phil_thread_handler, &phil_number[i]);
		printf("philosopher %d is thinking !!\n", i + 1);
	}

	for (i = 0; i < n; i++)
		pthread_join(phil_thread[i], NULL);
}
