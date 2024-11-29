#include "csc_matrix.h"
#include "vector.h"

void mat_vec_mul(const CSCMatrix *A, const Vector *x, Vector *result) {
  if (A->num_cols != x->size) {
    fprintf(stderr,
            "Error: Matrix and Vector sizes do not match in file %s at line # "
            "%d \n",
            __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }

  // result を 0 で初期化
  for (int i = 0; i < result->size; i++) {
    result->values[i] = 0.0;
  }

  for (int col = 0; col < A->num_cols; col++) {
    int start = A->col_ptr[col];
    int end = A->col_ptr[col + 1];

    for (int idx = start; idx < end; idx++) {
      int row = A->row_ind[idx];
      double val = A->values[idx];
      result->values[row] += val * x->values[col];

      if (col != row) {
        result->values[col] += val * x->values[row];
      }
    }
  }
}
