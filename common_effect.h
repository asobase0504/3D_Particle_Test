//=========================================
// 
// 共通処理(ヘッダーファイル)
// Author YudaKaito
// 
//=========================================
#ifndef _COMMON_EFFECT_H_
#define _COMMON_EFFECT_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "effect.h"

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void SetRandom(FRandFloat* fluct, float* fValue);	// float型の乱数適用
void SetRandom(FRandInt* fluct, int* nValue);		// int型の乱数適用


#endif // !_COMMON_EFFECT_H_
