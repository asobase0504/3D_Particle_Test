//=========================================
// 
// ポリゴンの作成(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _SETUP_H_
#define _SETUP_H_

//------------------------------------
// プロトタイプ宣言
//------------------------------------
float NormalizeRot(float rot);
void RectDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// 通常の描写
void RectAddDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);	// 加算処理の描写

#endif // !_SETUP_H_
