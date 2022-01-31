#include <stdio.h>
#include <stdlib.h>

typedef struct process{
    int pid;
    int priority;
    int at; // arrival time
    int bt; // burst time
    int rmt; // remaining time
    int wt; // waiting time
    int tat; // turnaround time
}process;

int n; // number of processes
int i, j; // used for iterations
int total_burst; // total cpu burst of processes

/* compare two processes  by arrival time */
int compare0(const void* process1, const void* process2){
    process* p1 = (process*) process1;
    process* p2 = (process*) process2;
    return p2->at < p1->at;
}

/* compare two processes  by priority */
int compare1(const void* process1, const void* process2){
    process* p1 = (process*) process1;
    process* p2 = (process*) process2;
    return p1->priority > p2->priority;
}

/* compare two processes  by remaining time */
int compare2(const void* process1, const void* process2){
    process* p1 = (process*) process1;
    process* p2 = (process*) process2;
    return p1->rmt > p2->rmt;
}

/* print all processes info */
void print_processes_info(process PCB[]){
    printf("\n%s%9s%10s%7s%6s%12s", "pid", "arrival", "priority", "burst", "wait", "turnarround");
    for(i = 0; i < n; i++)
        printf("\n%d%8d%10d%8d%7d%8d\n", 
        PCB[i].pid, PCB[i].at, PCB[i].priority, PCB[i].bt, PCB[i].wt, PCB[i].tat);
    printf("\n");
}

/* first come first serve policy */
void FCFS(process PCB[]){
    printf("\narrival time of processes: ");
    for(i = 0; i < n; i++){
        int arrival;
        scanf("%d", &arrival);
        PCB[i].at = arrival;
    }

    int time = 0;

    qsort(PCB, n, sizeof(struct process), compare0);
    
    for(i = 0; i < n; i++){
        PCB[i].wt = time;
        PCB[i].tat = PCB[i].wt + PCB[i].bt;
        PCB[i].rmt = 0;
        time += PCB[i].bt;
    }
    printf("\nFCFS:\n");
    print_processes_info(PCB);
}

/* shortest job first policy */
void SJF(process PCB[]){
    int time = 0;

    qsort(PCB, n, sizeof(struct process), compare2);

    for(i = 0; i < n; i++){
        PCB[i].at = 0;
        PCB[i].wt = time;
        PCB[i].tat = PCB[i].wt + PCB[i].bt;
        PCB[i].rmt = 0;
        time += PCB[i].bt;
    }
    printf("\nSJF:\n");
    print_processes_info(PCB);   
}

/* priority queue policy */
void PriorityQueue(process PCB[]){
    printf("\npriority of processes: ");
    for(i = 0; i < n; i++){
        int priority;
        scanf("%d", &priority);
        PCB[i].priority = priority;
    }
    
    int time = 0;

    qsort(PCB, n, sizeof(struct process), compare1);
    
    for(i = 0; i < n; i++){
        PCB[i].wt = time;
        PCB[i].tat = PCB[i].wt + PCB[i].bt;
        PCB[i].rmt = 0;
        time += PCB[i].bt;
    }
    printf("\nPriorityQueue:\n");
    print_processes_info(PCB);
}

/* round robin policy */
void RoundRobin(process PCB[]){
    int q = 1;
    printf("\ntime quantum: ");
    scanf("%d", &q);

    int time = 0;
    i = 0;

    while(total_burst != 0){
        if(PCB[i].rmt >= q){
            j = (i+1)%n;
            while(j != i){
                if(PCB[j].rmt != 0)
                    PCB[j].wt += q;
                j = (j+1)%n;
            }
            PCB[i].rmt -= q;
            total_burst -= q;
            i = (i+1)%n;            
        }else if(PCB[i].rmt > 0){
            j = (i+1)%n;
            while(j != i){
                if(PCB[j].rmt != 0)
                    PCB[j].wt += PCB[i].rmt;
                j = (j+1)%n;
            }
            total_burst -= PCB[i].rmt;
            PCB[i].rmt = 0;
            i = (i+1)%n; 
        }else if(PCB[i].rmt == 0){
            i = (i+1)%n;
        }
    }

    for(i = 0; i < n; i++)
        PCB[i].tat = PCB[i].wt + PCB[i].bt;
    
    printf("\nRoundRobin:\n");
    print_processes_info(PCB);
}


int main(int argc, char const *argv[]){
    printf("number of processes: ");
    scanf("%d", &n);

    process PCB[n];
    total_burst = 0;

    printf("\nburst time of processes: ");
    for(i = 0; i < n; i++){
        int burst;
        scanf("%d", &burst);
        total_burst += burst;
        PCB[i].pid = i;
        PCB[i].wt = 0;
        PCB[i].tat = 0;
        PCB[i].at = 0;
        PCB[i].priority = 0;
        PCB[i].bt = burst;
        PCB[i].rmt = burst;
    }
    
    FCFS(PCB);
    // SJF(PCB);
    // PriorityQueue(PCB);
    // RoundRobin(PCB);

    return 0;
}
