#include <time.h>

#include "csc_matrix.h"
#include "vector.h"

#define MAX_ITER 1000
#define EPS 1e-12

int main(void) {
  const char *filepath = "../data/nos5.rb";
  // Read a matrix from a file in the Rutherford-Boeing format
  CSCMatrix *A = read_rb_matrix(filepath);
  if (A == NULL) {
    fprintf(stderr, "Failed to load matrix from file: %s\n", filepath);
    return 1;
  }
  print_csc_matrix(A);

  // ones vector (x_true)
  Vector *x_true = create_vector(A->num_cols);
  if (x_true == NULL) {
    fprintf(stderr, "Failed to create vector x_true.\n");
    return 1;
  }
  for (int i = 0; i < x_true->size; i++) {
    x_true->values[i] = 1.0;
  }
  // 右辺項の設定: b = A * x_true
  Vector *b = create_vector(A->num_cols);
  if (x_true == NULL) {
    fprintf(stderr, "Failed to create vector b.\n");
    return 1;
  }
  mat_vec_mul(A, x_true, b);
  // print_vector(b);

  printf("start\n");

  clock_t start = clock();

  // conjugate_gradient(A, b, x_true, MAX_ITER, (double)EPS);
  conjugate_gradient(A, b, x_true, (int)(2 * (A->num_rows)), EPS);

  clock_t end = clock();

  double execution_time = (double)(end - start) / CLOCKS_PER_SEC;
  printf("Execution time: %.6f seconds\n", execution_time);

  printf("finished\n");
  free_csc_matrix(A);
  free_vector(b);
  free_vector(x_true);

  return 0;
}
