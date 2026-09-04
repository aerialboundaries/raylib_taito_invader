#include "raylib.h"
#include "sprites_data.h"
#include <stdio.h>

#define CELL_W 16

#define CELL_H 8
// 横8セル(128px) x 縦4セル(32px) のスプライトシート
#define SHEET_W (CELL_W * 8)
#define SHEET_H (CELL_H * 4)

// パターン画像描画ヘルパー関数
static void DrawPattern(Image *img, const char *pattern[], int lines,
                        int cell_x, int cell_y, Color color)
{
  int start_x = cell_x * CELL_W;
  int start_y = cell_y * CELL_H;

  for (int y = 0; y < lines; y++) {
    for (int x = 0; pattern[y][x] != '\0'; x++) {
      if (pattern[y][x] == '#') {

        ImageDrawPixel(img, start_x + x, start_y + y, color);
      }
    }
  }
}

int main(void)
{
  // 128x32 px の透明画像を作成
  Image sheet = GenImageColor(SHEET_W, SHEET_H, BLANK);

  // --- 1行目 (Y=0): キャラクター本体 ---
  DrawPattern(&sheet, SQUID_DATA[0], 8, 0, 0, WHITE);   // [0,0] イカ 1
  DrawPattern(&sheet, SQUID_DATA[1], 8, 1, 0, WHITE);   // [1,0] イカ 2
  DrawPattern(&sheet, CRAB_DATA[0], 8, 2, 0, WHITE);    // [2,0] カニ 1
  DrawPattern(&sheet, CRAB_DATA[1], 8, 3, 0, WHITE);    // [3,0] カニ 2
  DrawPattern(&sheet, OCTOPUS_DATA[0], 8, 4, 0, WHITE); // [4,0] タコ 1
  DrawPattern(&sheet, OCTOPUS_DATA[1], 8, 5, 0, WHITE); // [5,0] タコ 2
  DrawPattern(&sheet, UFO_DATA, 8, 6, 0, RED);          // [6,0] UFO
  DrawPattern(&sheet, PLAYER_DATA, 8, 7, 0, GREEN);     // [7,0] 自機

  // --- 2行目 (Y=1): 弾 & エフェクト各種 ---
  DrawPattern(&sheet, PLAYER_BULLET_DATA, 8, 0, 1, WHITE); // [0,1] 自機弾
  DrawPattern(&sheet, ENEMY_BULLETS_DATA[0][0], 8, 1, 1, RED); // [1,1] 敵弾A-1
  DrawPattern(&sheet, ENEMY_BULLETS_DATA[0][1], 8, 2, 1, RED); // [2,1] 敵弾A-2
  DrawPattern(&sheet, ENEMY_BULLETS_DATA[1][0], 8, 3, 1, RED); // [3,1] 敵弾B-1
  DrawPattern(&sheet, ENEMY_BULLETS_DATA[1][1], 8, 4, 1, RED); // [4,1] 敵弾B-2
  DrawPattern(&sheet, ENEMY_BULLETS_DATA[2][0], 8, 5, 1, RED); // [5,1] 敵弾C-1
  DrawPattern(&sheet, ENEMY_BULLETS_DATA[2][1], 8, 6, 1, RED); // [6,1] 敵弾C-2
  DrawPattern(&sheet, BULLET_EXPLOSION_DATA, 8, 7, 1, WHITE); // [7,1] 弾相殺

  // --- 3行目 (Y=2): 爆発演出 ＆ バンカー ---
  DrawPattern(&sheet, ALIEN_EXPLOSION_DATA, 8, 0, 2, GREEN); // [0,2] 敵撃破爆破
  DrawPattern(&sheet, PLAYER_EXPLOSION_DATA[0], 8, 1, 2,
              GREEN); // [1,2] 自機爆発 1
  DrawPattern(&sheet, PLAYER_EXPLOSION_DATA[1], 8, 2, 2,
              GREEN);                                // [2,2] 自機爆発 2
  DrawPattern(&sheet, BUNKER_DATA, 16, 3, 2, GREEN); // [3,2]〜[4,3] バンカー

  // ./graphics/spritesheet.png に書き出し
  if (ExportImage(sheet, "graphics/spritesheet.png")) {
    printf("graphics/spritesheet.png (128x32px) の生成に成功しました！\n");
  } else {

    printf("エラー: 画像の書き出しに失敗しました。\n");
  }

  UnloadImage(sheet);
  return 0;
}
