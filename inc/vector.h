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

/**
 * @brief Create a vector object
 * 
 * @param size 
 * @return Vector* 
 */
Vector *create_vector(int size);

/**
 * @brief Free the vector object
 * 
 * @param vec 
 */
void free_vector(Vector *vec);

/**
 * @brief Print the vector
 * 
 * @param vec 
 */
void print_vector(Vector *vec);



#endif  // _VECTOR_H_
