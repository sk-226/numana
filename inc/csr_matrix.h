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

  double *values;  // 非ゼロ要素の値
  int *col_ind;    // 非ゼロ要素の列インデックス
  int *row_ptr;    // 各業の開始位置
} CSRMatrix;

// ベクトルの作成と開放
CSRMatrix *create_csr_matrix(int num_rows, int num_cols, int num_nonzeros);
void free_csr_matrix(CSRMatrix *mat);

/* rb(Rutherford Boeing形式の読み込み) */
// TODO: ゼロおよび非ゼロ要素もまとめて一時的にarrayに変換
// TODO: CSR形式用パーサーの関数
CSRMatrix* read_rb_matrix(const char *filename);

// ベクトルの表示
void print_csr_matrix(CSRMatrix *mat);

#endif  // _CSR_MATRIX_H_
