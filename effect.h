//=========================================
//
// エフェクトの設定処理処理
// Auther：唐﨑結斗
// Auther：Yuda Kaito
//
//=========================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// エフェクトの種類列挙型
//------------------------------------
typedef enum
{
	TEST_TYPE_EFFECT = 0,
	TYPE01_EFFECT,
	MAX_TYPE_EFFECT
}EFFECT_TYPE;

//------------------------------------
// エフェクトのテクスチャ種類列挙型
//------------------------------------
typedef enum
{
	EFFECT_TEX_RECT = 0,		// テクスチャなし
	EFFECT_TEX_CIRCLE,			// 丸
	EFFECT_TEX_LINE_CIRCLE,		// 線の丸
	EFFECT_TEX_DOUBLE_CIRCLE,	// 二重丸
	EFFECT_TEX_THIN_SQUARE,		// 細い四角
	EFFECT_TEX_THIN_TRIANGLE,	// 三角
	MAX_EFFECT_TYPE_TEX
}EFFECT_TEX;

//------------------------------------
// エフェクトの描画種類列挙型
//------------------------------------
typedef enum
{
	TRIANGLESTRIP = 0,	// 通表描画
	LINESTRIP,			// ラインエフェクト(線)
	TRIANGLESTRIP_LINE,	// ラインエフェクト(テクスチャあり)
	MAX_DRAW_EFFECT		// 最大数
}EFFECT_TYPE_DRAW;

//------------------------------------
// エフェクトの死亡種類列挙型
//------------------------------------
typedef enum
{
	DIED_LIFE = 0,		// 寿命での死亡
	DIED_RADIUS,		// サイズでの死亡
	MAX_DIED_EFFECT		// 最大数
}EFFECT_DIED;

//------------------------------------
// エフェクトの死亡時種類列挙型
//------------------------------------
typedef enum
{
	ON_DIED_NONE = 0,	// 何もせず消去
	ON_DIED_POP,		// エフェクトの出現
	MAX_ON_DIED			// 最大数
}EFFECT_ON_DIED;

//------------------------------------
// 射出モードの列挙型
//------------------------------------
typedef enum
{
	PARTICLMODE_DIVERGENT = 0,		// 発散
	PARTICLMODE_CONVERGENCE,		// 収束
	PARTICLMODE_ROTATION,			// 回転
	MAX_PARTICLMODE					// パーティクルモードの最大数
}INJECTIONMODE;

//------------------------------------
// ブレンドモードの列挙型
//------------------------------------
typedef enum
{
	BLENDMODE_ADDITION = 0,		// 加算合成
	BLENDMODE_SUBTRACT,			// 減算合成
	MAX_BLENDMODE				// ブレンドモードの最大数
}BLENDMODE;

//------------------------------------
// 乱数を使用できる float型
//------------------------------------
typedef struct
{
	float fMax;			// 最大値
	float fMin;			// 最小値
	bool bIsRandom;		// 乱数を使用するか
} FRandFloat;

//------------------------------------
// 乱数を使用できる int型
//------------------------------------
typedef struct
{
	int nMax;			// 最大値
	int nMin;			// 最小値
	bool bIsRandom;		// 乱数を使用するか
} FRandInt;

//------------------------------------
// 変動を使用する float型
//------------------------------------
typedef struct
{
	float fValue;		// 現在値
	float fAddValue;	// 加算値
	FRandFloat initial;	// 初期値
	FRandFloat Add;		// 加算値
} SFluctFloat;

//------------------------------------
// 変動を使用する int型
//------------------------------------
typedef struct
{
	int nValue;			// 現在値
	float fAdd;			// 加算値
	FRandInt initial;	// 初期値
	FRandInt Add;		// 加算値
} SFluctInt;

//------------------------------------
// パーティクル構造体
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// 位置
	D3DXVECTOR3 spaen_pos;	// 出現位置
	D3DXVECTOR3 rand_pos;	// 発射位置
	EFFECT_TEX tex;			// テクスチャ情報
	float fLength;			// 対角線の長さ
	float fAngele;			// 対角線の角度
	SFluctFloat width;		// 幅
	SFluctFloat height;		// 高さ
	SFluctInt randLife;		// 寿命

	// 射出系統変数
	INJECTIONMODE Injection;	// 射出モード
	D3DXVECTOR3 shotAngle;		// 射出角度
	SFluctFloat shotAngleX;		// 射出角度(X)
	SFluctFloat shotAngleY;		// 射出角度(Y)
	SFluctFloat shotAngleZ;		// 射出角度(Z)

	// 向き系統変数
	D3DXVECTOR3		rot;	// 向き
	SFluctFloat		rotX;	// Xの向き
	SFluctFloat		rotY;	// Yの向き
	SFluctFloat		rotZ;	// Zの向き
	
	// 移動系統変数
	float fGravity;			// 重力
	D3DXVECTOR3 move;		// 移動量
	SFluctFloat speedX;		// 移動速度(X軸)
	SFluctFloat speedY;		// 移動速度(Y軸)
	SFluctFloat speedZ;		// 移動速度(Z軸)

	// 色系統変数
	D3DXCOLOR col;		// 頂点カラー
	SFluctFloat colR;	// r値
	SFluctFloat colG;	// g値
	SFluctFloat colB;	// b値
	SFluctFloat colA;	// a値

	// 死亡時系統変数
	EFFECT_DIED died;		// 死亡条件
	EFFECT_ON_DIED onDied;	// 死亡時


	// アニメーション系統変数
	int nCntAnim;		// アニメーションカウンター
	int nPatternAnim;	// 何個目のアニメーションか
	int nDivisionU;		// アニメーションの分割数(U)
	int nDivisionV;		// アニメーションの分割数(V)
	int nDivisionMAX;	// アニメーションの総分割数

	int nPopCnt;			// 一度に出すエフェクト数
	int nCountParticle;		// パーティクルの発生カウント
	bool bUse;				// 使用しているか
	
	// 機能のON/OFF
	bool bIsBillboard;
	BLENDMODE blend;	// ブレンドモード

	//3D時使用
	D3DXMATRIX mtxWorld;	// ワールドマトリックス
}Effect;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos,EFFECT_TYPE type);

void SetRandom(FRandFloat* fluct, float* fValue);	// float型の乱数適用
void SetRandom(FRandInt* fluct, int* nValue);		// int型の乱数適用

#endif
