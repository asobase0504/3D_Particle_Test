//=========================================
// 
// 処理の設定
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
// aブレンドの通常設定
//=========================================
LPDIRECT3DDEVICE9 NoneAlphaBlend(LPDIRECT3DDEVICE9 pDevice)
{
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	return pDevice;
}

//=========================================
// aブレンドの加算合成
//=========================================
LPDIRECT3DDEVICE9 AddAlphaBlend(LPDIRECT3DDEVICE9 pDevice)
{
	// aブレンディングを加算合成に設定
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	return pDevice;
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


