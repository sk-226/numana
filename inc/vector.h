// inc/vector.h
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h> // NULLを定義する

typedef struct vector {
  int size; // ベクトルの要素数
  double *data; // ベクトルの要素を格納する配列
} Vector;

// ベクトルの作成と開放
extern Vector* create_vector(int size);
extern void free_vector(Vector *vec);

// ベクトルの表示
extern void print_vector(Vector *vec);

#endif // _VECTOR_H_
