/* src/csr_matrix.c */

#include "csr_matrix.h"

// CSR形式の行列を作成
CSRMatrix* create_csr_matrix(int num_rows, int num_cols, int num_nonzeros) {
  CSRMatrix *mat = (CSRMatrix*)malloc(sizeof(CSRMatrix));
  mat->num_rows = num_rows;
  mat->num_cols = num_cols;
  mat->num_nonzeros = num_nonzeros;
  mat->values = (double*)malloc(num_nonzeros * sizeof(double));
  mat->col_ind = (int*)malloc(num_nonzeros * sizeof(int));
  mat->row_ptr = (int*)malloc((num_rows + 1) * sizeof(int));  // TODO: 実際のアルゴリズムに合わせる(非ゼロ要素の位置によって変わる)
  return mat;
}


