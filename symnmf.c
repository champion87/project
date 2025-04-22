#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "matrix_utils.h"
#include "symnmf_algo.h"

typedef enum goal_e {
    GOAL_INVALID = 0,
    GOAL_SYM = 1,
    GOAL_NORM = 2,
    GOAL_DDG = 3
} goal_t;

matrix_t pasre_input(const char * filename);
int count_lines(FILE * file);
int count_columns(FILE * file);


int count_lines(FILE * file) {

    int count = 0;
    char ch = 0;
    
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            count++;
        }
    }
    rewind(file);
    return count;
}

int count_columns(FILE * file) {
    int count = 0;
    char ch = 0;
    
    while ((ch = fgetc(file)) != '\n') {
        if (ch == ',') {
            count++;
        }
    }
    rewind(file);
    return count + 1;
}

matrix_t pasre_input(const char * filename) {
    FILE * file = fopen(filename, "r");
    if (!file) {
        printf("file %s does not exist\n", filename);
        exit(1);
    }
    const int hight = count_lines(file);
    const int width = count_columns(file);
    matrix_t mat = alloc_matrix(hight, width);
    if (IS_ERR_MAT(mat)) {
        printf("Error allocating matrix\n");
        exit(1);
    }

    for (size_t i = 0; i < hight; i++) {
        for (size_t j = 0; j < width; j++) {
            fscanf(file, "%lf,", &mat.data[i][j]);
        }
    }
    fclose(file);
    return mat;
}




int main(int argc, char* argv[]) {

    if (argc != 3) {
        printf("Usage: ./symnmf goal filename\n");
        exit(1);
    }

    goal_t goal = GOAL_INVALID;
    if (strcmp(argv[1], "sym") == 0) {
        goal = GOAL_SYM;
    } else if (strcmp(argv[1], "norm") == 0) {
        goal = GOAL_NORM;
    } else if (strcmp(argv[1], "ddg") == 0) {
        goal = GOAL_DDG;
    } else {
        printf("Invalid goal\n");
        exit(1);
    }

    matrix_t mat = pasre_input(argv[2]);
    matrix_t ans = ERR_MATRIX;
    switch (goal) {
        case GOAL_SYM:
            ans = sym(mat);
            break;
        case GOAL_NORM:
            ans = norm(mat);
            break;
        case GOAL_DDG:
            ans = ddg(mat);
            break;
        default:
            break;
    }

    print_matrix(ans);

    free_matrix(ans);
    free_matrix(mat);

    return EXIT_SUCCESS;
}