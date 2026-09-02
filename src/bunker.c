#include "bunker.h"

// モジュール内非公開：タイトーオリジナル（22x16px）の正確なシェルター形状をドット生成

static Image CreateBunkerImage(void)
{
  // 全体を緑で塗りつぶした画像を作成 (幅22 x 高さ16)
  Image img = GenImageColor(BUNKER_WIDTH, BUNKER_HEIGHT, GREEN);

  // 1. 上部左右の傾斜カット (角を削る)
  // 0行目: 左右4px消去, 1行目: 左右3px消去, 2行目: 左右2px消去, 3行目:
  // 左右1px消去
  for (int y = 0; y < 4; y++) {
    int cut_pixels = 4 - y;

    for (int x = 0; x < cut_pixels; x++) {

      ImageDrawPixel(&img, x, y, BLANK);                    // 左上
      ImageDrawPixel(&img, BUNKER_WIDTH - 1 - x, y, BLANK); // 右上
    }
  }

  // 2. 下部中央のアーチ構造 (自機の頭が入る空洞)
  // 幅22pxの中央（X=6 〜 15）のエリアをくり抜く
  for (int y = 9; y < BUNKER_HEIGHT; y++) {
    for (int x = 6; x <= 15; x++) {
      // 最上段(y=9)は中央寄りのみ削り、なだらかなアーチにする
      if (y == 9 && (x < 8 || x > 13))
        continue;

      ImageDrawPixel(&img, x, y, BLANK);
    }
  }

  // 3. 左右足元の1ピクセル凹み (本家のドット絵再現)
  ImageDrawPixel(&img, 0, BUNKER_HEIGHT - 1, BLANK);
  ImageDrawPixel(&img, BUNKER_WIDTH - 1, BUNKER_HEIGHT - 1, BLANK);

  return img;
}

// 初期化（本家の正確な座標 32, 72, 112, 152 に配置）
void BunkerGroup_Init(Bunker bunkers[BUNKER_COUNT])
{
  // 本家アーケードの配置X座標（各シェルターの左端位置）
  static const float bunker_x_positions[BUNKER_COUNT] = {32.0f, 72.0f, 112.0f,
                                                         152.0f};
  const float pos_y = 192.0f; // 本家の設置基準高さ

  for (int i = 0; i < BUNKER_COUNT; i++) {
    bunkers[i].image = CreateBunkerImage();
    bunkers[i].texture = LoadTextureFromImage(bunkers[i].image);
    bunkers[i].pos = (Vector2){bunker_x_positions[i], pos_y};
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

    // AABBによる外枠判定
    if (CheckCollisionRecs(bullet_rec, bunker_rec)) {
      // 弾の矩形範囲内にあるピクセルを走査して壁が存在するかチェック
      int start_x = (int)(bullet_pos.x - b->pos.x);
      int start_y = (int)(bullet_pos.y - b->pos.y);
      int end_x = start_x + (int)bullet_size.x;
      int end_y = start_y + (int)bullet_size.y;

      // ローカル座標のクランプ処理
      if (start_x < 0)
        start_x = 0;
      if (start_y < 0)
        start_y = 0;
      if (end_x >= BUNKER_WIDTH)
        end_x = BUNKER_WIDTH - 1;
      if (end_y >= BUNKER_HEIGHT)
        end_y = BUNKER_HEIGHT - 1;

      for (int ly = start_y; ly <= end_y; ly++) {
        for (int lx = start_x; lx <= end_x; lx++) {
          Color color = GetImageColor(b->image, lx, ly);

          // 壁（不透明ピクセル）に衝突した場合
          if (color.a > 0) {
            // 着弾点を中心に半径3pxのクレーターを削る
            ImageDrawCircle(&b->image, lx, ly, 3, BLANK);

            // テクスチャの更新
            UpdateTexture(b->texture, b->image.data);
            return true; // 命中したため弾を消滅させる
          }
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
