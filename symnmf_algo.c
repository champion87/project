#include "symnmf_algo.h"
#include "matrix_utils.h"
#include <stdio.h>
#include <stdlib.h>

#define INVALID_DIFF (-1)
#define IS_VALID_DIFF(diff) (diff >= 0)

// Calculates the new value of that should be at a given index for the matrix H in the optimisation step.
// The formula is Hij * (1 - beta + beta * (WHij / HHTHij))
double calc_index(matrix_t H, matrix_t WH, matrix_t HHTH, double beta, size_t i, size_t j)
{
    double Hij = H.data[i][j];
    double ratio = WH.data[i][j] / HHTH.data[i][j];
    double extrapolation = 1 - beta + (beta * ratio);
    double res = Hij * extrapolation;
    return res;
}

// Performs the optimization iteration inplace, i.e. changing the value of H.data[][]
void perform_iteration(matrix_t H, matrix_t W, double beta)
{
    matrix_t WH = dot(W, H);
    matrix_t HT = transpose(H);
    matrix_t HHT = dot(H, HT);
    matrix_t HHTH = dot(HHT, H);
    free_matrix(HT);
    free_matrix(HHT);

    for (size_t i = 0; i < H.height; i++)
    {
        for (size_t j = 0; j < H.width; j++)
        {
            double new_value = calc_index(H, WH, HHTH, beta, i, j);
            H.data[i][j] = new_value;
        }
    }

    free_matrix(WH);
    free_matrix(HHTH);
}

// TODO make sure to free everything
matrix_t symnmf(matrix_t H, matrix_t W, double eps, double beta, size_t max_iter)
{
    size_t iter_num = 0;
    matrix_t old_H = copy_matrix(H);
    double diff = INVALID_DIFF;
    do
    {
        perform_iteration(H, W, beta);
        diff = frobenius_distance_squared(H, old_H);
        free_matrix(old_H);
        old_H = copy_matrix(H);
        iter_num++;
    } while (diff > eps && iter_num < max_iter); //TODO check if the max iter is checked correctly

    free_matrix(old_H);
    return H;
}
