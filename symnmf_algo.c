#include "symnmf_algo.h"
#include <stdio.h>
#include <stdlib.h>

matrix_t dot(matrix_t A, matrix_t B)
{
    if (A.width != B.height)
    {
        return ERR_MATRIX;
    }

    matrix_t result = {
        .height = A.height,
        .width = B.width,
        .data = malloc(A.height * sizeof(double*)),
        .raw_data = NULL
    };

    for (size_t i = 0; i < A.height; i++)
    {
        result.data[i] = malloc(B.width * sizeof(double));
        for (size_t j = 0; j < B.width; j++)
        {
            double sum = 0;
            for (size_t k = 0; k < A.width; k++)
            {
                sum += A.data[i][k] * B.data[k][j];
            }
            result.data[i][j] = sum;
        }
    }

    return result;
}

matrix_t transpose(matrix_t A)
{
    matrix_t result = {
        .height = A.width,
        .width = A.height,
        .data = malloc(A.height * sizeof(double*)),
        .raw_data = NULL
    };

    for (size_t i = 0; i < A.width; i++)
    {
        result.data[i] = malloc(A.height * sizeof(double));
        for (size_t j = 0; j < A.height; j++)
        {
            result.data[i][j] = A.data[j][i];
        }
    }

    return result;
}

double calc_index(matrix_t H, matrix_t WH, matrix_t HHTH, double beta, int i, int j)
{
    double Hij = H.data[i][j];
    double ratio = WH.data[i][j] / HHTH.data[i][j];
    double extrapolation = 1 - beta + (beta * ratio);
    double res = Hij * extrapolation;
    return res;
}

void perform_iteration(matrix_t H, matrix_t W, double beta)
{
    matrix_t WH = dot(W, H);
    matrix_t HT = transpose(H);
    matrix_t HHT = dot(H, HT);
    matrix_t HHTH = dot(HHT, H);
    free_matrix(HT);
    free_matrix(HHT);

    for (int i = 0; i < H.height; i++)
    {
        for (int j = 0; j < H.width; j++)
        {
            double new_value = calc_index(H, WH, HHTH, beta, i, j);
            H.data[i][j] = new_value;
        }
    }

    free_matrix(WH);
    free_matrix(HHTH);
}
// TODO make sure to free everything
void symnmf(matrix_t H, matrix_t W, double eps, double beta)
{
    matrix_t old_H = copy_matrix(H);
    do
    {
        perform_iteration(H, W, beta);
    } while ();
    

}