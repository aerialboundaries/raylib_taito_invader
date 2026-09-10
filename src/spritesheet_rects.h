#ifndef SPRITESHEET_RECTS_H
#define SPRITESHEET_RECTS_H

#include "raylib.h"

// -----------------------------------------------------------------------------
// 1行目 (cell_y = 0, Y = 0) : キャラクター本体
// -----------------------------------------------------------------------------

// 1. イカ（上段：8x8px）
static const Rectangle REC_ALIEN_SQUID[2] = {

    {0.0f, 0.0f, 8.0f, 8.0f}, // コマ1 (cell_x = 0)
    {16.0f, 0.0f, 8.0f, 8.0f} // コマ2 (cell_x = 1)

};

// 2. カニ（中段：11x8px）
static const Rectangle REC_ALIEN_CRAB[2] = {
    {32.0f, 0.0f, 11.0f, 8.0f}, // コマ1 (cell_x = 2)
    {48.0f, 0.0f, 11.0f, 8.0f}  // コマ2 (cell_x = 3)
};

// 3. タコ（下段：12x8px）
static const Rectangle REC_ALIEN_OCTOPUS[2] = {
    {64.0f, 0.0f, 12.0f, 8.0f}, // コマ1 (cell_x = 4)
    {80.0f, 0.0f, 12.0f, 8.0f}  // コマ2 (cell_x = 5)
};

// 4. UFO（16x7px）
static const Rectangle REC_UFO = {96.0f, 0.0f, 16.0f, 7.0f}; // (cell_x = 6)

// 5. 自機（砲台：16x8px）
static const Rectangle REC_PLAYER = {112.0f, 0.0f, 16.0f, 8.0f}; // (cell_x = 7)

// -----------------------------------------------------------------------------
// 2行目 (cell_y = 1, Y = 8) : 弾 ＆ 相殺エフェクト
// -----------------------------------------------------------------------------

// 6. 自機弾（1x4px : ドット中央配置）
static const Rectangle REC_PLAYER_BULLET = {7.0f, 8.0f, 1.0f,
                                            4.0f}; // (cell_x = 0)

// 7. 敵弾 3種（各2コマ, 幅3px, 高さ6px）
static const Rectangle REC_ENEMY_BULLETS[3][2] = {
    // タイプ0 (ギザギザ弾)
    {
        {22.0f, 8.0f, 3.0f, 6.0f}, // (cell_x = 1)
        {38.0f, 8.0f, 3.0f, 6.0f}  // (cell_x = 2)
    },
    // タイプ1 (クロス弾)
    {
        {54.0f, 8.0f, 3.0f, 6.0f}, // (cell_x = 3)
        {70.0f, 8.0f, 3.0f, 6.0f}  // (cell_x = 4)
    },
    // タイプ2 (雷弾)
    {
        {86.0f, 8.0f, 3.0f, 6.0f}, // (cell_x = 5)
        {102.0f, 8.0f, 3.0f, 6.0f} // (cell_x = 6)
    }};

// 8. 弾の相殺エフェクト（6x8px）
static const Rectangle REC_BULLET_EXPLOSION = {116.0f, 8.0f, 6.0f,
                                               8.0f}; // (cell_x = 7)

// -----------------------------------------------------------------------------
// 3行目 (cell_y = 2, Y = 16) : 爆発演出 ＆ バンカー
// -----------------------------------------------------------------------------

// 9. インベーダー撃破爆破（13x7px）
static const Rectangle REC_ALIEN_EXPLOSION = {2.0f, 16.0f, 13.0f,
                                              7.0f}; // (cell_x = 0)

// 10. 自機被弾爆発（2コマ：16x8px）
static const Rectangle REC_PLAYER_EXPLOSION[2] = {
    {16.0f, 16.0f, 16.0f, 8.0f}, // コマ1 (cell_x = 1)
    {32.0f, 16.0f, 16.0f, 8.0f}  // コマ2 (cell_x = 2)
};

// 11. バンカー（防衛砦：22x16px）
static const Rectangle REC_BUNKER = {49.0f, 16.0f, 22.0f,
                                     16.0f}; // (cell_x = 3)

#endif // SPRITESHEET_RECTS_H
