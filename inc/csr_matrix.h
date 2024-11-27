// inc/csr_matrix.h
#ifndef _CSR_MATRIX_H_
#define _CSR_MATRIX_H_

#include <stddef.h>  // NULLを定義する
#include <stdio.h>
#include <stdlib.h>

// TODO: 行列の特徴etc.の情報も入れられるように
// e.g. Kind: Structural Problem, Symmetric: Yes, etc...
typedef struct {
  int num_rows;      // 行数
  int num_cols;      // 列数
  int num_nonzeros;  // 非ゼロ要素の数
  double valfmt;     // Format for numerical values of coefficient

  int *row_ptr;    // 各行の非ゼロ要素の開始位置 (values[row_ptr[i-1]] ~
  int *col_ind;    // 非ゼロ要素の列インデックス
  double *values;  // 非ゼロ要素の値
} CSRMatrix;

/**
 * @brief Read a matrix from a file in the Rutherford-Boeing format
 *
 * @param filepath
 * @return CSRMatrix*
 */
CSRMatrix *read_rb_matrix(const char *filepath);

/**
 * @brief Create a CSRMatrix object
 *
 * @param num_rows
 * @param num_cols
 * @param num_nonzeros
 * @param valfmt
 * @return CSRMatrix*
 */
CSRMatrix *create_csr_matrix(int num_rows, int num_cols, int num_nonzeros,
                             double valfmt);

/**
 * @brief Free the CSRMatrix object
 *
 * @param matrix
 */
void free_csr_matrix(CSRMatrix *matrix);

/**
 * @brief Print the CSRMatrix object
 *
 * @param matrix
 */
void print_csr_matrix(const CSRMatrix *matrix);

/**
 * @brief Parse the format specifier to get the field width
 *
 * @param fmt_str
 * @return int
 */
int parse_format(const char *fmt_str);
#endif  // _CSR_MATRIX_H_
