#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

/* Calculates the solution to the knapsack problem using a brute force approach
 * distributed over a given number of threads.
 * Returns 0 on successful completion.
 * Returns -1 if not given exactly 3 command-line arguments.
 * Returns 1 if the given file cannot be opened.
 */
int main(int argc, char** argv) {
    //Make sure there are exactly 3 arguments
    if(argc != 3) {
        printf("Invalid number of arguments.\n");
        printf("Usage: %s input_file.txt X (Where X is number of threads)\n",
            argv[0]);
        exit(-1);
    }
    

    return 0;
}
