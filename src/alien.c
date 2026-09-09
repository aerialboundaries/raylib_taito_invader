#include "alien.h"
#include <raylib.h>
#include <stdlib.h> // rand()用

// モジュール内非公開の補助関数：残数に応じた速度の更新
static void UpdateMoveInterval(AlienGroup *group)
{
  // 55体で0.5秒間隔、残り1体になると0.02秒間隔（超高速）まで加速
  float ratio = (float)group->alive_count / (float)TOTAL_ALIENS;
  group->move_interval = 0.02f + (0.48f * ratio);
}

// モジュール内非公開の補助関数：各列の一番下にいるインベーダーから敵弾を出す
static void TrySpawnBullet(AlienGroup *group)
{
  if (group->bullet.active || group->alive_count == 0)
    return;

  // 生きているインベーダーが存在する列をランダムに探す
  int valid_cols[ALIEN_COLS];
  int valid_col_count = 0;

  for (int c = 0; c < ALIEN_COLS; c++) {
    for (int r = ALIEN_ROWS - 1; r >= 0; r--) {
      if (group->aliens[r][c].active) {
        valid_cols[valid_col_count++] = c;
        break;
      }
    }
  }

  if (valid_col_count > 0 && (GetRandomValue(0, 100) < 5)) { // 確率で発射
    int random_col = valid_cols[GetRandomValue(0, valid_col_count - 1)];

    // その列の一番下にいるインベーダーを特定
    for (int r = ALIEN_ROWS - 1; r >= 0; r--) {
      if (group->aliens[r][random_col].active) {
        group->bullet.pos = (Vector2){

            group->aliens[r][random_col].pos.x + ALIEN_WIDTH / 2.0f - 1.0f,
            group->aliens[r][random_col].pos.y + ALIEN_HEIGHT};
        group->bullet.active = true;
        break;
      }
    }
  }
}

// 初期化
void AlienGroup_Init(AlienGroup *group)
{

  group->direction = 1;
  group->move_timer = 0.0f;
  group->alive_count = TOTAL_ALIENS;
  group->anim_state = 0;
  UpdateMoveInterval(group);

  // 敵弾の初期化
  group->bullet.active = false;
  group->bullet.speed = 120.0f; // 敵弾の落下速度

  // 5行11列の配置
  float start_x = 24.0f;
  float start_y = 40.0f;

  for (int r = 0; r < ALIEN_ROWS; r++) {
    for (int c = 0; c < ALIEN_COLS; c++) {
      Alien *a = &group->aliens[r][c];
      a->pos.x = start_x + c * ALIEN_SPACING_X;
      a->pos.y = start_y + r * ALIEN_SPACING_Y;
      a->type = r; // 0: 最上段, 1-2: 中段, 3-4: 下段
      a->anim_frame = 0;
      a->active = true;
    }
  }
}

// フレーム更新処理
void AlienGroup_Update(AlienGroup *group, float delta)
{
  if (group->alive_count == 0)
    return;

  // --- 1. 敵弾の更新 ---
  if (group->bullet.active) {
    group->bullet.pos.y += group->bullet.speed * delta;
    // 画面外（下端）に出たら消去
    if (group->bullet.pos.y > 256.0f) {
      group->bullet.active = false;
    }
  } else {
    TrySpawnBullet(group);
  }

  // --- 2. インベーダーのステップ移動処理 ---
  group->move_timer += delta;
  if (group->move_timer < group->move_interval)
    return;

  group->move_timer = 0.0f;
  group->anim_state = !group->anim_state; // コマ反転

  float step_x = 3.0f * group->direction;
  bool hit_edge = false;

  // 全体の移動および画面端のチェック
  for (int r = 0; r < ALIEN_ROWS; r++) {
    for (int c = 0; c < ALIEN_COLS; c++) {
      Alien *a = &group->aliens[r][c];

      if (!a->active)
        continue;

      a->pos.x += step_x;
      a->anim_frame = group->anim_state;

      // 画面左右の端に達したかチェック（画面幅 224 を想定）
      if ((a->pos.x < 8.0f && group->direction == -1) ||
          (a->pos.x + ALIEN_WIDTH > 216.0f && group->direction == 1)) {
        hit_edge = true;
      }
    }
  }

  // 端に達していたら方向反転 ＋ 一段降下
  if (hit_edge) {

    group->direction *= -1;
    for (int r = 0; r < ALIEN_ROWS; r++) {
      for (int c = 0; c < ALIEN_COLS; c++) {
        if (group->aliens[r][c].active) {
          group->aliens[r][c].pos.y += 8.0f; // 8ピクセル下へ
        }
      }
    }
  }
}

// 自機弾との当たり判定処理
bool AlienGroup_CheckCollision(AlienGroup *group, Vector2 bullet_pos,
                               Vector2 bullet_size, int *out_score)
{
  Rectangle bullet_rec = {bullet_pos.x, bullet_pos.y, bullet_size.x,
                          bullet_size.y};

  for (int r = 0; r < ALIEN_ROWS; r++) {
    for (int c = 0; c < ALIEN_COLS; c++) {
      Alien *a = &group->aliens[r][c];
      if (!a->active)
        continue;

      Rectangle alien_rec = {a->pos.x, a->pos.y, ALIEN_WIDTH, ALIEN_HEIGHT};

      if (CheckCollisionRecs(bullet_rec, alien_rec)) {
        a->active = false;
        group->alive_count--;
        UpdateMoveInterval(group); // 速度再計算

        // スコア加算テーブル（本家仕様）
        if (out_score != NULL) {
          if (a->type == 0)
            *out_score += 30; // 最上段

          else if (a->type <= 2)
            *out_score += 20; // 中段
          else
            *out_score += 10; // 下段
        }

        return true; // 命中した
      }
    }
  }
  return false;
}

// 侵略完了（最下段到達）チェック
bool AlienGroup_HasReachedBottom(const AlienGroup *group, float bottom_y)
{

  for (int r = 0; r < ALIEN_ROWS; r++) {
    for (int c = 0; c < ALIEN_COLS; c++) {
      if (group->aliens[r][c].active) {
        if (group->aliens[r][c].pos.y + ALIEN_HEIGHT >= bottom_y) {
          return true;
        }
      }
    }
  }
  return false;
}

// 描画処理
void AlienGroup_Draw(const AlienGroup *group)
{
  // 敵弾の描画
  if (group->bullet.active) {
    DrawRectangleV(group->bullet.pos, (Vector2){2.0f, 6.0f}, WHITE);
  }

  // インベーダーの描画（色分けと簡易アニメーション表現）
  for (int r = 0; r < ALIEN_ROWS; r++) {
    for (int c = 0; c < ALIEN_COLS; c++) {
      const Alien *a = &group->aliens[r][c];
      if (!a->active)
        continue;

      Color color = MAGENTA;
      if (a->type == 0)
        color = YELLOW; // 30点
      else if (a->type <= 2)
        color = GREEN; // 20点
      else
        color = SKYBLUE; // 10点

      // 本来は画像テクスチャを描画する部分。ここでは仮で基本形を描画
      // アニメーションコマによって幅を少し変化させて足を表現
      float anim_offset = (a->anim_frame == 1) ? 2.0f : 0.0f;
      DrawRectangle((int)a->pos.x, (int)a->pos.y, (int)ALIEN_WIDTH,
                    (int)ALIEN_HEIGHT, color);

      // 目に見立てた黒点（アニメーションの確認用）
      DrawRectangle((int)a->pos.x + 2 + (int)anim_offset, (int)a->pos.y + 2, 2,
                    2, BLACK);
    }
  }
}
