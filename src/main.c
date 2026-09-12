#include "alien.h"
#include "bunker.h"
#include "player.h"
#include "raylib.h"
#include "ufo.h"

#define GAME_WIDTH 224
#define GAME_HEIGHT 256
#define SCREEN_SCALE 3

int main(void)
{
  const int screen_width = GAME_WIDTH * SCREEN_SCALE;

  const int screen_height = GAME_HEIGHT * SCREEN_SCALE;

  InitWindow(screen_width, screen_height, "SPACE INVADERS (1978)");
  SetTargetFPS(60);

  // 画像の読み込み（相対パス指定）
  Texture2D sprite_sheet = LoadTexture("src/graphics/spritesheet.png");

  // 各ゲームモジュールの初期化

  Player player;
  AlienGroup aliens;
  Bunker bunkers[BUNKER_COUNT];
  Ufo ufo;

  Player_Init(&player);
  AlienGroup_Init(&aliens);
  BunkerGroup_Init(bunkers);
  Ufo_Init(&ufo);

  int score = 0;
  bool game_over = false;

  RenderTexture2D target = LoadRenderTexture(GAME_WIDTH, GAME_HEIGHT);

  while (!WindowShouldClose()) {
    // 1フレームの経過時間をここで1回だけ取得
    float delta = GetFrameTime();

    // ---------------------------------------------------------------------
    // 1. フレーム更新 (Update)
    // ---------------------------------------------------------------------
    if (!game_over) {
      Player_Update(&player, delta);
      AlienGroup_Update(&aliens, delta);
      Ufo_Update(&ufo, delta);

      // --- A. 自機弾の移動と各オブジェクトとの着弾判定 ---
      if (player.bullet.active) {
        Rectangle bullet_rect = Player_GetBulletBounds(&player);

        Vector2 b_pos = {bullet_rect.x, bullet_rect.y};
        Vector2 b_size = {bullet_rect.width, bullet_rect.height};

        // 1. シェルターへの着弾
        if (BunkerGroup_CheckCollision(bunkers, b_pos, b_size)) {
          player.bullet.active = false;
        }
        // 2. UFOへの命中
        else if (ufo.active &&
                 CheckCollisionRecs(bullet_rect, Ufo_GetBounds(&ufo))) {
          score += Ufo_OnHit(&ufo);
          player.bullet.active = false;
        }
        // 3. インベーダーへの命中
        else if (AlienGroup_CheckCollision(&aliens, b_pos, b_size, &score)) {
          player.bullet.active = false;
        }
      }

      // --- B. 敵弾の移動と各オブジェクトとの着弾判定 ---
      if (aliens.bullet.active) {
        Vector2 eb_pos = aliens.bullet.pos;
        Vector2 eb_size = {2.0f, 6.0f};

        // 1. シェルターへの着弾
        if (BunkerGroup_CheckCollision(bunkers, eb_pos, eb_size)) {
          aliens.bullet.active = false;
        }
        // 2. 自機への被弾
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

    // リスタート処理（Rキー）
    if (game_over && IsKeyPressed(KEY_R)) {

      Player_Init(&player);
      AlienGroup_Init(&aliens);
      BunkerGroup_Unload(bunkers);
      BunkerGroup_Init(bunkers);
      Ufo_Init(&ufo);
      score = 0;
      game_over = false;
    }

    // ---------------------------------------------------------------------
    // 2. 内部解像度への描画 (Render to Texture)
    // ---------------------------------------------------------------------

    BeginTextureMode(target);
    ClearBackground(BLACK);

    // UI
    DrawText(TextFormat("SCORE: %04d", score), 8, 8, 8, WHITE);
    DrawText(TextFormat("LIVES: %d", player.lives), 150, 8, 8, GREEN);

    // 各要素の描画
    Ufo_Draw(&ufo, sprite_sheet);
    BunkerGroup_Draw(bunkers);
    AlienGroup_Draw(&aliens, sprite_sheet);
    Player_Draw(&player, sprite_sheet);

    // 防衛ライン（最下部赤線）

    DrawLine(0, 235, GAME_WIDTH, 235, RED);

    if (game_over) {
      DrawText("GAME OVER", 60, 120, 16, RED);
      DrawText("PRESS R TO RESTART", 40, 140, 8, WHITE);
    }
    EndTextureMode();

    // ---------------------------------------------------------------------

    // 3. 画面への拡大描画 (Render to Screen)

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

  // メモリ解放

  UnloadTexture(sprite_sheet);
  BunkerGroup_Unload(bunkers);
  UnloadRenderTexture(target);
  CloseWindow();

  return 0;
}
