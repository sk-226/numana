// inc/csc_matrix.h
#ifndef _CSC_MATRIX_H_
#define _CSC_MATRIX_H_

#include "matrix.h"

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

  int *col_ptr;    // 各列の非ゼロ要素の開始位置 
  int *row_ind;    // 非ゼロ要素の行インデックス
  double *values;  // 非ゼロ要素の値
} CSCMatrix;

/**
 * @brief Read a matrix from a file in the Rutherford-Boeing format
 *
 * @param filepath
 * @return CSCMatrix*
 */
CSCMatrix *read_rb_matrix(const char *filepath);

/**
 * @brief Create a CSCMatrix object
 *
 * @param num_rows
 * @param num_cols
 * @param num_nonzeros
 * @param valfmt
 * @return CSCMatrix*
 */
CSCMatrix *create_csc_matrix(int num_rows, int num_cols, int num_nonzeros,
                             double valfmt);

/**
 * @brief Free the CSCMatrix object
 *
 * @param matrix
 */
void free_csc_matrix(CSCMatrix *matrix);

/**
 * @brief Print the CSCMatrix object
 *
 * @param matrix
 */
void print_csc_matrix(const CSCMatrix *matrix);

/**
 * @brief Parse the format specifier to get the field width
 *
 * @param fmt_str
 * @return int
 */
int parse_format(const char *fmt_str);
#endif  // _CSC_MATRIX_H_
