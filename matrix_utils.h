#ifndef MATRIX_UTILS_H
#define MATRIX_UTILS_H

#include <stdlib.h>


#define ERR_MATRIX (matrix_t){ 0 }

typedef struct matrix_s {
    double** data;
    double* raw_data; // not recommended for use
    size_t height;
    size_t width;
} matrix_t;

void free_matrix(matrix_t mat);
matrix_t dot(matrix_t A, matrix_t B);
matrix_t transpose(matrix_t A);
matrix_t copy_matrix(matrix_t mat);

#endif