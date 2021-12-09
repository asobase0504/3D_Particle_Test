//=========================================
// 
// 壁の作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _WALL_H_
#define _WALL_H_


//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitWall(void);			// 壁の初期化処理
void UninitWall(void);			// 壁の終了処理
void UpdateWall(void);			// 壁の更新処理
void DrawWall(void);			// 壁の描画処理
void SetWall(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// 壁の設定処理
D3DXVECTOR3* GetWallPos(void);	// 壁の取得処理

#endif // !_WALL_H_
