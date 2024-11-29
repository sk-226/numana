#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <ctype.h>   // for parse_format function
#include <stddef.h>  // NULLを定義する
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// typedef struct {
//   int num_rows;      // 行数
//   int num_cols;      // 列数
//   int num_nonzeros;  // 非ゼロ要素の数
//   double valfmt;     // Format for numerical values of coefficient

//   int *ptrcrd;    // 各行の非ゼロ要素の開始位置
//   int *indcrd;    // 非ゼロ要素のインデックス
//   double *values;  // 非ゼロ要素の値
// } Matrix;

/**
 * @brief Parse the format specifier to get the field width
 *
 * @param fmt_str
 * @return int
 */
int parse_format(const char* fmt_str);
#endif  // _MATRIX_H_
