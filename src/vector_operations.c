/* src/vector_operations.c */
#include "vector.h"

/* ベクトル同士の積 */
double vec_dot(const Vector *vec1, const Vector *vec2) {
  if (vec1->size != vec2->size) {
    fprintf(stderr,
            "Error: Vector sizes do not match in file %s at line # %d \n",
            __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }

  double result = 0.0;

  for (int i = 0; i < vec1->size; i++) {
    result += vec1->values[i] * vec2->values[i];
  }

  return result;
}

/* ベクトルのスカラー倍 */
void vec_scale(Vector *vec, double scalar) {
  for (int i = 0; i < vec->size; i++) {
    vec->values[i] *= scalar;
  }
}

/* ベクトルの加算 */
void vec_add(Vector *vec1, const Vector *vec2, Vector *result) {
  if (vec1->size != vec2->size || vec1->size != result->size) {
    fprintf(stderr,
            "Error: Vector sizes do not match in file %s at line # %d \n",
            __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < vec1->size; i++) {
    result->values[i] = vec1->values[i] + vec2->values[i];
  }
}

/* ベクトルの減算 */
void vec_sub(Vector *vec1, const Vector *vec2, Vector *result) {
  if (vec1->size != vec2->size || vec1->size != result->size) {
    fprintf(stderr,
            "Error: Vector sizes do not match in file %s at line # %d \n",
            __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < vec1->size; i++) {
    result->values[i] = vec1->values[i] - vec2->values[i];
  }
}

/* ベクトルの2-ノルムを計算 */
double vec_norm(const Vector *vec) {
  double result = 0.0;

  for (int i = 0; i < vec->size; i++) {
    result += vec->values[i] * vec->values[i];
  }

  return sqrt(result);
}

/* ベクトルをコピー */
void vec_copy(const Vector *src, Vector *dest) {
  if (src->size != dest->size) {
    fprintf(stderr,
            "Error: Vector sizes do not match in file %s at line # %d \n",
            __FILE__, __LINE__);
    exit(EXIT_FAILURE);
  }

  for (int i = 0; i < src->size; i++) {
    dest->values[i] = src->values[i];
  }
}
