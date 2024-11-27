// inc/vector.h
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <math.h>
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
void print_vector(const Vector *vec);

/**
 * @brief Calculate the dot product of two vectors
 *
 * @param vec1
 * @param vec2
 * @return double
 *
 * @note The two vectors must have the same size
 */
double vec_dot(const Vector *vec1, const Vector *vec2);

/**
 * @brief Scale the vector by a scalar
 *
 * @param vec
 * @param scalar
 */
void vec_scale(Vector *vec, double scalar);

/**
 * @brief Add two vectors
 *
 * @param vec1
 * @param vec2
 * @param result
 *
 * @note The two vectors must have the same size
 * @note a = a + c という形にしたい場合第一引数にresultと同じポインタを指定する
 */
void vec_add(Vector *vec1, const Vector *vec2, Vector *result);

/**
 * @brief Subtract two vectors
 *
 * @param vec1
 * @param vec2
 * @param result
 *
 * @note The two vectors must have the same size
 * @note a = a - c という形にしたい場合第一引数にresultと同じポインタを指定する
 */
void vec_sub(Vector *vec1, const Vector *vec2, Vector *result);

/**
 * @brief Calculate the 2-norm of the vector
 *
 * @param vec
 * @return double
 */
double vec_norm(const Vector *vec);

/**
 * @brief Copy the contents of one vector to another
 *
 * @param src
 * @param dest
 * @return double
 */
double vec_copy(const Vector *src, Vector *dest);

#endif  // _VECTOR_H_
