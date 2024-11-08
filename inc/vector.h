// inc/vector.h
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stddef.h>  // NULLを定義する
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int size;      // ベクトルの要素数
  double *data;  // ベクトルの要素を格納する配列
} Vector;

// ベクトルの作成と開放
Vector *create_vector(int size);
void free_vector(Vector *vec);

// ベクトルの表示
void print_vector(Vector *vec);

#endif  // _VECTOR_H_
