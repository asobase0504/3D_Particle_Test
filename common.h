//=========================================
// 
// 共通処理(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _COMMON_H_
#define _COMMON_H_

//------------------------------------
// プロトタイプ宣言
//------------------------------------
// 描画系統の処理
void RectDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);		// 通常の描写
void AddSynthesis(LPDIRECT3DDEVICE9 pDevice);	// 加算合成描写

// 計算系統の処理
void DiagonalLine(float* Length, float* Angele, float width, float height);			// 対角線の計算
void VtxCol(VERTEX_3D *pVtx,D3DXCOLOR col);			// 頂点カラーの設定
void NormalizeRot(float* rot);						// 角度の正規化
void TexAnim(VERTEX_3D *vtx, int U,int V,int Pattern);				// テクスチャアニメーションの処理。

// 頂点バッファーの処理
void RectVtxPos_XY(VERTEX_3D *pVtx, D3DXVECTOR3 rot,float angele,float length);		// X軸とY軸でメッシュの作成
void RectVtxPos_XZ(VERTEX_3D *pVtx, D3DXVECTOR3 rot,float angele,float length);		// X軸とZ軸でメッシュの作成
void RectVtxPos_YZ(VERTEX_3D *pVtx, D3DXVECTOR3 rot,float angele,float length);		// Y軸とZ軸でメッシュの作成

// カメラ系統の処理

#endif // !_COMMON_H_
