#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"

// 自機および弾のサイズ・速度定数（224x256 画面基準）
#define PLAYER_WIDTH 16.0f
#define PLAYER_HEIGHT 8.0f

#define PLAYER_SPEED 80.0f

#define BULLET_WIDTH 1.0f
#define BULLET_HEIGHT 4.0f
#define BULLET_SPEED 200.0f

// 自機弾構造体
typedef struct {
  Vector2 pos;
  bool active; // 画面上に存在するかどうか（同時発射1発制限に使用）
} PlayerBullet;

// 自機構造体
typedef struct {
  Vector2 pos;
  int lives;   // 残機数
  bool active; // 被弾後のスポーン待ちなどの状態管理用
  PlayerBullet bullet;
} Player;

// 公開関数
void Player_Init(Player *player);
void Player_Update(Player *player);
void Player_Draw(const Player *player);

Rectangle Player_GetBounds(const Player *player);
Rectangle Player_GetBulletBounds(const Player *player);
void Player_OnHit(Player *player);

#endif // PLAYER_H
