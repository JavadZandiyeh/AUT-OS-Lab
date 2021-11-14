#include <stdio.h>
#include <time.h>
#include <stdlib.h>

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

int main(int argc, char const *argv[]){
    int hist[25];
    for(int i = 0; i < 25; i++)
        hist[i] = 0;
    int iter = 5000;

    clock_t begin = clock();

    srand(time(NULL));
    for(int j = 0; j < iter; j++){
        int counter = 0;
        for(int i = 0; i < 12; i++){
            int r = rand() % 100;
            if(r >= 49)
                counter++;
            else
                counter--;
        }
        hist[counter + 12]++;
    }

    clock_t end = clock();

    writeResualt(begin, end, hist, iter);
    return 0;
}
