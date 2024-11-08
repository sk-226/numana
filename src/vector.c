/* src/vector.c */
#include "vector.h"

// ベクトルの作成
Vector *create_vector(int size) {
  Vector *vec = (Vector *)malloc(sizeof(Vector));

  vec->size = size;
  vec->data = (double *)calloc(size, sizeof(double));

  return vec;
}

// ベクトルの解放
void free_vector(Vector *vec) {
  if (vec != NULL) {
    free(vec->data);
    free(vec);
  }
}

// ベクトルの表示
void print_vector(Vector *vector) {
  printf("Vector of size: %d\n", vector->size);
  for (int i = 0; i < vector->size; i++) {
    printf(" [%d] = %lf\n", i, vector->data[i]);
  }
}
