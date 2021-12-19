//=========================================
// 
// 共通処理の設定
// プロジェクト全体で使用する処理
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "common.h"

//=========================================
// 角度の正規化
//=========================================
void NormalizeRot(float* rot)
{
	if (*rot > D3DX_PI)
	{
		*rot -= D3DX_PI * 2;
	}
	if (*rot < -D3DX_PI)
	{
		*rot += D3DX_PI * 2;
	}
}

//=========================================
// テクスチャアニメーション処理
//=========================================
void TexAnim(VERTEX_3D *vtx,int U, int V, int Pattern)
{

	float fLeft = 1.0f / U * (Pattern % U);
	float fRight = 1.0f / U *(Pattern % U) + 1.0f / U;
	float fUp = 1.0f / V * (Pattern / U);
	float fDown = 1.0f / V * (Pattern / U + 1.0f / V * V);

	vtx[0].tex = D3DXVECTOR2(fLeft, fUp);
	vtx[1].tex = D3DXVECTOR2(fRight, fUp);
	vtx[2].tex = D3DXVECTOR2(fLeft, fDown);
	vtx[3].tex = D3DXVECTOR2(fRight, fDown);
}

//=========================================
// 加算合成なしの描写(長方形)
//=========================================
void RectDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt)
{
	// テクスチャの設定
	pDevice->SetTexture(0, Texture);

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt, 2);
}

//=========================================
// 加算合成有りの描写(長方形)
//=========================================
void AddSynthesis(LPDIRECT3DDEVICE9 pDevice)
{
	// aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

//=========================================
// 対角線の値を算出(長方形)
//=========================================
void DiagonalLine(float* Length, float* Angele, float width, float height)
{
	// 対角線の長さを算出する
	*Length = sqrtf((width * width + height * height) / 2.0f);
	// 対角線の角度を算出
	*Angele = atan2f(width, height);
}