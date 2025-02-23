#include "matrix_utils.h"

void free_matrix(matrix_t mat) {
    if (mat.raw_data != NULL)
        free(mat.raw_data);
    if (mat.data != NULL)
        free(mat.data);
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

matrix_t transpose(matrix_t A) // TODO: check that this code is right
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

matrix_t copy_matrix(matrix_t mat)
{
    // matrix_t result = {
    //     .height = mat.height,
    //     .width = mat.width,
    //     .data = malloc(mat.height * sizeof(double*)),
    //     .raw_data = NULL
    // };

    // for (size_t i = 0; i < mat.height; i++)
    // {
    //     result.data[i] = malloc(mat.width * sizeof(double));
    //     for (size_t j = 0; j < mat.width; j++)
    //     {
    //         result.data[i][j] = mat.data[i][j];
    //     }
    // }

    // return result;
}