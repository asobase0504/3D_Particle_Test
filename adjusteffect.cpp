//**************************************************************************************************
//
// エフェクトの調整設定処理
// Auther：Karasaki Yuuto
// Auther：Yuda Kaito
//
//**************************************************************************************************
//************************************************************
// include
//************************************************************
#include "main.h"
#include "adjusteffect.h"
#include "input.h"
#include "effect.h"
#include <time.h>

//************************************************************
// マクロ定義
//************************************************************
#define MAX_NUMBER_EFFECT	(10)		// エフェクトの最大数
#define MAX_SPEED_EFFECT	(200.0f)	// エフェクトの最大移動量
#define MIN_RADIUS_EFFECT	(0.0f)		// エフェクトの最小半径
#define MAX_RADIUS_EFFECT	(200.0f)	// エフェクトの最大半径
#define MIN_LIFE_EFFECT		(0)			// エフェクトの最小寿命
#define MAX_LIFE_EFFECT		(1200)		// エフェクトの最大寿命
#define MIN_COL_EFFECT		(0.0f)		// エフェクトの最小カラー
#define MAX_COL_EFFECT		(1.0f)		// エフェクトの最大カラー

//************************************************************
// プロトタイプ宣言
//************************************************************

//*******************************************************************************
// 静的変数
//*******************************************************************************
															
//********************************************************************************
// エフェクトの移動量調整処理
//********************************************************************************
void AdjustMoveEffect(Effect* pParticle)
{
	AdjustSFluctFloat(&(pParticle->randSpeed), 0.1f, MAX_SPEED_EFFECT, 0.0f);
}

//********************************************************************************
// 一度に発生するエフェクトの数調整処理
//********************************************************************************
void AdjustOneShotEffect(Effect* pParticle)
{
	int* nPopCnt = &(pParticle->nPopCnt);

	if (GetKeyboardTrigger(DIK_B))
	{// [J]キーが押されたとき
		if (*nPopCnt > 1)
		{
			*nPopCnt--;
		}
		else if (*nPopCnt <= 1)
		{
			*nPopCnt = 1;
		}
	}
	else if (GetKeyboardTrigger(DIK_N))
	{// [L]キーが押されたとき
		if (*nPopCnt < MAX_NUMBER_EFFECT)
		{
			*nPopCnt++;
		}
		else if (*nPopCnt >= 10)
		{
			*nPopCnt = 10;
		}
	}
}

//********************************************************************************
// エフェクトの半径調整処理
//********************************************************************************
void AdjustRadiusEffect(Effect* pParticle)
{
	AdjustSFluctFloat(&(pParticle->randRadius), 0.1f, MAX_RADIUS_EFFECT, MIN_RADIUS_EFFECT);
}

//********************************************************************************
// エフェクトの寿命調整処理
//********************************************************************************
void AdjustLifeEffect(Effect* pParticle)
{
	SFluctInt* Life = &(pParticle->randLife);

	// 通常時の値
	if (GetKeyboardPress(DIK_B))
	{ // 数値を小さくする
		if (Life->nValue > MIN_LIFE_EFFECT)
		{
			Life->nValue--;
		}
		else if (Life->nValue <= MIN_LIFE_EFFECT)
		{
			Life->nValue = MIN_LIFE_EFFECT;
		}
	}
	else if (GetKeyboardPress(DIK_N))
	{ // 数値を大きくする
		if (Life->nValue < MAX_LIFE_EFFECT)
		{
			Life->nValue++;
		}
		else if (Life->nValue >= MAX_LIFE_EFFECT)
		{
			Life->nValue = MAX_LIFE_EFFECT;
		}
	}

	// 出現時の乱数
	if (GetKeyboardTrigger(DIK_R))
	{ // 出現時に乱数を使用するか否か
		Life->initial.bIsRandom = !Life->initial.bIsRandom;
	}
	if (GetKeyboardPress(DIK_Y))
	{ // 乱数の最小値の数値を小さくする
		if (Life->initial.nMin > MIN_LIFE_EFFECT)
		{
			Life->initial.nMin--;
		}
		else if (Life->initial.nMin <= MIN_LIFE_EFFECT)
		{
			Life->initial.nMin = MIN_LIFE_EFFECT;
		}
	}
	else if (GetKeyboardPress(DIK_T))
	{ // 乱数の最小値の数値を大きくする
		if (Life->initial.nMin < Life->initial.nMax)
		{
			Life->initial.nMin++;
		}
		else if (Life->initial.nMin >= Life->initial.nMax)
		{
			Life->initial.nMin = Life->initial.nMax;
		}
	}

	if (GetKeyboardPress(DIK_U))
	{ // 乱数の最大値の数値を小さくする
		if (Life->initial.nMax > Life->Add.nMin)
		{
			Life->initial.nMax--;
		}
		else if (Life->initial.nMax <= Life->Add.nMin)
		{
			Life->initial.nMax = Life->Add.nMin;
		}
	}
	else if (GetKeyboardPress(DIK_I))
	{ // 乱数の最大値の数値を大きくする
		if (Life->initial.nMax < MAX_LIFE_EFFECT)
		{
			Life->initial.nMax++;
		}
		else if (Life->initial.nMax >= MAX_LIFE_EFFECT)
		{
			Life->initial.nMax = MAX_LIFE_EFFECT;
		}
	}

	// 加算時の乱数
	if (GetKeyboardTrigger(DIK_F))
	{ // 加算時に乱数を使用するか否か
		Life->Add.bIsRandom = !Life->Add.bIsRandom;
	}
	if (GetKeyboardPress(DIK_G))
	{ // 乱数の最小値の数値を小さくする
		if (Life->Add.nMin > MIN_LIFE_EFFECT)
		{
			Life->Add.nMin--;
		}
		else if (Life->Add.nMin <= MIN_LIFE_EFFECT)
		{
			Life->Add.nMin = MIN_LIFE_EFFECT;
		}
	}
	else if (GetKeyboardPress(DIK_H))
	{ // 乱数の最小値の数値を大きくする
		if (Life->Add.nMin < Life->Add.nMax)
		{
			Life->Add.nMin++;
		}
		else if (Life->Add.nMin <= Life->Add.nMax)
		{
			Life->Add.nMin = Life->Add.nMax;
		}
	}

	if (GetKeyboardPress(DIK_J))
	{ // 乱数の最大値の数値を小さくする
		if (Life->Add.nMax > Life->Add.nMin)
		{
			Life->Add.nMax--;
		}
		else if (Life->Add.nMax >= Life->Add.nMin)
		{
			Life->Add.nMax = Life->Add.nMin;
		}
	}
	else if (GetKeyboardPress(DIK_K))
	{ // 乱数の最大値の数値を大きくする
		if (Life->Add.nMax < MAX_LIFE_EFFECT)
		{
			Life->Add.nMax++;
		}
		else if (Life->Add.nMax >= MAX_LIFE_EFFECT)
		{
			Life->Add.nMax = MAX_LIFE_EFFECT;
		}
	}

}

//********************************************************************************
// エフェクトの重力調整処理
//********************************************************************************
void AdjustGravityEffect(Effect* pParticle)
{
	if (GetKeyboardPress(DIK_B))
	{// [B]キーが押されたとき
		if (pParticle->fGravity < 1.0f)
		{
			pParticle->fGravity += 0.001f;
		}
		else if (pParticle->fGravity > 1.0f)
		{
			pParticle->fGravity = 1.0f;
		}
	}
	else if (GetKeyboardPress(DIK_M))
	{// [M]キーが押されたとき
		if (pParticle->fGravity > 0.0f)
		{
			pParticle->fGravity -= 0.001f;
		}
		else if (pParticle->fGravity < 0.0f)
		{
			pParticle->fGravity = 0.0f;
		}
	}

}

//********************************************************************************
// エフェクトの角度調整処理
//********************************************************************************
void AdjustRotEffect(Effect* pParticle)
{
	if (GetKeyboardPress(DIK_E))
	{// [E]キーが押されたとき
		if (pParticle->rot.z > -D3DX_PI)
		{
			pParticle->rot.z -= 0.01f;
		}
		else if (pParticle->rot.z < -D3DX_PI)
		{
			pParticle->rot.z = -1.0f;
		}
	}
	else if (GetKeyboardPress(DIK_Q))
	{// [Q]キーが押されたとき
		if (pParticle->rot.z < D3DX_PI)
		{
			pParticle->rot.z += 0.01f;
		}
		else if (pParticle->rot.z > D3DX_PI)
		{
			pParticle->rot.z = D3DX_PI;
		}
	}
}

//********************************************************************************
// エフェクトの飛散角度調整処理
//********************************************************************************
void AdjustSpreadEffect(Effect* pParticle)
{
	if (GetKeyboardPress(DIK_Z))
	{// [Z]キーが押されたとき
		if (pParticle->randAangle.initial.fMax > D3DXToRadian(0))
		{
			pParticle->randAangle.initial.fMax -= D3DXToRadian(1);
		}
		if (pParticle->randAangle.initial.fMax <= D3DXToRadian(0))
		{
			pParticle->randAangle.initial.fMax = D3DXToRadian(0);
		}
	}
	else if (GetKeyboardPress(DIK_C))
	{// [C]キーが押されたとき
		if (pParticle->randAangle.initial.fMax <= D3DX_PI)
		{
			pParticle->randAangle.initial.fMax += D3DXToRadian(1);
		}
		if (pParticle->randAangle.initial.fMax >= D3DX_PI)
		{
			pParticle->randAangle.initial.fMax = D3DXToRadian(360);
		}
	}

}

//********************************************************************************
// SFluctFloatの値のキー入力処理
//********************************************************************************
void AdjustSFluctFloat(SFluctFloat* Fluct, float Add, float Max, float Min)
{
	// 通常時の値
	if (GetKeyboardPress(DIK_B))
	{ // 数値を小さくする
		if (Fluct->fValue > Min)
		{
			Fluct->fValue += -Add;
		}
		else if (Fluct->fValue <= Min)
		{
			Fluct->fValue = Min;
		}
	}
	else if (GetKeyboardPress(DIK_N))
	{ // 数値を大きくする
		if (Fluct->fValue < Max)
		{
			Fluct->fValue += Add;
		}
		else if (Fluct->fValue >= Max)
		{
			Fluct->fValue = Max;
		}
	}

	// 出現時の乱数
	if (GetKeyboardTrigger(DIK_R))
	{ // 出現時に乱数を使用するか否か
		Fluct->initial.bIsRandom = !Fluct->initial.bIsRandom;
	}
	if (GetKeyboardPress(DIK_T))
	{ // 乱数の最小値の数値を小さくする
		if (Fluct->initial.fMin > Min)
		{
			Fluct->initial.fMin += -Add;
		}
		else if (Fluct->initial.fMin <= Min)
		{
			Fluct->initial.fMin = Min;
		}
	}
	else if (GetKeyboardPress(DIK_Y))
	{ // 乱数の最小値の数値を大きくする
		if (Fluct->initial.fMin < Fluct->initial.fMax)
		{
			Fluct->initial.fMin += Add;
		}
		else if (Fluct->initial.fMin >= Fluct->initial.fMax)
		{
			Fluct->initial.fMin = Fluct->initial.fMax;
		}
	}

	if (GetKeyboardPress(DIK_U))
	{ // 乱数の最大値の数値を小さくする
		if (Fluct->initial.fMax > Fluct->initial.fMin)
		{
			Fluct->initial.fMax += -Add;
		}
		else if (Fluct->initial.fMax <= Fluct->initial.fMin)
		{
			Fluct->initial.fMax = Fluct->initial.fMin;
		}
	}
	else if (GetKeyboardPress(DIK_I))
	{ // 乱数の最大値の数値を大きくする
		if (Fluct->initial.fMax < Max)
		{
			Fluct->initial.fMax += Add;
		}
		else if (Fluct->initial.fMax >= Max)
		{
			Fluct->initial.fMax = Max;
		}
	}

	// 加算時の乱数
	if (GetKeyboardTrigger(DIK_F))
	{ // 加算時に乱数を使用するか否か
		Fluct->Add.bIsRandom = !Fluct->Add.bIsRandom;
	}
	if (GetKeyboardPress(DIK_G))
	{ // 乱数の最小値の数値を小さくする
		if (Fluct->Add.fMin > -Max)
		{
			Fluct->Add.fMin += -Add;
		}
		else if (Fluct->Add.fMin <= -Max)
		{
			Fluct->Add.fMin = -Max;
		}
	}
	else if (GetKeyboardPress(DIK_H))
	{ // 乱数の最小値の数値を大きくする
		if (Fluct->Add.fMin < Fluct->Add.fMax)
		{
			Fluct->Add.fMin += Add;
		}
		else if (Fluct->Add.fMin <= Fluct->Add.fMax)
		{
			Fluct->Add.fMin = Fluct->Add.fMax;
		}
	}

	if (GetKeyboardPress(DIK_J))
	{ // 乱数の最大値の数値を小さくする
		if (Fluct->Add.fMax > Fluct->Add.fMin)
		{
			Fluct->Add.fMax += -Add;
		}
		else if (Fluct->Add.fMax >= Fluct->Add.fMin)
		{
			Fluct->Add.fMax = Fluct->Add.fMin;
		}
	}
	else if (GetKeyboardPress(DIK_K))
	{ // 乱数の最大値の数値を大きくする
		if (Fluct->Add.fMax < Max)
		{
			Fluct->Add.fMax += Add;
		}
		else if (Fluct->Add.fMax >= Max)
		{
			Fluct->Add.fMax = Max;
		}
	}
}
