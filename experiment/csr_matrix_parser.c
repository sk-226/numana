#include <ctype.h>  // for parse_format function
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int num_rows;      // 行数
  int num_cols;      // 列数
  int num_nonzeros;  // 非ゼロ要素の数
  double valfmt;     // Format for numerical values of coefficient

  double *values;  // 非ゼロ要素の値
  int *col_ind;    // 非ゼロ要素の列インデックス
  int *row_ptr;    // 各行の非ゼロ要素の開始位置 (values[row_ptr[i-1]] ~
                   // values[row_ptr[i-1] - 1]までの値が1行の中にある)
} CSRMatrix;

int parse_format(const char *fmt_str) {
  /* フォーマット指定子からフィールド幅を取得する関数 */
  // e.g. "(16I5)" -> 5
  int field_width = 0;
  const char *p = fmt_str;

  // '(' をスキップ
  while (*p && *p != '(') p++;
  if (*p == '(') p++;

  // 数字をスキップ
  while (*p && isdigit((unsigned char)*p)) p++;

  // データ型を取得 (一旦返すのはfield_widthのみ)
  char data_type = toupper((unsigned char)*p);
  p++;

  // フィールド値を取得
  if (isdigit((unsigned char)*p)) {
    field_width = atoi(p);
  }

  return field_width;
}

int main(void) {
  const char *filepath = "../data/LFAT5.rb";
  FILE *fp = fopen(filepath, "r");
  if (fp == NULL) {
    perror("Error opening file");
    return 1;
  }

  /* メタデータを格納する変数 */
  // コメントアウトされている変数について, Only present if there are right-hand
  // sides presents
  char title[81] = "";
  char date[81] = "";
  char author[81] = "";
  char ed[81] = "";
  char id[81] = "";

  int totcrd = 0;  // Total number of lines excluding header
  int ptrcrd = 0;  // Number of lines for pointers
  int indcrd = 0;  // Number of lines for row (or variables) indices
  int valcrd = 0;  // Number of lines for numerical values
  int rhscrd = 0;  // Number of lines for right-hand sides??

  char mxtype[4] = "";  // Matrix type
  int nrow = 0;         // Number of rows (or variables)
  int ncol = 0;         // Numbers of columns (or elements)
  int nnzero = 0;       // Number of non-zeros
  int neltvl = 0;  // Number of elemental matrix entries (zero in the case of
                   // assembled matrices)

  char ptrfmt[17] = "";  // Format for pointers
  char indfmt[17] = "";  // Format for row (or variables) indices
  char valfmt[21] = "";  // Format for numerical values of coefficient,
                         // str->doubleでキャストするように
  char rhsfmt[21] = "";  // Format for numerical values of right-hand sides??

  // Line 5 on headers -> only present if there are right-hand sides

  /* 行を読み込むときのための変数 */
  char line[82];
  int line_number = 0;

  /* ファイルから行を読み込む */
  while (fgets(line, sizeof(line), fp) != NULL) {
    line_number++;  // 1-based で考える

    // 行末の改行コードを削除
    line[strcspn(line, "\n")] = '\0';

    /* 行番号に応じてメタデータを抽出 */
    if (line_number == 1) {
      // タイトル業のフォーマットに従って、メタデータを抽出
      // e.g.
      // "Oberwolfach/LFAT5; 2004; J. Lienemann et al.; ed: E. Rudnyi |1440    "
      // セミコロンとパイプで区切る
      char *parts[5];  // タイトル, 日付, 著者, 編集者, ID
      char *part = strtok(line, ";|");
      int i = 0;

      // トークンに分割する
      while (part != NULL && i < 5) {
        parts[i++] = part;
        part = strtok(NULL, ";|");
      }

      /* それぞれのメタデータを格納 */
      strncpy(title, parts[0], sizeof(title));
      strncpy(date, parts[1], sizeof(date));
      strncpy(author, parts[2], sizeof(author));
      // 編集者については、"ed: "以外の、名前部分のみを抽出
      char *ed_str = strstr(parts[3], "ed:");
      if (ed_str != NULL) {
        ed_str += 3;                      // "ed:" をスキップ
        while (*ed_str == ' ') ed_str++;  // 先頭の空白を削除
        strncpy(ed, ed_str, sizeof(ed));
      } else {
        strncpy(ed, parts[3], sizeof(ed));
      }
      strncpy(id, parts[4], sizeof(id));
    } else if (line_number == 2) {
      // totcrd, ptrcrd, indcrd, valcrd, rhscrd
      // e.g.
      // "           13             1             2            10"
      // フォーマットに従ってそれぞれの整数を抽出
      // 少なくとも1つ以上のスペースがあるとき、スペースで区切る
      sscanf(line, " %d %d %d %d %d", &totcrd, &ptrcrd, &indcrd, &valcrd,
             &rhscrd);
      // sscanf(line, " %d %d %d %d", &totcrd, &ptrcrd, &indcrd, &valcrd);
    } else if (line_number == 3) {
      // mxtype, nrow, ncol, nnzero, neltvl
      // e.g.
      // "rsa                       14            14            30 0"
      sscanf(line, "%3s %d %d %d %d", mxtype, &nrow, &ncol, &nnzero, &neltvl);
    } else if (line_number == 4) {
      // ptrfmt, indfmt, valfmt, rhsfmt
      // e.g.
      // " (26I3) (26I3) (3E25.17)"
      sscanf(line, "%16s %16s %20s %20s)", ptrfmt, indfmt, valfmt, rhsfmt);
    } else {
      // それ以外の行は無視
      break;
    }
    printf("%s\n", line);
  }

  // フォーマット指定子からフィールド幅を取得
  int ptr_field_width = parse_format(ptrfmt);
  int ind_field_width = parse_format(indfmt);
  int val_field_width = parse_format(valfmt);

  CSRMatrix A;
  A.valfmt = 1.0;  // 仮の倍率
  A.num_rows = nrow;
  A.num_cols = ncol;
  A.num_nonzeros = nnzero;

  // メモリを確保する
  A.row_ptr = (int *)malloc(sizeof(int) * (A.num_rows + 1));
  A.col_ind = (int *)malloc(sizeof(int) * (A.num_nonzeros + 1));
  A.values = (double *)malloc(sizeof(double) * (A.num_nonzeros + 1));

  if (A.row_ptr == NULL || A.col_ind == NULL || A.values == NULL) {
    perror("Error allocating memory for matrix data");
    return 1;
  }

  /* ptrcrd 行分の row_ptr データを読み込み */
  int data_line_number = 0;
  for (int i = 0; i < ptrcrd; i++) {
    if (fgets(line, sizeof(line), fp) == NULL) {
      fprintf(stderr, "Error reading row_ptr data\n");
      fclose(fp);
    }
  }
  // データをパースして row_ptr に格納
  for (int j = 0; j < 82 / ptr_field_width; j++) {
    char field[ptr_field_width + 1];
    strncpy(field, line + j * ptr_field_width, ptr_field_width);
    field[ptr_field_width] = '\0';
    if (strlen(field) == 0 || isspace((unsigned char)field[0])) {
      continue;
    }
    int value = atoi(field) - 1;  // インデックスを0-basedに変換
    if (data_line_number < A.num_rows + 1) {
      A.row_ptr[data_line_number++] = value;
    }
  }

  fclose(fp);

  /* 抽出したメタデータを表示 */
  printf("title:  %s\n", title);
  printf("date:   %s\n", date);
  printf("author: %s\n", author);
  printf("ed:     %s\n", ed);
  printf("id:     %s\n", id);
  printf("\n");

  printf("totcrd: %d\n", totcrd);
  printf("ptrcrd: %d\n", ptrcrd);
  printf("indcrd: %d\n", indcrd);
  printf("valcrd: %d\n", valcrd);
  printf("rhscrd: %d\n", rhscrd);
  printf("\n");

  printf("mxtype: %s\n", mxtype);
  printf("nrow:   %d\n", nrow);
  printf("ncol:   %d\n", ncol);
  printf("nnzero: %d\n", nnzero);
  printf("neltvl: %d\n", neltvl);
  printf("\n");

  printf("ptrfmt: %s\n", ptrfmt);
  printf("indfmt: %s\n", indfmt);
  printf("valfmt: %s\n", valfmt);
  printf("rhsfmt: %s\n", rhsfmt);
  printf("\n");

  /**
   * @brief 行列の情報
   *
   */

  return 0;
}
