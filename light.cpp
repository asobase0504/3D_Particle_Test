//=========================================
// 
// ライトの作成
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "light.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define LIGTH_MAX	(5)

//------------------------------------
// 静的変数
//------------------------------------
static D3DLIGHT9 s_light[LIGTH_MAX];	//ライト情報

//=========================================
// 初期化
//=========================================
void InitLight(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXVECTOR3 vecDir;	// ライトの方向ベクトル

	// ライトをクリアする
	ZeroMemory(&s_light[0], sizeof(D3DLIGHT9));	// 構造体変数をゼロクリアできる関数

	// ライトの種類を設定
	s_light[0].Type = D3DLIGHT_DIRECTIONAL;		// 平行光源

	// ライトの拡散光を設定
	s_light[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);	// ライトの色

	// ライトの方向を設定
	vecDir = D3DXVECTOR3(0.2f, -0.8f, -0.4f);
	D3DXVec3Normalize(&vecDir, &vecDir);	// 正規化する(大きさ１のベクトルにする)
	s_light[0].Direction = vecDir;

	// ライトを設定する
	pDevice->SetLight(0, &s_light[0]);

	// ライトを有効にする
	pDevice->LightEnable(0, true);

	// ライトの種類を設定
	s_light[1].Type = D3DLIGHT_DIRECTIONAL;		// 平行光源

	// ライトの拡散光を設定
	s_light[1].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);	// ライトの色

	// ライトの方向を設定
	vecDir = D3DXVECTOR3(0.2f, -0.8f, 0.4f);
	D3DXVec3Normalize(&vecDir, &vecDir);	// 正規化する(大きさ１のベクトルにする)
	s_light[0].Direction = vecDir;

	// ライトを設定する
	pDevice->SetLight(0, &s_light[0]);

	// ライトを有効にする
	pDevice->LightEnable(0, true);
}

//=========================================
// 終了
//=========================================
void UninitLight(void)
{
}

//=========================================
// 更新
//=========================================
void UpdateLight(void)
{
}
