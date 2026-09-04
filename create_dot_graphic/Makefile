# コンパイラとフラグ設定
CC       := gcc
CFLAGS   := -std=c99 -Wall -Wextra
LIBS     := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# ディレクトリとファイル設定
SRC      := gen_spritesheet.c
TARGET   := gen_spritesheet

OUT_DIR  := ./graphics
OUT_PNG  := $(OUT_DIR)/spritesheet.png

.PHONY: all run clean

# デフォルトターゲット：ビルドと画像生成を実行
all: run


# ディレクトリを作成してから生成プログラムを実行

run: $(TARGET)
	@mkdir -p $(OUT_DIR)
	./$(TARGET)

# プログラムのコンパイル
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $< $(LIBS) -o $@

# 生成物（実行ファイルと画像）の削除
clean:
	rm -f $(TARGET)
	rm -rf $(OUT_DIR)
