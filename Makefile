# コンパイラとフラグの設定
CC = gcc
CFLAGS = -I./inc -Wall -Wextra -O2

# ディレクトリの設定
OBJDIR = obj
SRCDIR = src
INCDIR = inc
BINDIR = bin
EXAMPLES_DIR = examples

# メインプログラムのソースとオブジェクト
MAIN_SRC = main.c
MAIN_OBJ = $(OBJDIR)/main.o

# src ディレクトリ内のソースとオブジェクト
SRC_SOURCES = $(wildcard $(SRCDIR)/*.c)
SRC_OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SRC_SOURCES))

# examples ディレクトリ内のソースとバイナリ
EXAMPLES_SRCS = $(wildcard $(EXAMPLES_DIR)/*.c)
EXAMPLES_BIN = $(patsubst $(EXAMPLES_DIR)/%.c,$(BINDIR)/%,$(EXAMPLES_SRCS))

# デフォルトターゲット
all: directories numana $(EXAMPLES_BIN)

# ディレクトリの作成
directories:
	@mkdir -p $(OBJDIR) $(BINDIR)

# src/*.c のオブジェクトファイルのコンパイル
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# main.c のオブジェクトファイルのコンパイル
$(MAIN_OBJ): $(MAIN_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

# メインプログラムのリンク
numana: $(SRC_OBJECTS) $(MAIN_OBJ)
	$(CC) $(SRC_OBJECTS) $(MAIN_OBJ) -o $(BINDIR)/numana -lm

# examples/*.c のバイナリビルド
$(BINDIR)/%: $(EXAMPLES_DIR)/%.c $(SRC_OBJECTS)
	$(CC) $(CFLAGS) $< $(SRC_OBJECTS) -o $@ -lm

# クリーンアップ
clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/*

.PHONY: all clean directories
