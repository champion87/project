#ifndef SYMNF_ALGO_H
#define SYMNF_ALGO_H

#include <stdlib.h>

#define ERR_MATRIX (matrix_t){ 0 }

typedef struct matrix_s {
    double** data;
    double* raw_data;
    size_t height;
    size_t width;
} matrix_t;


#endif