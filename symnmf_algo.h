#ifndef SYMNF_ALGO_H
#define SYMNF_ALGO_H

#include <stdlib.h>
#include "matrix_utils.h"



matrix_t sym(matrix_t datapoints);

matrix_t ddg(matrix_t datapoints);

matrix_t norm(matrix_t datapoints);

void perform_iteration(matrix_t H, matrix_t W, double beta);
double calc_index(matrix_t H, matrix_t WH, matrix_t HHTH, double beta, size_t i, size_t j);
matrix_t symnmf(matrix_t H, matrix_t W, double eps, double beta, size_t max_iter);


#endif