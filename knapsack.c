#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DEBUG

#define MAX_NUM_ITEMS 1000

struct Item {
    int value;
    int weight;
};

int weight_limit = -1;
struct Item items[MAX_NUM_ITEMS]; 
int num_items = 0;

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
    }
   
    #ifdef DEBUG
    printf("Weight limit: %d\n", weight_limit);
    printf("Items:\n");
    for(int i=0; i<num_items; i++) {
        printf("\tItem #%d, v=%d w=%d\n", i, items[i].value, items[i].weight);
    }
    #endif

    return 0;
}
