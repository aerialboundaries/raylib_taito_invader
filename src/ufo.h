#ifndef UFO_H
#define UFO_H

#include "raylib.h"

#define UFO_WIDTH 16.0f
#define UFO_HEIGHT 7.0f
#define UFO_SPEED 50.0f
#define UFO_SPAWN_INTERVAL 15.0f // 出現間隔（秒）

typedef struct {
  Vector2 pos;
  bool active;

  int direction;     // 1: 右移動, -1: 左移動
  float timer;       // 出現用タイマー
  int score_display; // 撃破時に表示するスコア（0の場合は表示なし）
  float score_timer; // スコア表示の維持タイマー
  Vector2 score_pos; // スコア表示位置
} Ufo;

// 公開関数
void Ufo_Init(Ufo *ufo);
void Ufo_Update(Ufo *ufo, float delta);
void Ufo_Draw(const Ufo *ufo, Texture2D texture);
Rectangle Ufo_GetBounds(const Ufo *ufo);
int Ufo_OnHit(Ufo *ufo);

#endif // UFO_H
