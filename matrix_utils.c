#include <string.h>
#include <math.h>
#include "matrix_utils.h"

matrix_t alloc_matrix(size_t height, size_t width) {
    matrix_t result = {
        .height = height,
        .width = width,
        .data = NULL,
        .raw_data = NULL
    };
    result.raw_data = (double*)calloc(result.height * result.width, sizeof(*result.raw_data));
    if (!result.raw_data) {
        return ERR_MATRIX;
    }
    result.data = (double**)calloc(result.height, sizeof(*result.data));
    if (!result.data) {
        free(result.raw_data);
        return ERR_MATRIX;
    }
    for (size_t i = 0; i < result.height; i++) {
        result.data[i] = &result.raw_data[i * result.width];
    }
    return result;
}

void free_matrix(matrix_t mat) {
    if (mat.raw_data != NULL) {
        free(mat.raw_data);
    }
    if (mat.data != NULL) {
        free(mat.data);
    }
    mat = ERR_MATRIX;
}

double frobenius_distance_squared(matrix_t A, matrix_t B) // TODO: check that this code is right
{
    if (A.height != B.height || A.width != B.width)
    {
        return FROB_ERROR;
    }

    double sum = 0;
    for (size_t i = 0; i < A.height; i++)
    {
        for (size_t j = 0; j < A.width; j++)
        {
            sum += pow(A.data[i][j] - B.data[i][j], 2);
        }
    }

    return sum;
}


matrix_t dot(matrix_t A, matrix_t B) // TODO: check that this code is right
{
    if (A.width != B.height)
    {
        return ERR_MATRIX;
    }

    matrix_t result = alloc_matrix(A.height, A.width);
    if (IS_ERR_MAT(result)) {
        return ERR_MATRIX;
    }

    for (size_t i = 0; i < A.height; i++)
    {
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

matrix_t transpose(matrix_t A) // TODO: check that this code is right
{
    matrix_t result = alloc_matrix(A.height, A.width);
    if (IS_ERR_MAT(result)) {
        return ERR_MATRIX;
    }

    for (size_t i = 0; i < A.width; i++)
    {
        for (size_t j = 0; j < A.height; j++)
        {
            result.data[i][j] = A.data[j][i];
        }
    }

    return result;
}

matrix_t copy_matrix(matrix_t mat)
{
    matrix_t result = alloc_matrix(mat.height, mat.width);
    if (IS_ERR_MAT(result)) {
        return ERR_MATRIX;
    }
    memcpy(result.raw_data, mat.raw_data, result.height * result.width * sizeof(*result.raw_data));
    return result;
}