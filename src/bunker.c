#include "bunker.h"

// モジュール内非公開：シェルター初期形状のドット作成（アーケード本家風の型抜き）
static Image CreateBunkerImage(void)
{
  // 全体を緑で塗りつぶした画像を作成
  Image img = GenImageColor(BUNKER_WIDTH, BUNKER_HEIGHT, GREEN);

  // 上部の左右の角を削る（斜めカット）
  for (int y = 0; y < 4; y++) {
    for (int x = 0; x < 4 - y; x++) {
      ImageDrawPixel(&img, x, y, BLANK);                    // 左上
      ImageDrawPixel(&img, BUNKER_WIDTH - 1 - x, y, BLANK); // 右上
    }
  }

  // 下部の中央（自機が入るくぼみ）を空洞にする
  for (int y = 8; y < BUNKER_HEIGHT; y++) {
    for (int x = 6; x < 18; x++) {
      // アーケード特有のアーチ形状（中央上部を少し傾斜）
      if (y == 8 && (x < 8 || x > 15))
        continue;
      ImageDrawPixel(&img, x, y, BLANK);
    }
  }

  return img;
}

// 初期化（4つのシェルターを画面下部に均等配置）
void BunkerGroup_Init(Bunker bunkers[BUNKER_COUNT])
{
  float start_x = 24.0f;
  float spacing = 48.0f;
  float pos_y = 192.0f; // 自機の上部に配置

  for (int i = 0; i < BUNKER_COUNT; i++) {
    bunkers[i].image = CreateBunkerImage();
    bunkers[i].texture = LoadTextureFromImage(bunkers[i].image);
    bunkers[i].pos = (Vector2){start_x + i * spacing, pos_y};
  }
}

// 着弾時のドット破壊判定

bool BunkerGroup_CheckCollision(Bunker bunkers[BUNKER_COUNT],
                                Vector2 bullet_pos, Vector2 bullet_size)
{
  Rectangle bullet_rec = {bullet_pos.x, bullet_pos.y, bullet_size.x,
                          bullet_size.y};

  for (int i = 0; i < BUNKER_COUNT; i++) {
    Bunker *b = &bunkers[i];
    Rectangle bunker_rec = {b->pos.x, b->pos.y, BUNKER_WIDTH, BUNKER_HEIGHT};

    // AABBによる大まかな外枠衝突判定
    if (CheckCollisionRecs(bullet_rec, bunker_rec)) {
      // 着弾位置をシェルター内のローカル座標に変換
      int local_x = (int)(bullet_pos.x - b->pos.x);
      int local_y = (int)(bullet_pos.y - b->pos.y);

      // 境界チェック
      if (local_x >= 0 && local_x < BUNKER_WIDTH && local_y >= 0 &&
          local_y < BUNKER_HEIGHT) {
        // 着弾点周辺のピクセルが不透明（壁が存在する）かチェック
        Color color = GetImageColor(b->image, local_x, local_y);

        if (color.a > 0) {
          // 円形にピクセルを消去（透明色で上書き）
          ImageDrawCircle(&b->image, local_x, local_y, 3, BLANK);

          // 変更したピクセルデータをGPUテクスチャに反映
          UpdateTexture(b->texture, b->image.data);
          return true; // 命中して着弾消滅
        }
      }
    }
  }
  return false;
}

// 描画処理

void BunkerGroup_Draw(const Bunker bunkers[BUNKER_COUNT])
{
  for (int i = 0; i < BUNKER_COUNT; i++) {
    DrawTextureV(bunkers[i].texture, bunkers[i].pos, WHITE);
  }
}

// 解放処理（メモリリーク防止のためテクスチャとImageの両方を破棄）
void BunkerGroup_Unload(Bunker bunkers[BUNKER_COUNT])
{

  for (int i = 0; i < BUNKER_COUNT; i++) {
    UnloadTexture(bunkers[i].texture);
    UnloadImage(bunkers[i].image);
  }
}
