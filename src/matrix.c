#include "matrix.h"

int parse_format(const char* fmt_str) {
  if (fmt_str == NULL) {
    fprintf(stderr, "Error: fmt_str is NULL\n");
    return -1;  // エラーコードを返す
  }
  /* フォーマット指定子からフィールド幅を取得する関数 */
  // e.g. "(16I5)" -> 5
  int field_width = 0;
  const char* p = fmt_str;

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
