//**************************************************************************************************
//
// エフェクトのスポーン設定処理処理
// Auther：Yuda Kaito
//
//**************************************************************************************************
#ifndef _ADJUSTEFFECT_H_
#define _ADJUSTEFFECT_H_

#include "effect.h"

//**********************************************************
// マクロ定義
//**********************************************************

//************************************************************
// プロトタイプ宣言
//************************************************************
void AdjustMoveEffect(Effect* pParticle);		// エフェクトの移動量調整処理
void AdjustOneShotEffect(Effect* pParticle);	// 一度に発生するエフェクトの数調整処理
void AdjustRadiusEffect(Effect* pParticle);		// エフェクトの半径調整処理
void AdjustLifeEffect(Effect* pParticle);		// エフェクトの寿命調整処理
void AdjustGravityEffect(Effect* pParticle);	// エフェクトの重力調整処理
void AdjustRotEffect(Effect* pParticle);		// エフェクトの角度調整処理
void AdjustSpreadEffect(Effect* pParticle);		// エフェクトの飛散角度の調整処理
void AdjustSFluctFloat(SFluctFloat* Fluct, float Add, float Max, float Min);	// SFluctFloatの値の調整処理

#endif
