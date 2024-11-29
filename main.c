#include "csc_matrix.h"

int main(void) {
  const char *filepath = "./data/LFAT5.rb";
  CSCMatrix *matrix = read_rb_matrix(filepath);
  if (matrix == NULL) {
    fprintf(stderr, "Failed to load matrix from file: %s\n", filepath);
    return 1;
  }

  // 必要に応じて行列の内容を表示
  print_csc_matrix(matrix);

  // メモリの解放
  free_csc_matrix(matrix);

  return 0;
}
