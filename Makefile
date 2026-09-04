# フォルダ名を取得して実行ファイル名に設定
DIR_NAME       := $(notdir $(CURDIR))

NAME           := $(DIR_NAME)


CC             := gcc
CFLAGS         := -g -Wall -Wextra -std=c99 -pedantic-errors -MMD -MP

# WSL (Linux) 用のライブラリ
LIBS_WSL       := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Windows用raylibのパス定義
RAYLIB_WIN_DIR := /mnt/c/raylib/w64devkit
CFLAGS_WIN     := -I $(RAYLIB_WIN_DIR)/include

# ソースディレクトリ
SRC_DIR        := src
OBJDIR         := obj

# 【修正箇所1】src/*.c を正確に指定 (引数にスペースを入れない)
SRCS           := $(wildcard $(SRC_DIR)/*.c)

# 【修正箇所2】src/%.c を obj/%.o に置換
OBJS           := $(patsubst $(SRC_DIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
DEPS           := $(OBJS:.o=.d)

# Windowsビルド用
OBJDIR_WIN     := obj_win
OBJS_WIN       := $(patsubst $(SRC_DIR)/%.c,$(OBJDIR_WIN)/%.o,$(SRCS))
DEPS_WIN       := $(OBJS_WIN:.o=.d)

.PHONY: all
all: $(NAME)

# ----------------------------------------------------
# 1. WSL (Linux) 用ビルド
# ----------------------------------------------------
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS_WSL)

# 【修正箇所3】依存関係の対応関係を src/%.c -> obj/%.o に明示
$(OBJDIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ----------------------------------------------------
# 依存関係ファイルの読み込み
# ----------------------------------------------------
-include $(DEPS)
-include $(DEPS_WIN)


# ----------------------------------------------------
# ユーティリティターゲット
# ----------------------------------------------------
.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(OBJDIR_WIN)

.PHONY: fclean
fclean: clean
	rm -f $(NAME) $(NAME).exe

.PHONY: re
re: fclean all

# デバッグ用ターゲット
.PHONY: debug

debug: CFLAGS += -g -fsanitize=integer -fsanitize=address -fsanitize=leak
debug: re

# 動作確認用
.PHONY: test run
test: all
	./$(NAME)

run: all

	./$(NAME)

# --- 通常の Linux用 Makefile の末尾に追記 ---
.PHONY: win
win:
	make -f Makefile.win

.PHONY: win-run winrun
win-run winrun:
	make -f Makefile.win run


# ----------------------------------------------------
# LSP (Neovim / clangd) 用のコンパイルデータベース生成
# ----------------------------------------------------
.PHONY: compiledb

compiledb: fclean
	bear -- $(MAKE)
