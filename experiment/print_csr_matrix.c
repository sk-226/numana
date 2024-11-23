#include <stdio.h>
// #include <stdlib.h>

// #include "csr_matrix.h"

void print_csr_matrix(int num_rows, double values[], int col_ind[], int row_ptr[]);
// CSRMatrix* create_csr_matrix(int num_rows, int num_cols, int )

int main(void) {

  /** 
   * 
   * [
   * [2,  0, -7,  0],
   * [0, -5,  1,  0],
   * [0,  0,  0,  9],
   * [0,  0,  8,  3]
   * ]
   *
  */

  int num_rows = 4;      // 行数
  int num_cols = 4;      // 列数
  int num_nonzeros = 7;  // 非ゼロ要素の数
  
  double values[] = {2, -7, -5, 1, 9, 8, 3};
  int col_ind[] = {1, 3, 2, 3, 4, 3, 4};
  int row_ptr[] = {1, 3, 5, 6, 8}; 

  print_csr_matrix(num_rows, values, col_ind, row_ptr);

  return 0;
}

// 対称性を持つと仮定する
void print_csr_matrix(int num_rows, double values[], int col_ind[], int row_ptr[]) {
  for (int i = 0; i < num_rows; i++) {
    int start = row_ptr[i] - 1;
    int end = row_ptr[i+1] - 1;

    for (int j = start; j < end; ++j) {
      int row = i + 1;
      int column = col_ind[j];
      double val = values[j];

      printf("(%d, %d) \t %lf \n", row, column, val);
    }
  }
}
