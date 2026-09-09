#include "ufo.h"

// 初期化
void Ufo_Init(Ufo *ufo)
{
  ufo->pos = (Vector2){-UFO_WIDTH, 24.0f}; // 画面上部（Y=24）
  ufo->active = false;
  ufo->direction = 1;
  ufo->timer = 0.0f;
  ufo->score_display = 0;
  ufo->score_timer = 0.0f;

  ufo->score_pos = (Vector2){0.0f, 0.0f};
}

// フレーム更新
void Ufo_Update(Ufo *ufo, float delta)
{

  // 撃破スコア表示のタイマー処理
  if (ufo->score_display > 0) {
    ufo->score_timer -= delta;
    if (ufo->score_timer <= 0.0f) {
      ufo->score_display = 0;
    }
  }

  // 非アクティブ時：出現タイマー更新
  if (!ufo->active) {
    ufo->timer += delta;
    if (ufo->timer >= UFO_SPAWN_INTERVAL) {
      ufo->timer = 0.0f;
      ufo->active = true;

      // 左右どちらから出現するかランダム選択
      if (GetRandomValue(0, 1) == 0) {
        ufo->pos.x = -UFO_WIDTH;
        ufo->direction = 1; // 右へ移動
      } else {
        ufo->pos.x = 224.0f;
        ufo->direction = -1; // 左へ移動
      }
    }
    return;
  }

  // 移動処理
  ufo->pos.x += ufo->direction * UFO_SPEED * delta;

  // 画面外に抜けたら消去
  if ((ufo->direction == 1 && ufo->pos.x > 224.0f) ||
      (ufo->direction == -1 && ufo->pos.x < -UFO_WIDTH)) {
    ufo->active = false;
  }
}

// 撃破処理（本家のスコアテーブルからランダム還元）
int Ufo_OnHit(Ufo *ufo)
{
  static const int scores[] = {50, 100, 150, 300};
  int pts = scores[GetRandomValue(0, 3)];

  ufo->score_display = pts;
  ufo->score_timer = 1.0f; // 1秒間スコア数値を表示
  ufo->score_pos = ufo->pos;
  ufo->active = false;

  return pts;
}

// 当たり判定用矩形
Rectangle Ufo_GetBounds(const Ufo *ufo)
{
  if (!ufo->active)
    return (Rectangle){0, 0, 0, 0};
  return (Rectangle){ufo->pos.x, ufo->pos.y, UFO_WIDTH, UFO_HEIGHT};
}

// 描画処理（赤色のアッパー・円盤形状）
void Ufo_Draw(const Ufo *ufo)
{
  // 撃破スコアの表示
  if (ufo->score_display > 0) {
    DrawText(TextFormat("%d", ufo->score_display), (int)ufo->score_pos.x,
             (int)ufo->score_pos.y, 8, RED);
  }

  if (!ufo->active)
    return;

  int px = (int)ufo->pos.x;
  int py = (int)ufo->pos.y;

  // UFOのドット絵風描画（赤色）
  DrawRectangle(px + 5, py, 6, 1, RED);
  DrawRectangle(px + 3, py + 1, 10, 1, RED);
  DrawRectangle(px + 2, py + 2, 12, 1, RED);
  DrawRectangle(px + 1, py + 3, 14, 2, RED);
  // 下部の突起（ランプ）
  DrawRectangle(px + 2, py + 5, 2, 1, RED);
  DrawRectangle(px + 6, py + 5, 4, 1, RED);
  DrawRectangle(px + 12, py + 5, 2, 1, RED);
}
