//**************************************************************************************************
//
// エフェクトの設定処理
// Auther：唐﨑結斗
// Auther：Yuda Kaito
//
//**************************************************************************************************
//*******************************************************************************
// include
//*******************************************************************************
#include "effect.h"
#include "spawn_effect.h"
#include "LoadEffect.h"
#include "input.h"
#include "setup.h"
#include <assert.h>
#include <time.h>

//*******************************************************************************
// マクロ定義
//*******************************************************************************
#define MAX_EFFECT	(4096)		// エフェクトの最大数

//*******************************************************************************
// プロトタイプ宣言
//*******************************************************************************

//-----------------------
// エフェクト処理
//-----------------------
void StatChange(Effect* effect);			// 数値の増減処理
void AddStat(SFluctFloat* fluct);			// 数値の加算処理
void MoveEffect(Effect* effect);			// 移動処理
bool DiedCriteriaEffect(Effect* effect);	// 死亡条件処理
void DiedEffect(Effect* effect);			// 死亡時処理
void OffScreenEffect(Effect* effect);		// 画面外処理

//-----------------------
// ロード関係
//-----------------------
void LoadFluct(FILE** file ,SFluctFloat* fluct);
void LoadFluct(FILE* file, SFluctInt* fluct);

//-----------------------
// 乱数データ処理
//-----------------------
void InitRandom(FRandFloat* inRandomFloat, float fValue);	// 乱数の初期化

//*******************************************************************************
// 静的変数
//*******************************************************************************
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_EFFECT_TYPE_TEX] = {};		// テクスチャへのポインタ
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};						// 頂点バッファへのポインタ
static Effect s_aEffect[MAX_EFFECT];								// エフェクトの情報

//-----------------------
// テクスチャ名
//-----------------------
static const char* pTexturename[] =
{
	NULL,							// テクスチャなし
	"data/TEXTURE/effect000.jpg",	// 丸
	"data/TEXTURE/effect100.jpg",	// 線の丸
	"data/TEXTURE/effect101.jpg",	// 二重丸
	"data/TEXTURE/effect102.jpg",	// 細い四角
	"data/TEXTURE/effect103.jpg",	// 三角
};
// pTexture と EFFECT_TYPE_TEX の数をチェック
static_assert((sizeof(pTexturename) / sizeof(pTexturename[0])) == MAX_EFFECT_TYPE_TEX, "error テクスチャの数と列挙型の数が一致してません");

//********************************************************************************
// エフェクトの初期化処理
//********************************************************************************
void InitEffect(void)
{
	srand((unsigned int)time(0));	// 乱数の初期化

	// デバイスへのポインタ
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// デバイスの取得

	// ポリゴンに張り付けるテクスチャの読み込み
	for (int i = 0; i < MAX_EFFECT_TYPE_TEX; i++)
	{
		D3DXCreateTextureFromFile(pDevice
			,pTexturename[i]
			,&s_pTexture[i]);
	}

	// エフェクトの情報を初期化
	ZeroMemory(s_aEffect, sizeof(s_aEffect));

	Effect* pEffect;	// エフェクトのポインタ型

	// 頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,		// 確保するバッファサイズ
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	// 頂点情報へのポインタを生成
	VERTEX_3D *pVtx;

	// 頂点バッファをロックし、頂点情報へのポインタを取得
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点バッファーの設定
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		pEffect = &(s_aEffect[i]);

		// 頂点座標の設定
		pVtx[0].pos.x = sinf(pEffect->rot.z + (D3DX_PI + pEffect->fAngele)) * pEffect->fLength;
		pVtx[0].pos.y = cosf(pEffect->rot.z + (D3DX_PI + pEffect->fAngele)) * pEffect->fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = sinf(pEffect->rot.z + (D3DX_PI - pEffect->fAngele)) * pEffect->fLength;
		pVtx[1].pos.y = cosf(pEffect->rot.z + (D3DX_PI - pEffect->fAngele)) * pEffect->fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = sinf(pEffect->rot.z - (0 + pEffect->fAngele)) * pEffect->fLength;
		pVtx[2].pos.y = cosf(pEffect->rot.z - (0 + pEffect->fAngele)) * pEffect->fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = sinf(pEffect->rot.z - (0 - pEffect->fAngele)) * pEffect->fLength;
		pVtx[3].pos.y = cosf(pEffect->rot.z - (0 - pEffect->fAngele)) * pEffect->fLength;
		pVtx[3].pos.z = 0.0f;

		// 頂点カラーの設定
		pVtx[0].col = pEffect->col;
		pVtx[1].col = pEffect->col;
		pVtx[2].col = pEffect->col;
		pVtx[3].col = pEffect->col;

		//各頂点の法線の設定　※　ベクトルの大きさは1にする必要がある
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// テクスチャ座標
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	// 頂点データのポインタを4つ分進める
	}

	// 頂点バッファをアンロック
	s_pVtxBuff->Unlock();
}

//********************************************************************************
// エフェクトの終了処理
//********************************************************************************
void UninitEffect(void)
{
	// テクスチャの破棄	 
	for (int i = 0; i < MAX_EFFECT_TYPE_TEX; i++)
	{
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();

			s_pTexture[i] = NULL;
		}
	}

	// 頂点バッファを破棄
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();

		s_pVtxBuff = NULL;
	}
}

//********************************************************************************
// エフェクトの更新処理
//********************************************************************************
void UpdateEffect(void)
{
	Effect* pEffect;
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		pEffect = &(s_aEffect[i]);

		// エフェクトが使用されていないなら繰り返しを一回スキップ
		if (!(pEffect->bUse))
		{
			continue;
		}

		StatChange(pEffect);	// 各種パラメータの設定

		MoveEffect(pEffect);	// 移動の更新

		pEffect->speedY.fValue -= pEffect->fGravity;	// 重力

		
		pEffect->pos += pEffect->move;	// 位置の更新

		// 対角線の長さを算出する
		pEffect->fLength = sqrtf(((pEffect->width.fValue * pEffect->width.fValue) + (pEffect->height.fValue * pEffect->height.fValue)) / 2.0f);
		// 対角線の角度を算出
		pEffect->fAngele = atan2f(pEffect->width.fValue, pEffect->height.fValue);
		
		// 頂点バッファーの設定
		VERTEX_3D *pVtx;	// 頂点情報へのポインタを生成

		// 頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * i;

		// 頂点座標の設定
		pVtx[0].pos.x = sinf(pEffect->rot.z + (D3DX_PI + pEffect->fAngele)) * pEffect->fLength;
		pVtx[0].pos.y = cosf(pEffect->rot.z + (D3DX_PI + pEffect->fAngele)) * pEffect->fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = sinf(pEffect->rot.z + (D3DX_PI - pEffect->fAngele)) * pEffect->fLength;
		pVtx[1].pos.y = cosf(pEffect->rot.z + (D3DX_PI - pEffect->fAngele)) * pEffect->fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = sinf(pEffect->rot.z + -pEffect->fAngele) * pEffect->fLength;
		pVtx[2].pos.y = cosf(pEffect->rot.z + -pEffect->fAngele) * pEffect->fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = sinf(pEffect->rot.z + pEffect->fAngele) * pEffect->fLength;
		pVtx[3].pos.y = cosf(pEffect->rot.z + pEffect->fAngele) * pEffect->fLength;
		pVtx[3].pos.z = 0.0f;

		// 頂点カラーの設定
		pVtx[0].col = pEffect->col;
		pVtx[1].col = pEffect->col;
		pVtx[2].col = pEffect->col;
		pVtx[3].col = pEffect->col;

		//テクスチャ設定
		pEffect->nCntAnim++;
		if ((pEffect->nCntAnim % pEffect->nDivisionMAX) == 0)
		{
			pEffect->nPatternAnim = (pEffect->nPatternAnim + 1) % pEffect->nDivisionMAX;

			float fLeft = 1.0f / pEffect->nDivisionU * (pEffect->nPatternAnim % pEffect->nDivisionU);
			float fRight = 1.0f / pEffect->nDivisionU *(pEffect->nPatternAnim % pEffect->nDivisionU) + 1.0f / pEffect->nDivisionU;
			float fUp = 1.0f / pEffect->nDivisionV * (pEffect->nPatternAnim / pEffect->nDivisionU);
			float fDown = 1.0f / pEffect->nDivisionV * (pEffect->nPatternAnim / pEffect->nDivisionU + 1.0f / pEffect->nDivisionV * pEffect->nDivisionV);

			pVtx[0].tex = D3DXVECTOR2(fLeft, fUp);
			pVtx[1].tex = D3DXVECTOR2(fRight, fUp);
			pVtx[2].tex = D3DXVECTOR2(fLeft, fDown);
			pVtx[3].tex = D3DXVECTOR2(fRight, fDown);
		}

		// 頂点バッファをアンロック
		s_pVtxBuff->Unlock();

		// 死亡条件を満たしているかチェック
		if (!(DiedCriteriaEffect(pEffect)))
		{
			// 死亡処理
			DiedEffect(pEffect);
		}

		// window画面以外に出た場合の処理
		OffScreenEffect(pEffect);
	}
}

//********************************************************************************
// ステータスの変更処理
//********************************************************************************
void StatChange(Effect * effect)
{
	// 対角線の長さを算出する
	effect->fLength = sqrtf(((effect->width.fValue * effect->width.fValue) + (effect->height.fValue * effect->height.fValue)) / 2.0f);
	// 対角線の角度を算出
	effect->fAngele = atan2f(effect->width.fValue, effect->height.fValue);

	effect->randLife.nValue--;	// 寿命

	AddStat(&(effect->rotZ));
	AddStat(&(effect->width));
	AddStat(&(effect->height));
	AddStat(&(effect->colR));
	AddStat(&(effect->colG));
	AddStat(&(effect->colB));
	AddStat(&(effect->colA));

	// 色の設定
	effect->col = D3DXCOLOR(effect->colR.fValue, effect->colG.fValue, effect->colB.fValue, effect->colA.fValue);
}

//********************************************************************************
// ステータスの加算処理
//********************************************************************************
void AddStat(SFluctFloat* fluct)
{
	if (fluct->Add.bIsRandom)
	{
		fluct->fValue += fluct->fAddValue;
	}
}

//********************************************************************************
// 移動処理
//********************************************************************************
void MoveEffect(Effect * effect)
{
	// 移動の決定
	effect->move.x = sinf((D3DX_PI * effect->shotAngleZ.fValue) + effect->rot.z) * effect->speedX.fValue;
	effect->move.y = tanf((D3DX_PI * effect->shotAngleZ.fValue) + effect->rot.z) * effect->speedY.fValue;
	effect->move.z = cosf((D3DX_PI * effect->shotAngleZ.fValue) + effect->rot.z) * effect->speedZ.fValue;

	//for (int nHeight = 0; nHeight <= s_aMesh[0].nSurfaceHeight; nHeight++)
	//{
	//	float fRotHeight = D3DX_PI / s_aMesh[0].nSurfaceHeight;
	//	float fHeight = cosf(fRotHeight * nHeight) * s_aMesh[0].fLineHeight;
	//	for (int nWidth = 0; nWidth <= s_aMesh[0].nSurfaceWidth; nWidth++)
	//	{
	//		float fRotWidth = 2.0f * D3DX_PI / s_aMesh[0].nSurfaceWidth * nWidth;
	//		NormalizeRot(fRotWidth);

	//		effect->move.x = cosf(fRotWidth) * sinf(fRotHeight) * effect->speedX.fValue;
	//		effect->move.y = cosf(fRotHeight) * effect->speedX.fValue;
	//		effect->move.z = sinf(fRotWidth) * sinf(fRotHeight) * effect->speedX.fValue;

	//		effect->move.x = -sinf((D3DX_PI * nCntParticle / 10) - (s_aParticle[nCntParticle].fAngle / (5 * s_aParticle[nCntParticle].fAttenuation)));
	//		effect->move.y -= tanf(D3DX_PI) * s_aParticle[nCntParticle].fRadius / 2;
	//		effect->move.z = -cosf((D3DX_PI * nCntParticle / 10) - (s_aParticle[nCntParticle].fAngle / (5 * s_aParticle[nCntParticle].fAttenuation)));
	//	}
	//}

	if (effect->Injection == PARTICLMODE_CONVERGENCE)	// 収縮
	{
		float fRotMove, fRotDest, fRotDiff;
		D3DXVECTOR3 PosDifference = D3DXVECTOR3(effect->spaen_pos.x - effect->pos.x, effect->spaen_pos.y - effect->pos.y, 0.0f);

		fRotMove = atan2f(effect->move.x, effect->move.y);		// 現在の移動方向(角度)
		fRotDest = atan2f(PosDifference.x, PosDifference.y);	// 目的の移動方向
		fRotDiff = fRotDest - fRotMove;							// 目的の移動方向までの差分

		// 角度の正規化
		if (fRotDiff > D3DX_PI)
		{// 目的の差分が半円周(+方向)を超える場合
			fRotDiff -= (D3DX_PI * 2);
		}
		else if (fRotDiff < -D3DX_PI)
		{// 目的の差分が半円周(-方向)を超える場合
			fRotDiff += (D3DX_PI * 2);
		}

		fRotMove += fRotDiff * 1.0f;	// 移動方向(角度)の補正

		// 角度の正規化
		if (fRotMove > D3DX_PI)
		{// 移動方向(角度)が半円周(+方向)を超える場合
			fRotMove -= (D3DX_PI * 2);
		}
		else if (fRotMove < -D3DX_PI)
		{// 移動方向(角度)が半円周(-方向)を超える場合
			fRotMove += (D3DX_PI * 2);
		}

		// 位置の更新
		effect->move.x = sinf(fRotMove) * effect->speedX.fValue;
		effect->move.y = cosf(fRotMove) * effect->speedX.fValue;
	}
	if (effect->Injection == PARTICLMODE_ROTATION)	// ローテーション
	{
		effect->rot.z += D3DXToRadian(1);

		// 移動量の算出
		effect->move.x = sinf(effect->rot.z) * effect->speedX.fValue;
		effect->move.y = cosf(effect->rot.z) * effect->speedX.fValue;
	}

}

//********************************************************************************
// 死亡条件処理
//********************************************************************************
bool DiedCriteriaEffect(Effect* effect)
{
	if (effect->randLife.nValue <= 0)
	{// 寿命が0の時
		effect->bUse = false;	// 使用してない
	}
	return effect->bUse;
}

//********************************************************************************
// エフェクトの死亡処理
//********************************************************************************
void DiedEffect(Effect * effect)
{

}

//********************************************************************************
// エフェクトの描画処理
//********************************************************************************
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// 計算用マトリックス

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		// パーティクル情報の取得
		Effect *pEffect = &(s_aEffect[i]);
		if (!(pEffect->bUse))
		{
			continue;
		}
		// ワールドマトリックスの初期化
		D3DXMatrixIdentity(&pEffect->mtxWorld);	// 行列初期化関数(第1引数の行列を単位行列に初期化)
		D3DXMATRIX mtxView;
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// カメラの逆行列を設定
		if (pEffect->bIsBillboard)
		{
			pEffect->mtxWorld._11 = mtxView._11;
			pEffect->mtxWorld._12 = mtxView._21;
			pEffect->mtxWorld._13 = mtxView._31;
			pEffect->mtxWorld._21 = mtxView._12;
			pEffect->mtxWorld._22 = mtxView._22;
			pEffect->mtxWorld._23 = mtxView._32;
			pEffect->mtxWorld._31 = mtxView._13;
			pEffect->mtxWorld._32 = mtxView._23;
			pEffect->mtxWorld._33 = mtxView._33;
		}

		// 位置を反映
		D3DXMatrixTranslation(&mtxTrans, pEffect->pos.x, pEffect->pos.y, pEffect->pos.z);		// 行列移動関数(第１引数にX,Y,Z方向の移動行列を作成)
		D3DXMatrixMultiply(&pEffect->mtxWorld, &pEffect->mtxWorld, &mtxTrans);					// 行列掛け算関数(第2引数×第3引数第を１引数に格納)

		// ワールドマトリックスの設定
		pDevice->SetTransform(D3DTS_WORLD, &pEffect->mtxWorld);	// ワールド座標行列の設定

		// 頂点バッファをデバイスのデータストリームに設定
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

		// 頂点フォーマットの設定
		pDevice->SetFVF(FVF_VERTEX_3D);

		// ライトを無効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, false);

		// Zバッファの上書きを無効にする
		pDevice->SetRenderState(D3DRS_ZENABLE, false);

		// カリングの設定
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		
		// アルファテストを有効
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		if (pEffect->blend == BLENDMODE_ADDITION)
		{
			// αブレンディングを加算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// ブレンドオペレーション(計算方式)
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
		else if (pEffect->blend == BLENDMODE_SUBTRACT)
		{
			// αブレンディングを減算合成に設定
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}

		// 描画
		RectDraw(pDevice, s_pTexture[pEffect->tex], i * 4);

		// ライトを有効にする
		pDevice->SetRenderState(D3DRS_LIGHTING, true);

		// Zバッファの上書きを無効にする
		pDevice->SetRenderState(D3DRS_ZENABLE, true);

		// カリングの設定
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// アルファテストを有効
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

		// テクスチャの解除
		pDevice->SetTexture(0, NULL);

	}
	// αブレンディングを元に戻す
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//********************************************************************************
// エフェクトの設定処理
//********************************************************************************
void SetEffect(D3DXVECTOR3 pos,EFFECT_TYPE type)
{
	// 変数宣言
	Effect* pEffect;	// グローバル変数をポインタ型に変換用

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		pEffect = &(s_aEffect[i]);	// ポインタ型に変換

		if (pEffect->bUse)
		{
			continue;
		}

#ifndef TEST_MODE	// テスト環境か否か
		// ファイルの読み込み
		s_aEffect[i] = LoadEffect(type);
#else	// エフェクトテスト時のみ有効
		*pEffect = *GetParticle();
#endif																	

		pEffect->pos = pos;			// 位置
		pEffect->spaen_pos = pos;	// スポーン位置

		SetRandom(&pEffect->colR.initial, &pEffect->colR.fValue);				// Redの出現時の数値
		SetRandom(&pEffect->colG.initial, &pEffect->colG.fValue);				// Greenの出現時の数値
		SetRandom(&pEffect->colB.initial, &pEffect->colB.fValue);				// Blueの出現時の数値
		SetRandom(&pEffect->colA.initial, &pEffect->colA.fValue);				// Alphaの出現時の数値
		SetRandom(&pEffect->speedX.initial, &(pEffect->speedX.fValue));			// 移動量(X軸)
		SetRandom(&pEffect->speedY.initial, &(pEffect->speedY.fValue));			// 移動量(Y軸)
		SetRandom(&pEffect->speedZ.initial, &(pEffect->speedZ.fValue));			// 移動量(Z軸)
		SetRandom(&pEffect->randLife.initial, &(pEffect->randLife.nValue));		// 寿命
		SetRandom(&pEffect->width.initial, &(pEffect->width.fValue));			// 幅
		SetRandom(&pEffect->height.initial, &(pEffect->height.fValue));			// 高さ
		SetRandom(&pEffect->shotAngleX.initial, &(pEffect->shotAngleX.fValue));	// 発射角度(X軸)
		SetRandom(&pEffect->shotAngleY.initial, &(pEffect->shotAngleY.fValue));	// 発射角度(Y軸)
		SetRandom(&pEffect->shotAngleZ.initial, &(pEffect->shotAngleZ.fValue));	// 発射角度(Z軸)

		SetRandom(&pEffect->colR.Add, &pEffect->colR.fAddValue);				// Redの出現時の数値
		SetRandom(&pEffect->colG.Add, &pEffect->colG.fAddValue);				// Greenの出現時の数値
		SetRandom(&pEffect->colB.Add, &pEffect->colB.fAddValue);				// Blueの出現時の数値
		SetRandom(&pEffect->colA.Add, &pEffect->colA.fAddValue);				// Alphaの出現時の数値
		SetRandom(&pEffect->speedX.Add, &(pEffect->speedX.fAddValue));			// 移動量(X軸)
		SetRandom(&pEffect->speedY.Add, &(pEffect->speedY.fAddValue));			// 移動量(Y軸)
		SetRandom(&pEffect->speedZ.Add, &(pEffect->speedZ.fAddValue));			// 移動量(Z軸)
		SetRandom(&pEffect->width.Add, &(pEffect->width.fAddValue));			// 幅
		SetRandom(&pEffect->height.Add, &(pEffect->height.fAddValue));			// 高さ
		SetRandom(&pEffect->randLife.Add, &(pEffect->randLife.nValue));			// 寿命
		SetRandom(&pEffect->shotAngleX.Add, &(pEffect->shotAngleX.fAddValue));	// 発射角度(X軸)
		SetRandom(&pEffect->shotAngleY.Add, &(pEffect->shotAngleY.fAddValue));	// 発射角度(Y軸)
		SetRandom(&pEffect->shotAngleZ.Add, &(pEffect->shotAngleZ.fAddValue));	// 発射角度(Z軸)

		pEffect->nDivisionU = 1;
		pEffect->nDivisionV = 1;

		pEffect->nDivisionMAX = pEffect->nDivisionU * pEffect->nDivisionV;	// 分割数の計算

		// 角度の代入
		pEffect->shotAngle.x = pEffect->shotAngleX.fValue;
		pEffect->shotAngle.y = pEffect->shotAngleY.fValue;
		pEffect->shotAngle.z = pEffect->shotAngleZ.fValue;

		// 色の代入
		pEffect->col.r = pEffect->colR.fValue;
		pEffect->col.g = pEffect->colG.fValue;
		pEffect->col.b = pEffect->colB.fValue;
		pEffect->col.a = pEffect->colA.fValue;

		// 対角線の長さを算出する
		pEffect->fLength = sqrtf(((pEffect->width.fValue * pEffect->width.fValue) + (pEffect->height.fValue * pEffect->height.fValue)) / 2.0f);
		// 対角線の角度を算出
		pEffect->fAngele = atan2f(pEffect->width.fValue, pEffect->height.fValue);

		// 頂点情報へのポインタを生成
		VERTEX_3D *pVtx;
		// 頂点バッファをロックし、頂点情報へのポインタを取得
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * i;	// 頂点データのポインタを4つ分進める
		
		// 頂点座標の設定
		pVtx[0].pos.x = sinf(pEffect->rot.z + (D3DX_PI + pEffect->fAngele)) * pEffect->fLength;
		pVtx[0].pos.y = cosf(pEffect->rot.z + (D3DX_PI + pEffect->fAngele)) * pEffect->fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = sinf(pEffect->rot.z + (D3DX_PI - pEffect->fAngele)) * pEffect->fLength;
		pVtx[1].pos.y = cosf(pEffect->rot.z + (D3DX_PI - pEffect->fAngele)) * pEffect->fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = sinf(pEffect->rot.z + -pEffect->fAngele) * pEffect->fLength;
		pVtx[2].pos.y = cosf(pEffect->rot.z + -pEffect->fAngele) * pEffect->fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = sinf(pEffect->rot.z + pEffect->fAngele) * pEffect->fLength;
		pVtx[3].pos.y = cosf(pEffect->rot.z + pEffect->fAngele) * pEffect->fLength;
		pVtx[3].pos.z = 0.0f;

		// 頂点カラーの設定
		pVtx[0].col = pEffect->col;
		pVtx[1].col = pEffect->col;
		pVtx[2].col = pEffect->col;
		pVtx[3].col = pEffect->col;

		// 頂点バッファをアンロック
		s_pVtxBuff->Unlock();

		pEffect->bUse = true;				// 使用してる状態
		break;
	}
}

//********************************************************************************
// エフェクトの画面外処理
//********************************************************************************
void OffScreenEffect(Effect* effect)
{
	//// window画面以外に出た場合の処理
	//if (effect->pos.y + effect->randRadius.fValue >= SCREEN_HEIGHT)
	//{// Y座標の移動方向を反転
	//	effect->rot.z = D3DX_PI + effect->rot.z * -0.8f;
	//	effect->move.y *= -0.8f;
	//}
	//else if (effect->pos.x - effect->randRadius.fValue < 0 || effect->pos.x + effect->randRadius.fValue > SCREEN_WIDH)
	//{// X座標の移動方向を反転
	//	effect->rot.z *= -1.0f;
	//	effect->move.x *= -1.0f;
	//}
}

//********************************************************************************
// SFluctFloatの値の読み込み
//********************************************************************************
void LoadFluct(FILE** file, SFluctFloat* fluct)
{
}

//********************************************************************************
// SFluctIntの値の読み込み
//********************************************************************************
void LoadFluct(FILE* file, SFluctInt* fluct)
{
}

//********************************************************************************
// SRandFloatの初期化
//********************************************************************************
void InitRandom(FRandFloat* inRandomFloat, float fValue)
{
	inRandomFloat->bIsRandom = false;
}

//********************************************************************************
// SRandFloatの乱数結果を反映
//********************************************************************************
void SetRandom(FRandFloat* fluct,float* fValue)
{
	if (fluct->bIsRandom)	// 乱数が使用されている
	{
		*fValue = (rand() / (float)RAND_MAX) * (fluct->fMax - fluct->fMin) + fluct->fMin;
	}
}

//********************************************************************************
// SRandIntの乱数結果を反映
//********************************************************************************
void SetRandom(FRandInt* fluct, int* nValue)
{
	if (fluct->bIsRandom)	// 乱数が使用されている
	{
		*nValue = (rand() / RAND_MAX) * (fluct->nMax - fluct->nMin) + fluct->nMin;
	}
}