#include <stdlib.h>

#include "symnmf_algo.h"


void free_matrix(matrix_t mat) {
    if (mat.raw_data != NULL)
        free(mat.raw_data);
    if (mat.data != NULL)
        free(mat.data);
}