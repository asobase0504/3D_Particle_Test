//=========================================
// 
// ビルボードの作成
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "setup.h"
#include "billboard.h"

//------------------------------------
// ビルボードの種類の列挙型
//------------------------------------
#define BIILBOARD_MAX	(4)	// ビルボードの最大数

//------------------------------------
// ビルボードの構造体を定義
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 頂点座標
	D3DXVECTOR3 rot;		// 回転座標
	bool bUse;				// 使用状況
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
} Billboard;

//------------------------------------
// 静的変数
//------------------------------------
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};		// 頂点バッファーへのポインタ
static LPDIRECT3DTEXTURE9 s_pTexture = {};			// テクスチャへのポインタ
static Billboard s_abillboard[BIILBOARD_MAX];			// ビルボードの構造体

//=========================================
// 初期化
//=========================================
void InitBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	ZeroMemory(s_abillboard, sizeof(s_abillboard));

	// テクスチャの読込
	D3DXCreateTextureFromFile(pDevice,
		"data/TEXTURE/player.png",
		&s_pTexture);

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * BIILBOARD_MAX,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	VERTEX_3D* pVtx = NULL;

	// 頂点座標をロック
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for (int i = 0; i < BIILBOARD_MAX; i++)
	{
		// 頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(-10.0f, 10.0f, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(10.0f, 10.0f, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(-10.0f, 0.0f, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(10.0f, 0.0f, 0.0f);

		// 各頂点の法線の設定(※ベクトルの大きさは1にする必要がある)
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// 頂点カラーの設定
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// テクスチャ座標の設定
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;
	}
	// 頂点座標をアンロック
	s_pVtxBuff->Unlock();

}

//=========================================
// 終了
//=========================================
void UninitBillboard(void)
{
	if (s_pTexture != NULL)
	{
		s_pTexture->Release();
		s_pTexture = NULL;
	}

	// 頂点バッファーの破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();
		s_pVtxBuff = NULL;
	}
}

//=========================================
// 更新
//=========================================
void UpdateBillboard(void)
{

}

//=========================================
// 描画
//=========================================
void DrawBillboard(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	for (int i = 0; i < BIILBOARD_MAX; i++)
	{
		if (!(s_abillboard[i].bUse))
		{
			continue;
		}
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&s_abillboard[i].mtxWorld);	// 行列初期化関数(第1引数の行列を単位行列に初期化)
		D3DXMATRIX mtxView;
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// カメラの逆行列を設定
		s_abillboard->mtxWorld._11 = mtxView._11;
		s_abillboard->mtxWorld._12 = mtxView._21;
		s_abillboard->mtxWorld._13 = mtxView._31;
		s_abillboard->mtxWorld._21 = mtxView._12;
		s_abillboard->mtxWorld._22 = mtxView._22;
		s_abillboard->mtxWorld._23 = mtxView._32;
		s_abillboard->mtxWorld._31 = mtxView._13;
		s_abillboard->mtxWorld._32 = mtxView._23;
		s_abillboard->mtxWorld._33 = mtxView._33;

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, s_abillboard[i].pos.x, s_abillboard[i].pos.y, s_abillboard[i].pos.z);			// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
		D3DXMatrixMultiply(&s_abillboard[i].mtxWorld, &s_abillboard[i].mtxWorld, &mtxTrans);					// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &s_abillboard[i].mtxWorld);	// ワールド座標行列の設定

		// 頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// ライトを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, false);

		// 描画
		RectDraw(pDevice, s_pTexture, i * 4);

		// ライトを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, true);

		// テクスチャの解除
		pDevice->SetTexture(0, NULL);

	}
}

void SetBillboard(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	for (int i = 0; i < BIILBOARD_MAX; i++)
	{
		if (s_abillboard[i].bUse)
		{	// 既に使用している場合
			continue;
		}

		s_abillboard[i].pos = pos;
		s_abillboard[i].rot = rot;
		s_abillboard[i].bUse = true;

		break;
	}

}

D3DXVECTOR3* GetBillboardPos(void)
{
	return &(s_abillboard[0].pos);
}