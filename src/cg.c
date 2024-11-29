// src/cg.c
#include "csc_matrix.h"
#include "vector.h"

/* CSCMatrix用のCG法(Conjugate Gradient method) */

/**
 * @brief CG法による連立一次方程式の解法
 *
 * @param A 係数行列
 * @param b 右辺項
 * @param x_true 真の解
 * @param max_iter 最大反復回数
 * @param eps 収束判定基準
 * TODO: 誤差解析用にapend, errを追加
 */

void conjugate_gradient(const CSCMatrix *A, const Vector *b,
                        const Vector *x_true, const int max_iter,
                        const double eps) {
  Vector *x = create_vector(b->size);
  Vector *r = create_vector(b->size);
  Vector *p = create_vector(b->size);
  Vector *w = create_vector(b->size);  // A * p
  // Vector *err = create_vector(b->size);

  Vector *tmp = create_vector(b->size);

  // 近似解の初期ベクトル x_0 = 0
  for (int i = 0; i < x->size; i++) {
    x->values[i] = 0.0;
  }

  // print_vector(b);

  // r = b (x_0 = 0)
  vec_copy(b, r);

  // p = r
  vec_copy(r, p);

  double rho_old = vec_dot(r, r);

  // double norm2_x_true = vec_norm(x_true);
  double norm2_b = vec_norm(b);
  // printf("norm2_b = %e\n", norm2_b);

  for (int i = 0; i < max_iter; i++) {
    // w = A * p
    mat_vec_mul(A, p, w);
    double sigma = vec_dot(p, w);
    if (sigma == 0.0) {
      fprintf(stderr, "Error: Division by zero detected in iteration %d\n",
              i + 1);
      break;
    }

    // alpha係数
    double alpha = rho_old / sigma;
    // 解の更新: x = x + alpha * p
    vec_scale(p, alpha, tmp);
    vec_add(x, tmp, x);
    // 残差の更新: r = r - alpha * Ap
    vec_scale(w, alpha, tmp);
    vec_sub(r, tmp, r);

    // vec_sub(x_true, x, err);
    double rel_res = vec_norm(r) / norm2_b;

    // 収束判定
    if (rel_res < eps) {
      printf("-------------------------------------\n");
      printf("Converged in %d iterations\n", i + 1);
      printf("Relres_norm = %e\n", rel_res);
      printf("-------------------------------------\n");
      break;
    } else {
      // printf("Iteration %d\n", i + 1);
      // printf("Relres_norm = %e\n", rel_res);
    }
    // printf("%e\n", vec_norm(r));

    double rho_new = vec_dot(r, r);
    // ベータ係数
    double beta = rho_new / rho_old;
    rho_old = rho_new;
    // 探索方向の更新: p = r + beta * p
    vec_scale(p, beta, tmp);  // beta * p
    vec_add(r, tmp, p);
  }

  if ((vec_norm(r) / norm2_b) >= eps) {
    printf("Did not converge in %d iterations\n", max_iter);
  }

  free_vector(x);
  free_vector(r);
  free_vector(p);
  free_vector(w);
  free_vector(tmp);
}
