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

/* ベクトルの作成と開放 */
// TODO: ゼロおよび非ゼロ要素もまとめて一時的にarrayに変換
// TODO: CSR形式用パーサーの関数
/**
 * @brief この関数を一番最初に読み出し、rb形式(Rutherford
 * Boeing形式)の行列をCSRMatrixとして変数化する。 e.g CSRMatrix *A =
 * read_csr_matrix(char *mat_file_name);
 * @param char *filename
 * @return CSRMatrix*
 * @sa CSRMatirx* create_csr_matrix(int num_rows, int num_cols, int
 * num_nonzeros)
 * @detail num_rows, num_cols, _num_nonzerosは既知とする。
 * -> create_csr_matrixを呼び出す。
 */
CSRMatrix *read_rb_matrix(const char *filepath);
CSRMatrix *create_csr_matrix(int num_rows, int num_cols, int num_nonzeros,
                             double valfmt);
void free_csr_matrix(CSRMatrix *matrix);

// ベクトルの表示
void print_csr_matrix(CSRMatrix *matrix);

int parse_format(const char *fmt_str);
#endif  // _CSR_MATRIX_H_
