//**************************************************************************************************
//
// エフェクトのスポーン設定処理処理
// Auther：唐﨑結斗
// Auther：Yuda Kaito
//
//**************************************************************************************************
#ifndef _SPAWN_EFFECT_H_
#define _SPAWN_EFFECT_H_

#include "effect.h"

//**********************************************************
// マクロ定義
//**********************************************************
#define TEST_MODE

#define POS_SPAWN_EFFECT_X		(640.0f)		// スポーンエフェクトの初期位置(X)
#define POS_SPAWN_EFFECT_Y		(360.0f)		// スポーンエフェクトの初期位置(Y)
#define SPEED_SPAWN_EFFECT		(1.0f)			// 移動速度
#define DAMPING_COEFFICIENT		(0.15f)			// 減衰係数
#define MAX_PARTICLE			(5)				// パーティクルの最大数
#define POS_RAND_X				(20)			// 位置の乱数範囲[X]
#define POS_RAND_Y				(20)			// 位置の乱数範囲[Y]
#define MAX_PARTICLE_CATEGORY	(128)			// パーティクルの種類

////**********************************************************
//// 射出モードの列挙型
////**********************************************************
//typedef enum
//{
//	PARTICLMODE_DIVERGENT = 0,		// 発散
//	PARTICLMODE_CONVERGENCE,		// 収束
//	PARTICLMODE_ROTATION,			// 回転
//	MAX_PARTICLMODE					// パーティクルモードの最大数
//}INJECTIONMODE;
//
////**********************************************************
//// ブレンドモードの列挙型
////**********************************************************
//typedef enum
//{
//	BLENDMODE_ADDITION = 0,		// 加算合成
//	BLENDMODE_SUBTRACT,			// 減算合成
//	MAX_BLENDMODE				// ブレンドモードの最大数
//}BLENDMODE;
//

//**********************************************************
// ブレンドモードの列挙型
//**********************************************************
typedef enum
{
	ADJUST_INFO_ONESHOT = 0,	// 一度に出現する数
	ADJUST_INFO_ROT,			// 本体の角度
	ADJUST_INFO_SPREAD,			// 発射角度
	ADJUST_INFO_MOVE,			// 移動速度
	ADJUST_INFO_RADIUS,			// 半径
	ADJUST_INFO_LIFE,			// 寿命
	ADJUST_INFO_COLR,			// 赤色
	ADJUST_INFO_COLG,			// 緑色
	ADJUST_INFO_COLB,			// 青色
	ADJUST_INFO_COLA,			// 透明
	ADJUST_INFO_GRAVITY,		// 重力
	MAX_ADJUST_INFO				// ブレンドモードの最大数
}ADJUST_INFO;

//**********************************************************
// エフェクト出現位置の構造体
//**********************************************************
typedef struct
{
	D3DXVECTOR3		pos;			// 位置
	D3DXVECTOR3		move;			// 移動距離
	float			fSpeed;			// 移動速度
	bool			bUse;			// 使用しているか
	int				nCountParticle;
}SpawnPoint;

////************************************************************
//// 乱数モード構造体
////************************************************************
//typedef struct
//{
//	bool bRandLife;	// 寿命の乱数
//	bool bRandMove;	// 移動量の乱数
//}RandMode;

//************************************************************
// プロトタイプ宣言
//************************************************************
void InitSpawnEffect(void);			// スポーンエフェクトの初期化処理
void UninitSpawnEffect(void);		// スポーンエフェクトの終了処理
void UpdateSpawnEffect(void);		// スポーンエフェクトの更新処理
void AdjustCountSpawnEffect(void);	// スポーンエフェクトの数の調整
void MoveSpawnEffect(void);			// スポーンエフェクトの移動処理
void DrawSpawnEffect(void);			// スポーンエフェクトの描画処理
void SetSpawnEffect(void);			// スポーンエフェクトの設定処理
Effect *GetParticle(void);		// パーティクルの情報取得処理
SpawnPoint *GetSpawnEffect(void);	// スポーンエフェクトの情報取得処理
int GetMaxSwitch(void);
int GetSwitch(void);
int GetMode(void);
int GetAdjustInfo(void);

#endif
