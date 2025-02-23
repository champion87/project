#ifndef SYMNF_ALGO_H
#define SYMNF_ALGO_H

#include <stdlib.h>

#define ERR_MATRIX (matrix_t){ 0 }

typedef struct matrix_s {
    double** data;
    double* raw_data; // not recommended for use
    size_t height;
    size_t width;
} matrix_t;

void free_matrix(matrix_t mat);

matrix_t sym(matrix_t datapoints);

matrix_t ddg(matrix_t datapoints);


matrix_t norm(matrix_t datapoints);

#endif