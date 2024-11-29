#include "csc_matrix.h"
#include "vector.h"

int main(void) {
  const char *filepath = "../data/LFAT5.rb";
  CSCMatrix *matrix = read_rb_matrix(filepath);
  if (matrix == NULL) {
    fprintf(stderr, "Failed to load matrix from file: %s\n", filepath);
    return 1;
  }
  print_csc_matrix(matrix);

  Vector *x = create_vector(matrix->num_cols);
  for (int i = 0; i < x->size; i++) {
    x->values[i] = 1.0;
  }
  Vector *result = create_vector(matrix->num_rows);

  print_vector(x);

  mat_vec_mul(matrix, x, result);

  print_vector(result);

  // メモリの解放
  free_csc_matrix(matrix);
  free_vector(x);
  free_vector(result);

  return 0;
}
