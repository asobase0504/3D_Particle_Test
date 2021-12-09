//=========================================
// 
// ビルボードの作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _BILLBOARD_H_
#define _BILLBOARD_H_


//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitBillboard(void);			// ビルボードのの初期化処理
void UninitBillboard(void);			// ビルボードのの終了処理
void UpdateBillboard(void);			// ビルボードのの更新処理
void DrawBillboard(void);			// ビルボードのの描画処理
void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR3 rot);	// ビルボードのの設定処理
D3DXVECTOR3* GetBillboardPos(void);	// ビルボードのの取得処理

#endif // !_BILLBOARD_H_
