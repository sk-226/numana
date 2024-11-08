#include <stdio.h>

#include "vector.h"

int main(void) {
  int size = 5;

  Vector* b = create_vector(size);
  for (int i = 0; i < b->size; i++) {
    b->data[i] = 1.0;
  }

  printf("Vector b: \n");
  print_vector(b);

  // メモリの開放
  free_vector(b);

  return 0;
}
