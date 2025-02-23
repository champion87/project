#include <stdio.h>
#include <stdlib.h>

#include "matrix_utils.h"
#include "symnmf_algo.h"

matrix_t pasre_input(const char * filename);

matrix_t pasre_input(const char * filename) {
    FILE * file = fopen(filename, "r");
    if (!file) {
        printf("file %s does not exist\n");
        exit(1);
    }
    
}



int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Usage: python symnmf.py k goal filename\n");
        exit(1);
    }

    char * goal = argv[1];
    pasre_input(argv[2]);

    return EXIT_SUCCESS;
}