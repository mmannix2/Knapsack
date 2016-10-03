#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

#define MAX_NUM_ITEMS 31 //Max of 31 Items because (2^32)-1 = UINT_MAX

//  Represents an Item from the input file. Each Item has a weight and a value.
struct Item {
    int value;
    int weight;
};

/*  Represents a selection of Items. Because creating 2^(num_items) structs
    would be costly, I only create one struct Combination for each thread that
    holds and returns the best value Combination that thread has encountered.*/
struct Combination {
    unsigned int number; /* An int holding a binary representation of which 
                            Items are included in this Combination.
                            For example:
                            A combination with number 8 includes Items 1 and 3.
                            8 = 0b101 */
    int value;  //Sum of values for this Combination
    int weight; //Sum of weight for this Combination
};

int weight_limit = -1;
struct Item items[MAX_NUM_ITEMS]; 
int num_items = -1;  //Number of Items
unsigned int num_combos = 0; //2^(num_items)
int num_threads = 1;
int combos_per_thread = -1;

/* Calculates the value of one set of items
 * Returns the value of the items if the weight does not exceed weight_limit.
 * Returns -1 if the given combination is not valid for the Item set.
 * Returns -2 if the given combination exceeds weight_limit.
 *
 * NOTE: weight_limit must be given a value before this is called!
 */
int calc_combo(unsigned int combination, int* value, int* weight) {
    if(combination >= num_combos) {
        return -1;
    }
    else {
        *value = 0;
        *weight = 0;
        
        for(int i = 0; i < num_items; i++) {
            if(combination % 2 != 0) {
                *value += items[i].value;
                *weight += items[i].weight;
            }
            combination >>= 1;
        }
        if(*weight > weight_limit) {
            return -2;
        }
        //This combination is within the weight limit 
        return 0;
    }
}

void* try_combos(void* idp) {
    int id = * (int*) idp;
    struct Combination* best = malloc(sizeof(struct Combination));
    best->value = -1;
    best->weight = -1;
    int temp_value = -1;
    int temp_weight = -1;

    for(unsigned int i = combos_per_thread*id; 
        i < (combos_per_thread*id)+combos_per_thread;
        i++) {
        if( calc_combo(i, &temp_value, &temp_weight) == 0 && 
            temp_value > best->value ) {
            best->number = i;
            best->value = temp_value;
            best->weight = temp_weight;
        }
    }
    
    pthread_exit((void*) best);
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
            fscanf(file,"%d %d", &items[i].weight, &items[i].value);
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
    
    #ifdef DEBUG
    printf("Number of items: %d\n", num_items);
    printf("Number of combinations: %u\n", num_combos);
    printf("\nUsing %d threads with %u combinations per thread.\n",
        num_threads, combos_per_thread); 
    #endif
    
    pthread_t threads[num_threads];
    int ids[num_threads];

    //Spawn all the threads
    for(int i=0; i<num_threads; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, try_combos, &ids[i]);
    }
    
    //Initialize struct Combination
    struct Combination best;
    best.value = -1; //Dummy value
    
    //Join all threads
    for(int i=0; i<num_threads; i++) {
        struct Combination* thread_best = (struct Combination*)malloc(
            sizeof(struct Combination) );
        pthread_join(threads[i], (void**) &thread_best);
        if(thread_best->value > best.value) {
            best.number = thread_best->number;
            best.value = thread_best->value;
            best.weight = thread_best->weight;
        }
        
        free(thread_best);
    }
    
    //Final output
    printf("Best Value:\n\tCombination: 0x%X\n\tValue: %d\n\tWeight: %d\n",
            best.number,
            best.value,
            best.weight );
    
    pthread_exit(NULL);
}
