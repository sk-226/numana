#include "vector.h"

int main(void) {
  /* Vectorに関する関数のテスト */
  int size = 5;

  Vector *a = create_vector(size);
  for (int i = 0; i < a->size; i++) {
    a->values[i] = 1.0;
  }
  Vector *b = create_vector(size);
  for (int i = 0; i < b->size; i++) {
    b->values[i] = 2.0;
  }

  print_vector(a);
  print_vector(b);

  double dot_product = vec_dot(a, b);
  printf("a * b = %e\n", dot_product);
  print_vector(b);

  vec_scale(a, 10.0, a);
  printf("a * 10.0 = \n");
  print_vector(a);
  printf("\n");

  vec_add(a, b, a);
  printf("a += b: \n");
  print_vector(a);
  printf("\n");

  vec_sub(a, b, a);
  printf("a -= b: \n");
  print_vector(a);
  printf("\n");

  double norm_a = vec_norm(a);
  printf("||a||_2 = %e\n", norm_a);
  printf("\n");

  Vector *c = create_vector(size);
  vec_copy(a, c);
  print_vector(c);
  printf("\n");

  free_vector(a);
  free_vector(b);
  free_vector(c);

  return 0;
}
