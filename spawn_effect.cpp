//**************************************************************************************************
//
// エフェクトの設定処理
// Auther：Karasaki Yuuto
// Auther：Yuda Kaito
//
//**************************************************************************************************
//************************************************************
// include
//************************************************************
#include "main.h"
#include "spawn_effect.h"
#include "SaveEffect.h"
#include "adjusteffect.h"
#include "LoadEffect.h"
#include "input.h"
#include "effect.h"
#include <time.h>

//************************************************************
// マクロ定義
//************************************************************

//************************************************************
// プロトタイプ宣言
//************************************************************

static void ConfigSpawnPoint(void);

static void SelectModeChange(void);			// モードチェンジ
static void RandModeChange(void);			// ↑統合、乱数モードの切り替え
static void InjectionModeChange(void);		// ↑統合、射出モードの切り替え
static void BlendModeChange(void);			// ↑統合、ブレンドモードの切り替え
static void TestParticle(void);				// 一回のみの出現のテスト
static void MovingLimit(void);				// 移動制限

//*******************************************************************************
// static 変数
//*******************************************************************************
static SpawnPoint g_aSpawnPoint[MAX_PARTICLE_CATEGORY];		// スポーンエフェクトの情報
static Effect g_aParticle[MAX_PARTICLE_CATEGORY];			// パーティクル情報
static int s_nPointCnt = -1;								// データ切り替えの変数
static int g_nMaxSwitch = 0;								// 切り替え可能なデータの数
static int g_nMode = 0;										// モードの切り替え
static int s_nAdjustInfo = 0;								// 切り替えデータの変更

//********************************************************************************
// エフェクトの初期化処理
//********************************************************************************
void InitSpawnEffect(void)
{
	// 乱数の初期化
	srand((unsigned int)time(0));

	// スポーンエフェクトの各数値の初期化
	ZeroMemory(g_aSpawnPoint, sizeof(g_aSpawnPoint));
	// パーティクルの各数値の初期化
	ZeroMemory(g_aParticle, sizeof(g_aParticle));
}

//********************************************************************************
// エフェクトの終了処理
//********************************************************************************
void UninitSpawnEffect(void)
{
}

//********************************************************************************
// エフェクトの更新処理
//********************************************************************************
void UpdateSpawnEffect(void)
{
	// モードの切り替え変数
	if (GetKeyboardTrigger(DIK_F9))
	{// [T]キーが押されたとき
		g_nMode++;

		if (g_nMode >= 2)
		{// 2以上の時
			g_nMode = 0;
		}
	}

#ifndef TEST_MODE
	SetEffect(D3DXVECTOR3(SCREEN_WIDH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), TYPE01_EFFECT);
#else
	// 扱うParticleを切り替える
	if (s_nPointCnt != -1)
	{
		int nNextParticle = s_nPointCnt;	// 切り替えの値を保存

		if (GetKeyboardTrigger(DIK_TAB))
		{// [TAB]キーが押されたとき
			nNextParticle++;
		}
		else if (GetKeyboardTrigger(DIK_LSHIFT))
		{// [LSHIFT]キーが押されたとき
			nNextParticle += g_nMaxSwitch - 1;
		}

		nNextParticle %= g_nMaxSwitch;
		s_nPointCnt = nNextParticle;
	}

	if (g_nMode == 0)
	{// モードが0の時)
	 // エフェクト数の設定
		AdjustCountSpawnEffect();

		Effect* pParticle = &(g_aParticle[s_nPointCnt]);
		// エフェクトの設定
		if (g_aSpawnPoint[s_nPointCnt].bUse && pParticle->bUse)
		{
			// 編集する情報を変更
			if (GetKeyboardTrigger(DIK_Q))
			{
				s_nAdjustInfo++;
				if (s_nAdjustInfo == MAX_ADJUST_INFO)
				{
					s_nAdjustInfo = MAX_ADJUST_INFO - 1;
				}
			}
			else if (GetKeyboardTrigger(DIK_E))
			{
				s_nAdjustInfo--;
				if (s_nAdjustInfo <= -1)
				{
					s_nAdjustInfo = 0;
				}
			}

			switch (s_nAdjustInfo)
			{
			case ADJUST_INFO_ONESHOT:
				// 一度に出現するエフェクト数
				AdjustOneShotEffect(pParticle);
				break;
			case ADJUST_INFO_ROT:
				// エフェクトの角度調整処理
				AdjustRotEffect(pParticle);
				break;
			case ADJUST_INFO_SPREAD:
				// エフェクトの発射角度調整処理
				AdjustSpreadEffect(pParticle);
				break;
			case ADJUST_INFO_MOVE:
				// エフェクトの移動量調整処理
				AdjustMoveEffect(pParticle);
				break;
			case ADJUST_INFO_RADIUS:
				// エフェクトの半径調整処理
				AdjustRadiusEffect(pParticle);
				break;
			case ADJUST_INFO_LIFE:
				// エフェクトの寿命調整処理
				AdjustLifeEffect(pParticle);
				break;
			case ADJUST_INFO_COLR:
				// エフェクトの色(Red)調整処理
				AdjustSFluctFloat(&(pParticle->colR), 0.01f, 1.0f, 0.0f);
				break;											  
			case ADJUST_INFO_COLG:								  
				// エフェクトの色(Green)調整処理				
				AdjustSFluctFloat(&(pParticle->colG), 0.01f, 1.0f, 0.0f);
				break;
			case ADJUST_INFO_COLB:
				// エフェクトの色(Blue)調整処理
				AdjustSFluctFloat(&(pParticle->colB), 0.01f, 1.0f, 0.0f);
				break;
			case ADJUST_INFO_COLA:
				// エフェクトの色(Alpha)調整処理
				AdjustSFluctFloat(&(pParticle->colA), 0.01f, 1.0f, 0.0f);
				break;
			case ADJUST_INFO_GRAVITY:
				// エフェクトの重力の調整
				AdjustGravityEffect(pParticle);
				break;
			default:
				break;
			}

			// 乱数モードの切り替え
			RandModeChange();
			// 射出モードの切り替え
			InjectionModeChange();
			// ブレンドモードの切り替え
			BlendModeChange();

			for (int i = 0; i < MAX_PARTICLE_CATEGORY; i++)
			{
				pParticle = &(g_aParticle[i]);

				if (!(pParticle->bUse))
				{
					continue;
				}

				for (int j = 0; j < pParticle->nPopCnt; j++)
				{
					// エフェクトの設定
					SetEffect(D3DXVECTOR3(0.0f,0.0f,0.0f), TEST_TYPE_EFFECT);
				}
			}

			// スポーンエフェクトの設定
			ConfigSpawnPoint();
		}
	}
	if (g_nMode == 1)
	{
		TestParticle();
	}

	// データの保存
	if (GetKeyboardTrigger(DIK_F3))
	{// [F3]キーが押されたとき
		SaveEffect(g_aParticle[s_nPointCnt]);
	}
#endif // !TEAT_MODE
}

//********************************************************************************
// スポーンエフェクトの更新処理
//********************************************************************************
void ConfigSpawnPoint(void)
{
	// エフェクトの移動処理
	MoveSpawnEffect();

	// SpawnPointをポインタ型に変換
	SpawnPoint* SpawnPoint = &(g_aSpawnPoint[s_nPointCnt]);

	// 移動量の更新
	SpawnPoint->pos += SpawnPoint->move;

	// 移動量の更新(減衰させる)
	SpawnPoint->move.x += (0.0f - SpawnPoint->move.x) * DAMPING_COEFFICIENT;
	SpawnPoint->move.y += (0.0f - SpawnPoint->move.y) * DAMPING_COEFFICIENT;

	// 出現位置のスクリーン制限
	MovingLimit();

}

//********************************************************************************
// スポーンエフェクトの数の調整処理
//********************************************************************************
void AdjustCountSpawnEffect(void)
{
	Effect* pParticle = &(g_aParticle[s_nPointCnt]);
	if (GetKeyboardTrigger(DIK_0) && !(g_nMaxSwitch >= MAX_PARTICLE_CATEGORY))
	{// [0]キーが押されたとき
		SetSpawnEffect();

		g_nMaxSwitch++;
		s_nPointCnt = g_nMaxSwitch - 1;
		pParticle->Injection = (INJECTIONMODE)0;
	}
	else if (GetKeyboardTrigger(DIK_9) && g_nMaxSwitch >= 0)
	{// [9]キーが押されたとき
		g_aSpawnPoint[g_nMaxSwitch - 1].bUse = false;	// 使用していない
		g_aParticle[g_nMaxSwitch - 1].bUse = false;		// 使用していない

		g_nMaxSwitch--;
		s_nPointCnt = g_nMaxSwitch - 1;
		pParticle->Injection = (INJECTIONMODE)0;
	}
}

//********************************************************************************
// スポーンエフェクトの移動処理
//********************************************************************************
void MoveSpawnEffect(void)
{
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (GetKeyboardPress(DIK_A))
	{	// [A]キーが押されたとき
		// 位置の更新
		vec.x += sinf(D3DX_PI * -0.5f);
		vec.y += cosf(D3DX_PI * -0.5f);
	}
	if (GetKeyboardPress(DIK_D))
	{	// [D]キーが押されたとき
		// 位置の更新
		vec.x += sinf(D3DX_PI * 0.5f);
		vec.y += cosf(D3DX_PI * 0.5f);
	}
	if (GetKeyboardPress(DIK_W))
	{	// [W]キーが押されたとき
		// 位置の更新
		vec.x += sinf(D3DX_PI);
		vec.y += cosf(D3DX_PI);
	}

	if (GetKeyboardPress(DIK_S))
	{	// [S]キーが押されたとき
		// 位置の更新
		vec.x += sinf(D3DX_PI - D3DX_PI);
		vec.y += cosf(D3DX_PI - D3DX_PI);
	}

	// ベクトルが加算されていたら
	if (!(D3DXVec3Length(&vec) == 0.0f))
	{
		D3DXVec3Normalize(&vec, &vec);	// 正規化する(大きさ１のベクトルにする)
		g_aSpawnPoint[s_nPointCnt].move += vec *  g_aSpawnPoint[s_nPointCnt].fSpeed;
	}
}

//********************************************************************************
// 乱数モードを切り替えをする
//********************************************************************************
void RandModeChange(void)
{
	Effect* pParticle = &(g_aParticle[s_nPointCnt]);

	if (GetKeyboardTrigger(DIK_G))
	{// [G]キーが押されたとき
	}
}

//********************************************************************************
// 射出モードを切り替えをする
//********************************************************************************
void InjectionModeChange(void)
{
	if (GetKeyboardTrigger(DIK_F4))
	{// [H]キーが押されたとき
		Effect* pParticle = &(g_aParticle[s_nPointCnt]);

		pParticle->Injection = (INJECTIONMODE)(pParticle->Injection + 1);

		if (pParticle->Injection >= MAX_PARTICLMODE)
		{// 収束している
			pParticle->Injection = (INJECTIONMODE)0;
		}

#if 0
		int nextMode;
		nextMode = pParticle->Injection + 1; // ふやすとき。
		nextMode = pParticle->Injection + MAX_PARTICLMODE - 1; // へらすとき。
		nextMode %= MAX_PARTICLMODE;
		pParticle->Injection = (INJECTIONMODE)nextMode;
#endif
	}
}

//********************************************************************************
// ブレンドモードを切り替えをする
//********************************************************************************
void BlendModeChange(void)
{
	// ブレンドモードの切り替え
	if (GetKeyboardTrigger(DIK_F2))
	{// [Y]キーが押されたとき
		Effect* pParticle = &(g_aParticle[s_nPointCnt]);

		pParticle->blend = (BLENDMODE)(pParticle->blend + 1);

		if (pParticle->blend >= MAX_BLENDMODE)
		{// 加算合成
			pParticle->blend = (BLENDMODE)0;
		}
	}
}

//********************************************************************************
// 一度飲みの出現テスト
//********************************************************************************
void TestParticle(void)
{
	if (GetKeyboardPress(DIK_DOWN))
	{// [↓]キーが押されたとき
		if (g_aSpawnPoint[s_nPointCnt].nCountParticle > 1)
		{
			g_aSpawnPoint[s_nPointCnt].nCountParticle--;
		}
		else if (g_aSpawnPoint[s_nPointCnt].nCountParticle <= 1)
		{
			g_aSpawnPoint[s_nPointCnt].nCountParticle = 1;
		}
	}
	else if (GetKeyboardPress(DIK_UP))
	{// [↑]キーが押されたとき
		if (g_aSpawnPoint[s_nPointCnt].nCountParticle < 1000)
		{
			g_aSpawnPoint[s_nPointCnt].nCountParticle++;
		}
		else if (g_aSpawnPoint[s_nPointCnt].nCountParticle >= 1000)
		{
			g_aSpawnPoint[s_nPointCnt].nCountParticle = 1000;
		}
	}
	if (GetKeyboardTrigger(DIK_RETURN))
	{// [Enter]キーが押されたとき
		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE_CATEGORY; nCntParticle++)
		{
			if (g_aParticle[nCntParticle].bUse)
			{
				for (int nCountEffect = 0; nCountEffect < g_aParticle[nCntParticle].nPopCnt; nCountEffect++)
				{// Particleの設定
					for (int k = 0; k < g_aSpawnPoint[s_nPointCnt].nCountParticle; k++)
					{
						SetEffect(g_aSpawnPoint[s_nPointCnt].pos, TEST_TYPE_EFFECT);
					}
				}
			}
		}
	}
}

//********************************************************************************
// 出現位置の移動制限
//********************************************************************************
void MovingLimit(void)
{
	if (g_aSpawnPoint[s_nPointCnt].pos.x < 0.0f + POS_RAND_X)
	{
		g_aSpawnPoint[s_nPointCnt].pos.x = 0.0f + POS_RAND_X;

		// 移動量の更新
		g_aSpawnPoint[s_nPointCnt].move.x = 0.0f;
		g_aSpawnPoint[s_nPointCnt].move.y = 0.0f;
	}
	else if (g_aSpawnPoint[s_nPointCnt].pos.x > SCREEN_WIDTH - POS_RAND_X)
	{
		g_aSpawnPoint[s_nPointCnt].pos.x = SCREEN_WIDTH - POS_RAND_X;

		// 移動量の更新
		g_aSpawnPoint[s_nPointCnt].move.x = 0.0f;
		g_aSpawnPoint[s_nPointCnt].move.y = 0.0f;
	}
	else if (g_aSpawnPoint[s_nPointCnt].pos.y < 0.0f + POS_RAND_Y)
	{
		g_aSpawnPoint[s_nPointCnt].pos.y = 0.0f + POS_RAND_Y;

		// 移動量の更新
		g_aSpawnPoint[s_nPointCnt].move.x = 0.0f;
		g_aSpawnPoint[s_nPointCnt].move.y = 0.0f;
	}
	else if (g_aSpawnPoint[s_nPointCnt].pos.y > SCREEN_HEIGHT - POS_RAND_Y)
	{
		g_aSpawnPoint[s_nPointCnt].pos.y = SCREEN_HEIGHT - POS_RAND_Y;

		// 移動量の更新
		g_aSpawnPoint[s_nPointCnt].move.x = 0.0f;
		g_aSpawnPoint[s_nPointCnt].move.y = 0.0f;
	}
}

//********************************************************************************
// エフェクトの一度の出現に出現する数の調整
//********************************************************************************
void AdjustPopEffect(void)
{
	if (GetKeyboardPress(DIK_DOWN))
	{// [↓]キーが押されたとき
		if (g_aParticle[s_nPointCnt].nCountParticle > 1)
		{
			g_aParticle[s_nPointCnt].nCountParticle--;
		}
		else if (g_aParticle[s_nPointCnt].nCountParticle <= 1)
		{
			g_aParticle[s_nPointCnt].nCountParticle = 1;
		}
	}
	else if (GetKeyboardPress(DIK_UP))
	{// [↑]キーが押されたとき
		if (g_aParticle[s_nPointCnt].nCountParticle < 1000)
		{
			g_aParticle[s_nPointCnt].nCountParticle++;
		}
		else if (g_aParticle[s_nPointCnt].nCountParticle >= 1000)
		{
			g_aParticle[s_nPointCnt].nCountParticle = 1000;
		}
	}
}

//********************************************************************************
// エフェクトの描画処理
//********************************************************************************
void DrawSpawnEffect(void)
{

}

//********************************************************************************
// エフェクトの設定処理
//********************************************************************************
void SetSpawnEffect(void)
{
	// 変数宣言
	SpawnPoint* pSpawnEffect;
	Effect* pParticle;

	for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE_CATEGORY; nCntParticle++)
	{
		pSpawnEffect = &(g_aSpawnPoint[nCntParticle]);
		pParticle = &(g_aParticle[nCntParticle]);

		if (pSpawnEffect->bUse && pParticle->bUse)
		{
			continue;
		}

		// スポーンエフェクトの各数値の初期化
		pSpawnEffect->pos = D3DXVECTOR3(POS_SPAWN_EFFECT_X, POS_SPAWN_EFFECT_Y, 0.0f);	//位置
		pSpawnEffect->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//移動量
		pSpawnEffect->fSpeed = SPEED_SPAWN_EFFECT;										//移動速度
		pSpawnEffect->bUse = true;														// 使用している

		//パーティクル情報の初期化
		g_aParticle[nCntParticle] = LoadEffect(TEST_TYPE_EFFECT);

		SetRandom(&pParticle->colR.initial, &pParticle->colR.fValue);				// Redの出現時の数値
		SetRandom(&pParticle->colG.initial, &pParticle->colG.fValue);				// Greenの出現時の数値
		SetRandom(&pParticle->colB.initial, &pParticle->colB.fValue);				// Blueの出現時の数値
		SetRandom(&pParticle->colA.initial, &pParticle->colA.fValue);				// Alphaの出現時の数値
		SetRandom(&pParticle->speedX.initial, &pParticle->speedX.fValue);
		SetRandom(&pParticle->width.initial, &pParticle->width.fValue);
		SetRandom(&pParticle->height.initial, &pParticle->height.fValue);
		SetRandom(&pParticle->shotAngleZ.initial, &pParticle->shotAngleZ.fValue);
		//SetRandom(&pParticle->randLife, &pParticle->randLife.fValue);
		pParticle->col.r = pParticle->colR.fValue;
		pParticle->col.g = pParticle->colG.fValue;
		pParticle->col.b = pParticle->colB.fValue;
		pParticle->col.a = pParticle->colA.fValue;
		pParticle->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// 位置
		pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// 向き
		pParticle->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// 移動量
		pParticle->Injection = PARTICLMODE_DIVERGENT;							// モード
		pParticle->blend = BLENDMODE_ADDITION;									// ブレンドモード

		pParticle->nPopCnt = 1;					// エフェクトの最大数
		pParticle->nCountParticle = 60;			// パーティクルの発生カウント
		pParticle->bUse = true;					// 使用している

		break;
	}
}

//********************************************************************************
// パーティクルの情報取得
//********************************************************************************
Effect *GetParticle(void)
{
	return &g_aParticle[0];
}

//********************************************************************************
// スポーンパーティクルの情報取得
//********************************************************************************
SpawnPoint *GetSpawnEffect(void)
{
	return &g_aSpawnPoint[0];
}

//********************************************************************************
// 扱うパーティクル数の情報取得
//********************************************************************************
int GetMaxSwitch(void)
{
	return g_nMaxSwitch;
}

//********************************************************************************
// 扱うパーティクル情報の情報取得
//********************************************************************************
int GetSwitch(void)
{
	return s_nPointCnt;
}

//********************************************************************************
// 現在のモードの情報取得
//********************************************************************************
int GetMode(void)
{
	return g_nMode;
}

//********************************************************************************
// 調整する属性の情報取得
//********************************************************************************
int GetAdjustInfo(void)
{
	return s_nAdjustInfo;
}