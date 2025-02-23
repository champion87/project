#include <stdlib.h>

#include "symnmf_algo.h"


void free_matrix(matrix_t mat) {
    free(mat.raw_data);
    free(mat.data);
}