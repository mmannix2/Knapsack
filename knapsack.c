#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define DEBUG

#define STR_SIZE 64
#define NUM_STR 1000

char strings[NUM_STR][STR_SIZE];
int num_str = 0;

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
        for(i=0; !feof(file); i++) {
            fscanf(file, "%s", strings[i]);
            printf("%s\n", strings[i]);
        }
        num_str = i;
    }
    
    return 0;
}
