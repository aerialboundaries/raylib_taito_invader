#ifndef ALIEN_H
#define ALIEN_H

#include "raylib.h"

// インベーダーの配置定数
#define ALIEN_ROWS 5
#define ALIEN_COLS 11
#define TOTAL_ALIENS (ALIEN_ROWS * ALIEN_COLS)

// 描画・サイズ定数（アーケード解像度 224x256 を想定）
#define ALIEN_WIDTH 12.0f
#define ALIEN_HEIGHT 8.0f
#define ALIEN_SPACING_X 16.0f
#define ALIEN_SPACING_Y 16.0f

// 1匹のインベーダーを表す構造体
typedef struct {
  Vector2 pos; // 画面上の座標

  int type;       // 0:最上段(30点), 1-2:中段(20点), 3-4:下段(10点)
  int anim_frame; // アニメーションコマ (0 または 1)
  bool active;    // 生存フラグ (true: 生存, false: 撃破)
} Alien;

// 敵弾構造体
typedef struct {
  Vector2 pos;
  bool active;
  float speed;
} AlienBullet;

// 55匹の群れ全体を管理する構造体
typedef struct {
  Alien aliens[ALIEN_ROWS][ALIEN_COLS];
  int direction; // 移動方向 (1: 右, -1: 左)

  float move_timer;    // 移動間隔計測用タイマー
  float move_interval; // 現在の移動速度（秒）
  int alive_count;     // 生存数
  int anim_state;      // 全体のアニメーションコマ同期用 (0 または 1)
  AlienBullet bullet;  // 敵弾（画面内に同時に1発として管理）
} AlienGroup;

// 公開関数
void AlienGroup_Init(AlienGroup *group);
void AlienGroup_Update(AlienGroup *group);
void AlienGroup_Draw(const AlienGroup *group);
bool AlienGroup_CheckCollision(AlienGroup *group, Vector2 bullet_pos,
                               Vector2 bullet_size, int *out_score);
bool AlienGroup_HasReachedBottom(const AlienGroup *group, float bottom_y);

#endif // ALIEN_H
