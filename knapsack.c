#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

#define DEBUG

#define MAX_NUM_ITEMS 32 //Max of 32 Items because 2^32 = UINT_MAX

struct Item {
    int value;
    int weight;
};

int weight_limit = -1;
struct Item items[MAX_NUM_ITEMS]; 
int num_items = -1;  //Number of Items
unsigned int num_combos = 0; //2^(num_items)

/* Calculates the value of one set of items
 * Returns the value of the items if the weight does not exceed weight_limit.
 * Returns -1 if the given combination is not valid for the Item set.
 *
 * NOTE: weight_limit must be given a value before this is called!
 */
int calcvalue(unsigned int combination, int* value) {
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
            }
            combination >>= 1;
        }
        //printf("Items done. value: %d\n", *value);
        return 0;
    }
}

/* Calculates the solution to the knapsack problem using a brute force approach
 * distributed over a given number of threads.
 * Returns 0 on successful completion.
 * Returns -1 if not given exactly 3 command-line arguments.
 * Returns 1 if the given file cannot be opened.
 */
int main(int argc, char** argv) {
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
    
    printf("Number of items: %d\n", num_items);
    printf("Number of combinations: %u\n", num_combos);
    
    int v = 0;
    for(unsigned int i = 0; i < num_combos; i++) {
        calcvalue(i, &v);
        printf("Combo: %u Value: %d\n", i, v);
    }

    return 0;
}
