#include "alien.h"
#include "bunker.h"
#include "player.h"
#include "raylib.h"

#define GAME_WIDTH 224
#define GAME_HEIGHT 256
#define SCREEN_SCALE 3

int main(void)
{

  const int screen_width = GAME_WIDTH * SCREEN_SCALE;
  const int screen_height = GAME_HEIGHT * SCREEN_SCALE;

  InitWindow(screen_width, screen_height, "SPACE INVADERS (1978)");
  SetTargetFPS(60);

  Player player;
  AlienGroup aliens;
  Bunker bunkers[BUNKER_COUNT];

  Player_Init(&player);
  AlienGroup_Init(&aliens);
  BunkerGroup_Init(bunkers);

  int score = 0;

  bool game_over = false;

  RenderTexture2D target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);

  while (!WindowShouldClose()) {
    // ---------------------------------------------------------------------
    // 1. 更新処理 (Update)
    // ---------------------------------------------------------------------
    if (!game_over) {
      Player_Update(&player);
      AlienGroup_Update(&aliens);

      // --- A. 自機弾の処理と各オブジェクト判定 ---
      if (player.bullet.active) {
        Rectangle bullet_rect = Player_GetBulletBounds(&player);
        Vector2 b_pos = {bullet_rect.x, bullet_rect.y};
        Vector2 b_size = {bullet_rect.width, bullet_rect.height};

        // 1. シェルターへの着弾判定

        if (BunkerGroup_CheckCollision(bunkers, b_pos, b_size)) {

          player.bullet.active = false;
        }
        // 2. インベーダーへの着弾判定
        else if (AlienGroup_CheckCollision(&aliens, b_pos, b_size, &score)) {
          player.bullet.active = false;
        }
      }

      // --- B. 敵弾の処理と各オブジェクト判定 ---
      if (aliens.bullet.active) {
        Vector2 eb_pos = aliens.bullet.pos;
        Vector2 eb_size = {2.0f, 6.0f};

        // 1. シェルターへの着弾判定
        if (BunkerGroup_CheckCollision(bunkers, eb_pos, eb_size)) {
          aliens.bullet.active = false;

        }
        // 2. 自機への被弾判定
        else {

          Rectangle bullet_rec = {eb_pos.x, eb_pos.y, eb_size.x, eb_size.y};
          Rectangle player_rec = Player_GetBounds(&player);

          if (CheckCollisionRecs(bullet_rec, player_rec)) {
            aliens.bullet.active = false;
            Player_OnHit(&player);

            if (player.lives <= 0) {
              game_over = true;
            }
          }
        }
      }

      // --- C. インベーダー侵略完了チェック ---
      if (AlienGroup_HasReachedBottom(&aliens, 216.0f)) {
        game_over = true;
      }
    }

    // リスタート処理

    if (game_over && IsKeyPressed(KEY_R)) {
      Player_Init(&player);
      AlienGroup_Init(&aliens);
      BunkerGroup_Unload(bunkers);
      BunkerGroup_Init(bunkers);
      score = 0;
      game_over = false;
    }

    // ---------------------------------------------------------------------
    // 2. 描画処理 (Render)
    // ---------------------------------------------------------------------
    BeginTextureMode(target);
    ClearBackground(BLACK);

    // UI
    DrawText(TextFormat("SCORE: %04d", score), 8, 8, 8, WHITE);
    DrawText(TextFormat("LIVES: %d", player.lives), 150, 8, 8, GREEN);

    // ゲームオブジェクト描画
    BunkerGroup_Draw(bunkers);
    AlienGroup_Draw(&aliens);
    Player_Draw(&player);

    // 防衛ライン
    DrawLine(0, 235, GAME_WIDTH, 235, RED);

    if (game_over) {
      DrawText("GAME OVER", 60, 120, 16, RED);
      DrawText("PRESS R TO RESTART", 40, 140, 8, WHITE);
    }

    EndTextureMode();

    // ---------------------------------------------------------------------
    // 3. スケーリング描画
    // ---------------------------------------------------------------------
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(target.texture,
                   (Rectangle){0, 0, (float)target.texture.width,
                               (float)-target.texture.height},
                   (Rectangle){0, 0, (float)screen_width, (float)screen_height},
                   (Vector2){0, 0}, 0.0f, WHITE);
    EndDrawing();
  }

  // 解放処理
  BunkerGroup_Unload(bunkers);

  UnloadRenderTexture(target);

  CloseWindow();

  return 0;
}
