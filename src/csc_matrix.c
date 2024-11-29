/* src/csc_matrix.c */

#include "csc_matrix.h"

// CSR形式の行列を作成
CSCMatrix* create_csc_matrix(int num_rows, int num_cols, int num_nonzeros,
                             double valfmt) {
  CSCMatrix* matrix = (CSCMatrix*)malloc(sizeof(CSCMatrix));
  matrix->num_rows = num_rows;
  matrix->num_cols = num_cols;
  matrix->num_nonzeros = num_nonzeros;
  matrix->valfmt = valfmt;  // 仮の倍率
  matrix->col_ptr = (int*)malloc(sizeof(int) * (matrix->num_cols + 1));
  matrix->row_ind = (int*)malloc(sizeof(int) * (matrix->num_nonzeros + 1));
  matrix->values = (double*)malloc(sizeof(double) * (matrix->num_nonzeros + 1));

  if (matrix->col_ptr == NULL || matrix->row_ind == NULL ||
      matrix->values == NULL) {
    perror("Error allocating memory for matrix data");
    free(matrix);
    return NULL;
  }

  return matrix;
}

void free_csc_matrix(CSCMatrix* matrix) {
  if (matrix != NULL) {
    free(matrix->col_ptr);
    free(matrix->row_ind);
    free(matrix->values);
    free(matrix);
  }
}

void print_csc_matrix(const CSCMatrix* matrix) {
  printf("matrix: %d x %d, nnz: %d\n", matrix->num_rows, matrix->num_cols,
         matrix->num_nonzeros);
  for (int col = 0; col < matrix->num_cols; col++) {
    int start = matrix->col_ptr[col];
    int end = matrix->col_ptr[col + 1];
    // printf("col: %d, start: %d, end: %d\n", col, start, end);

    for (int idx = start; idx < end; ++idx) {
      int row = matrix->row_ind[idx];
      double val = matrix->values[idx];
      // printf("row: %d, val: %e\n", row, val);

      // 1ベースのインデックスに変換して出力する場合
      printf("(%d, %d)  %e \n", row + 1, col + 1, val);
      if (col != row) {
        printf("(%d, %d)  %e \n", col + 1, row + 1, val);
      }
    }
  }
}

/**
 * read_rb_matrix
 * -> parser fn to convert csc_matrix from rm format matrix
 * -> create_csc_matrix
 *  */
CSCMatrix* read_rb_matrix(const char* filepath) {
  FILE* fp = fopen(filepath, "r");
  if (fp == NULL) {
    perror("Error opening file");
    return NULL;
  }

  /* メタデータを格納する変数 */
  char title[81] = "";
  char date[5] = "";
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

  while (fgets(line, sizeof(line), fp) != NULL) {
    line_number++;  // 1-based で考える,

    // 行末の改行コードを削除
    line[strcspn(line, "\n")] = '\0';

    /* 行番号に応じてメタデータを抽出 */
    if (line_number == 1) {
      // タイトル業のフォーマットに従って、メタデータを抽出
      // e.g.
      // "Oberwolfach/LFAT5; 2004; J. Lienemann et al.; ed: E. Rudnyi |1440    "
      // セミコロンとパイプで区切る
      char* parts[5];  // タイトル, 日付, 著者, 編集者, ID
      char* part = strtok(line, ";|");
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
      char* ed_str = strstr(parts[3], "ed:");
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
      break;
    } else {
      // それ以外の行は無視
      break;
    }
  }
  // printf("%s\n", line);

  /* フォーマット指定子からフィールド幅を取得 */
  int ptr_field_width = parse_format(ptrfmt);
  int ind_field_width = parse_format(indfmt);
  int val_field_width = parse_format(valfmt);
  // printf("val_field_width: %d\n", val_field_width);

  // TODO:
  // valfmtをdouble型でキャストして、create_csc_matrixの引数として入るようにする
  CSCMatrix* matrix = create_csc_matrix(nrow, ncol, nnzero, 1.0);
  if (matrix == NULL) {
    perror("Error allocating memory for CSCMatrix");
    fclose(fp);
    return NULL;
  }

  // バッファサイズを設定
  // valuesのバッファサイズはプログラムでありえる最大のbuf_size
  int buf_size = (val_field_width + 1) * matrix->num_nonzeros + 1;
  char* data_buffer = (char*)malloc(buf_size);
  if (data_buffer == NULL) {
    perror("Error allocating memory for data buffer");
    free_csc_matrix(matrix);
    fclose(fp);
    return NULL;
  }
  data_buffer[0] = '\0';

  int index = 0;

  /* ptrcrd 行分の col_ptr データを読み込み */
  data_buffer[0] = '\0';  // バッファを初期化
  for (int i = 0; i < ptrcrd; i++) {
    if (fgets(line, sizeof(line), fp) == NULL) {
      fprintf(stderr, "Error reading col_ptr data\n");
      free_csc_matrix(matrix);
      fclose(fp);
      return NULL;
    }
    // 行末の改行を削除
    line[strcspn(line, "\n")] = '\0';
    // データを連結
    strcat(data_buffer, line);
    strcat(data_buffer, " ");
  }
  // データをパースして col_ptr に格納
  char* token = strtok(data_buffer, " ");
  index = 0;
  while (token != NULL && index < matrix->num_cols + 1) {
    int value = atoi(token) - 1;  // インデックスを0始まりに調整
    matrix->col_ptr[index++] = value;
    token = strtok(NULL, " ");
  }
  if (index != matrix->num_cols + 1) {
    fprintf(stderr, "Error: col_ptr data count mismatch\n");
    free_csc_matrix(matrix);
    fclose(fp);
    return NULL;
  }

  /* indcrd 行分の row_ind データを読み込み */
  data_buffer[0] = '\0';
  for (int i = 0; i < indcrd; i++) {
    if (fgets(line, sizeof(line), fp) == NULL) {
      fprintf(stderr, "Error reading row_ind data\n");
      free_csc_matrix(matrix);
      fclose(fp);
      return NULL;
    }
    // 行末の改行を削除
    line[strcspn(line, "\n")] = '\0';
    // データを連結
    strcat(data_buffer, line);
    strcat(data_buffer, " ");
  }
  // データをパースして row_ind に格納
  token = strtok(data_buffer, " ");
  index = 0;
  while (token != NULL && index < matrix->num_nonzeros) {
    int value = atoi(token) - 1;  // インデックスを0始まりに調整
    matrix->row_ind[index++] = value;
    token = strtok(NULL, " ");
  }
  if (index != matrix->num_nonzeros) {
    fprintf(stderr, "Error: row_ind data count mismatch\n");
    free_csc_matrix(matrix);
    fclose(fp);
    return NULL;
  }

  /* valcrd 行分の values データを読み込み */
  data_buffer[0] = '\0';
  for (int i = 0; i < valcrd; i++) {
    if (fgets(line, sizeof(line), fp) == NULL) {
      fprintf(stderr, "Error reading values data\n");
      free_csc_matrix(matrix);
      fclose(fp);
      return NULL;
    }
    // 行末の改行を削除
    line[strcspn(line, "\n")] = '\0';
    // データを連結
    strcat(data_buffer, line);
    strcat(data_buffer, " ");
  }
  // データをパースして values に格納
  token = strtok(data_buffer, " ");
  index = 0;
  while (token != NULL && index < matrix->num_nonzeros) {
    double value = atof(token) * matrix->valfmt;  // valfmt を適用
    matrix->values[index++] = value;
    token = strtok(NULL, " ");
  }
  // printf("index: %d\n", index);
  if (index != matrix->num_nonzeros) {
    fprintf(stderr, "Error: values data count mismatch\n");
    free_csc_matrix(matrix);
    fclose(fp);
    return NULL;
  }

  printf("Matrix data read successfully\n");

  /* 抽出したメタデータを表示 */
  printf("--------Matrix Metadata--------\n");
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
  printf("-------------------------------\n");

  /* CSCMatrix matrix の内容を表示 */
  printf("col_ptr:\n");
  for (int i = 0; i < matrix->num_rows + 1; i++) {
    printf("%d ", matrix->col_ptr[i]);
  }
  printf("\n");

  printf("row_ind:\n");
  for (int i = 0; i < matrix->num_nonzeros; i++) {
    printf("%d ", matrix->row_ind[i]);
  }
  printf("\n");

  printf("values:\n");
  for (int i = 0; i < matrix->num_nonzeros; i++) {
    printf("%e ", matrix->values[i]);
  }
  printf("\n");

  fclose(fp);
  return matrix;
}
