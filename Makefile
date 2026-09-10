# ==============================================================================
# raylib Cross-Platform Makefile (WSL Native & Windows via w64devkit)
# ==============================================================================
# 【基本の使い方】
#   make         : ヘルプ（使い方）の表示
#   make build   : WSL(Linux)向けデバッグビルド & 実行ファイルの作成
#   make run     : WSL(Linux)向けビルド & 実行
#   make win     : Windows(.exe)向けビルド (w64devkit使用)
#   make win-run : Windows(.exe)向けビルド & PowerShell経由で実行
#   make release : Linux向けリリースビルド (-O2 最適化)
#   make win-release : Windows向けリリースビルド (-O2 最適化)
# ==============================================================================

# ------------------------------------------------------------------------------
# 1. プロジェクト基本設定・変数定義
# ------------------------------------------------------------------------------
# 実行ファイル名をカレントフォルダ名から自動取得
DIR_NAME        := $(notdir $(CURDIR))
NAME            := $(DIR_NAME)

# WSL(Linux)用コンパイラ設定
CC              := gcc

# Windows(w64devkit)用コンパイラ & 環境変数設定
export WSLENV   := PATH/l
W64DEVKIT_BIN   := /mnt/c/raylib/w64devkit/bin
CC_WIN          := $(W64DEVKIT_BIN)/gcc.exe

CFLAGS          := -g -Wall -Wextra -std=c99 -pedantic-errors -MMD -MP

# ディレクトリ設定
SRC_DIR         := src
OBJDIR          := obj
OBJDIR_WIN      := obj_win

# ソースファイルとオブジェクトファイルの自動抽出
SRCS            := $(wildcard $(SRC_DIR)/*.c)

OBJS            := $(patsubst $(SRC_DIR)/%.c,$(OBJDIR)/%.o,$(SRCS))
DEPS            := $(OBJS:.o=.d)

OBJS_WIN        := $(patsubst $(SRC_DIR)/%.c,$(OBJDIR_WIN)/%.o,$(SRCS))
DEPS_WIN        := $(OBJS_WIN:.o=.d)

# ------------------------------------------------------------------------------
# 2. ライブラリ & パス設定

# ------------------------------------------------------------------------------
# WSL (Linux) 用設定
LIBS_WSL        := -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Windows (w64devkit) 用設定 (静的リンク -static を追加して依存DLLエラーを完全防止)
RAYLIB_WIN_DIR  := C:/raylib/w64devkit
CFLAGS_WIN      := $(CFLAGS) -I$(RAYLIB_WIN_DIR)/include
LIBS_WIN        := -L$(RAYLIB_WIN_DIR)/lib -static -lraylib -lopengl32 -lgdi32 -lwinmm

# ------------------------------------------------------------------------------
# 3. メイン・ヘルプ ターゲット
# ------------------------------------------------------------------------------
.PHONY: default help
default: help


help:
	@echo "========================================================================"

	@echo "  raylib Project Makefile - Usage Help"
	@echo "========================================================================"

	@echo "  [開発・実行 (WSL / Linux)]"
	@echo "    make build        : Linux向け開発用ビルド (-g デバッグ情報付き)"
	@echo "    make run (test)   : Linux向けビルド & ゲームの実行"
	@echo "    make debug        : Sanitizer(メモリ破壊・リーク検出)付きビルド"
	@echo ""
	@echo "  [開発・実行 (Windows .exe)]"

	@echo "    make win          : Windows向け開発用ビルド ($(NAME).exe)"
	@echo "    make win-run      : Windows向けビルド & PowerShell経由で実行"
	@echo ""

	@echo "  [リリースビルド (-O2 最適化)]"
	@echo "    make release      : Linux向け最適化ビルド"
	@echo "    make win-release  : Windows向け最適化ビルド"
	@echo ""
	@echo "  [ユーティリティ・Neovim連携]"

	@echo "    make clean        : オブジェクトファイル (obj/, obj_win/) の削除"
	@echo "    make fclean       : 生成物（実行ファイル・obj等）の完全削除"
	@echo "    make re           : 完全再ビルド (fclean -> build)"
	@echo "    make compiledb    : Neovim(clangd)用 compile_commands.json の生成"
	@echo "========================================================================"

# ------------------------------------------------------------------------------
# 4. WSL (Linux) 用ビルド規則
# ------------------------------------------------------------------------------
.PHONY: build
build: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS_WSL)

$(OBJDIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ------------------------------------------------------------------------------
# 5. Windows (.exe) 用ビルド規則 (w64devkit 直接呼出)
# ------------------------------------------------------------------------------
.PHONY: win
win: $(NAME).exe

$(NAME).exe: $(OBJS_WIN)
	export PATH="$(W64DEVKIT_BIN):$$PATH" && $(CC_WIN) $(CFLAGS_WIN) -o $@ $^ $(LIBS_WIN)
	@chmod 755 $@


$(OBJDIR_WIN)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	export PATH="$(W64DEVKIT_BIN):$$PATH" && $(CC_WIN) $(CFLAGS_WIN) -c $< -o $@

# ------------------------------------------------------------------------------
# 6. リリリース・デバッグ・特殊ビルド設定
# ------------------------------------------------------------------------------
.PHONY: release win-release
release: CFLAGS := $(filter-out -g,$(CFLAGS)) -O2

release: fclean build

win-release: CFLAGS := $(filter-out -g,$(CFLAGS)) -O2
win-release: fclean win

.PHONY: debug
debug: CFLAGS += -fsanitize=integer -fsanitize=address -fsanitize=leak
debug: fclean build


# ------------------------------------------------------------------------------
# 7. 実行・クリーンアップ・開発支援ターゲット
# ------------------------------------------------------------------------------
.PHONY: run test win-run winrun
run test: build

	./$(NAME)

win-run winrun: win
	powershell.exe -Command "Set-Location '$$(wslpath -w .)'; .\$(NAME).exe"

.PHONY: clean fclean re
clean:
	rm -rf $(OBJDIR) $(OBJDIR_WIN)

fclean: clean
	rm -f $(NAME) $(NAME).exe

re: fclean build


.PHONY: compiledb
compiledb: fclean
	bear -- $(MAKE) build

# ------------------------------------------------------------------------------
# 8. 依存関係ファイル (.d) の読み込み
# ------------------------------------------------------------------------------
-include $(DEPS)
-include $(DEPS_WIN)
