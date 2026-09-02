#include "player.h"

// 初期化処理

void Player_Init(Player *player)
{
  // 画面下部に配置（画面幅224, 高さ256を想定）

  player->pos = (Vector2){(224.0f - PLAYER_WIDTH) / 2.0f, 216.0f};
  player->lives = 3;
  player->active = true;

  // 自機弾の初期化
  player->bullet.pos = (Vector2){0.0f, 0.0f};
  player->bullet.active = false;
}

// フレーム更新処理
void Player_Update(Player *player)
{
  if (!player->active)
    return;

  float delta = GetFrameTime();

  // --- 1. 移動処理 ---
  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
    player->pos.x -= PLAYER_SPEED * delta;
  }
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
    player->pos.x += PLAYER_SPEED * delta;
  }

  // 画面左右端の移動制限（移動可能範囲：X=16〜208）
  if (player->pos.x < 16.0f) {
    player->pos.x = 16.0f;
  }
  if (player->pos.x > 224.0f - 16.0f - PLAYER_WIDTH) {
    player->pos.x = 224.0f - 16.0f - PLAYER_WIDTH;
  }

  // --- 2. 弾の発射処理（画面上に弾が存在しない場合のみ発射可能） ---
  if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_Z)) &&
      !player->bullet.active) {
    player->bullet.pos.x =
        player->pos.x + (PLAYER_WIDTH / 2.0f) - (BULLET_WIDTH / 2.0f);
    player->bullet.pos.y = player->pos.y - BULLET_HEIGHT;
    player->bullet.active = true;
  }

  // --- 3. 弾の移動処理 ---
  if (player->bullet.active) {
    player->bullet.pos.y -= BULLET_SPEED * delta;

    // 画面上端（スコア表示エリア下）に到達したら消去
    if (player->bullet.pos.y < 32.0f) {
      player->bullet.active = false;
    }
  }
}

// 描画処理
void Player_Draw(const Player *player)
{
  // 自機弾の描画
  if (player->bullet.active) {
    DrawRectangleV(player->bullet.pos, (Vector2){BULLET_WIDTH, BULLET_HEIGHT},
                   WHITE);
  }

  if (!player->active)
    return;

  // 砲台の形状を幾何学図形（緑色）で簡易表現
  // 土台部分
  DrawRectangle((int)player->pos.x, (int)player->pos.y + 3, (int)PLAYER_WIDTH,
                5, GREEN);
  // 砲身の凸部分
  DrawRectangle((int)player->pos.x + 5, (int)player->pos.y, 3, 3, GREEN);
  DrawRectangle((int)player->pos.x + 6, (int)player->pos.y - 2, 1, 2, GREEN);
}

// 自機の当たり判定用矩形を取得
Rectangle Player_GetBounds(const Player *player)
{
  return (Rectangle){player->pos.x, player->pos.y, PLAYER_WIDTH, PLAYER_HEIGHT};
}

// 自機弾の当たり判定用矩形を取得
Rectangle Player_GetBulletBounds(const Player *player)
{

  if (!player->bullet.active) {
    return (Rectangle){0.0f, 0.0f, 0.0f, 0.0f};
  }
  return (Rectangle){player->bullet.pos.x, player->bullet.pos.y, BULLET_WIDTH,
                     BULLET_HEIGHT};
}

// 被弾時の処理

void Player_OnHit(Player *player)
{
  if (player->lives > 0) {
    player->lives--;
  }

  // 被弾時は初期位置に戻す
  player->pos.x = (224.0f - PLAYER_WIDTH) / 2.0f;
  player->bullet.active = false;
}
