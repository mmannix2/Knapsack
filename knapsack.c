#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

#define DEBUG

#define MAX_NUM_ITEMS 31 //Max of 31 Items because (2^32)-1 = UINT_MAX

struct Item {
    int value;
    int weight;
};

int weight_limit = -1;
struct Item items[MAX_NUM_ITEMS]; 
int num_items = -1;  //Number of Items
unsigned int num_combos = 0; //2^(num_items)
int num_threads = 1;
int combos_per_thread = -1;

pthread_mutex_t mutex;

/* Calculates the value of one set of items
 * Returns the value of the items if the weight does not exceed weight_limit.
 * Returns -1 if the given combination is not valid for the Item set.
 * Returns -2 if the given combination exceeds weight_limit.
 *
 * NOTE: weight_limit must be given a value before this is called!
 */
int calcvalue(unsigned int combination, int* value) {
    int weight = 0;
    
    printf("Combo: %u\n", combination);

    if(combination >= num_combos) {
        return -1;
    }
    else {
        *value = 0;
        for(int i = 0; i < num_items; i++) {
            //printf("\tItem #%d, value: %d\n", i, items[i].value);
            //printf("\tCombo: %u Bit: %d\n", combination, combination % 2);
            if(combination % 2 != 0) {
                *value += items[i].value;
                weight += items[i].weight;
            }
            combination >>= 1;
        }
        printf("\tItems done. value: %d <= %d: %d\n", *value, weight_limit,
            weight <= weight_limit);
        if(weight > weight_limit) {
            //printf("\tItems exceed weight_limit. Value = -1\n");
            *value = -1;
            return -2;
        }
        
        return 0;
    }
}

void* try_combos(void* idp) {
    int id = * (int*) idp;
    int* highest_value = malloc(sizeof(int));
    //*highest_value = 0;

    //printf("Thread #%d!\n", id);
    //printf("\tStart: %10u, \tEnd: %10u\n", combos_per_thread*id,
    //    combos_per_thread*id + combos_per_thread -1);
    int temp = -1;
    for(unsigned int i = combos_per_thread*id; 
        i < (combos_per_thread*id)+combos_per_thread;
        i++) {
        pthread_mutex_lock(&mutex);
        calcvalue(i, &temp);
        //printf("\tCombo: %u Value: %d\n", i, temp);
        pthread_mutex_unlock(&mutex);
        if(temp > *highest_value) {
            printf("\tCombo %u is highest! Value: %d\n", i, temp);
            *highest_value = temp;
        }
    }
    
    pthread_exit((void*) highest_value);
}

/* Calculates the solution to the knapsack problem using a brute force approach
 * distributed over a given number of threads.
 * Returns 0 on successful completion.
 * Returns -1 if not given exactly 3 command-line arguments.
 * Returns 1 if the given file cannot be opened.
 */
int main(int argc, char** argv) {
    /*** INITIALIZATION/SETUP ***/
    //Make sure there are exactly 3 arguments
    if(argc != 3) {
        printf("ERROR: Invalid number of arguments.\n");
        printf("Usage: %s input_file.txt X (Where X is number of threads)\n",
            argv[0]);
        exit(-1);
    }
    
    #ifdef DEBUG
    printf("cmd args:%s %s %s\n", argv[0], argv[1], argv[2]);
    #endif
    
    //Open the file
    FILE* file = fopen( argv[1], "r");
    if(file == NULL) {
        printf("ERROR: Unable to open %s.\n", argv[1]);
        exit(1);
    }
    else {
        int i;
        //Read in weight_limit
        fscanf(file, "%d", &weight_limit);
        //Read in item values and weights
        for(i=0; !feof(file); i++) {
            fscanf(file,"%d %d", &items[i].value, &items[i].weight);
        }
        num_items = i-1;
        num_combos = 0b1;
        for(int i = 0; i < num_items; i++) {
            num_combos *= 2; 
        }
    }
    
    //Get num_threads from cmd line arg
    num_threads = atoi(argv[2]);
    combos_per_thread = num_combos/num_threads;
    
    printf("Number of items: %d\n", num_items);
    printf("Number of combinations: %u\n", num_combos);
    printf("\nUsing %d threads with %u combinations per thread.\n",
        num_threads, combos_per_thread); 
    
    pthread_t threads[num_threads];
    int ids[num_threads];
    pthread_mutex_init(&mutex, NULL);

    //Spawn all the threads
    for(int i=0; i<num_threads; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, try_combos, &ids[i]);
    }
    
    //Join all threads
    int highest_value = 0;
    for(int i=0; i<num_threads; i++) {
        int* thread_highest = (int*)malloc(sizeof(int));
        pthread_join(threads[i], (void**) &thread_highest);
        printf("thread_highest: %d\n", *thread_highest);
        if(*thread_highest > highest_value) {
            highest_value = *thread_highest;
        }
    }

    printf("Highest Value: %d\n", highest_value);
    
    pthread_exit(NULL);
}
