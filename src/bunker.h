#ifndef BUNKER_H
#define BUNKER_H

#include "raylib.h"

#define BUNKER_COUNT 4
#define BUNKER_WIDTH 22
#define BUNKER_HEIGHT 16

// 1つのシェルター構造体
typedef struct {
  Image image;       // ピクセル直接操作用のCPU画像データ
  Texture2D texture; // 描画用のGPUテクスチャ
  Vector2 pos;       // 配置座標
} Bunker;

// 公開関数
void BunkerGroup_Init(Bunker bunkers[BUNKER_COUNT]);
void BunkerGroup_Draw(const Bunker bunkers[BUNKER_COUNT]);
void BunkerGroup_Unload(Bunker bunkers[BUNKER_COUNT]);

// 弾との衝突判定（着弾した箇所のドットを削る）
bool BunkerGroup_CheckCollision(Bunker bunkers[BUNKER_COUNT],
                                Vector2 bullet_pos, Vector2 bullet_size);

#endif // BUNKER_H
