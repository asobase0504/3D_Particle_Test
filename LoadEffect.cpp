//**************************************************************************************************
//
// エフェクトの読込処理
// Auther：Yuda Kaito
//
//**************************************************************************************************
//************************************************************
// include
//************************************************************
#include "main.h"
#include "effect.h"
#include "LoadEffect.h"
#include <assert.h>

//************************************************************
// マクロ定義
//************************************************************

//************************************************************
// プロトタイプ宣言
//************************************************************

//*******************************************************************************
// 静的変数
//*******************************************************************************

//-----------------------
// ファイル名
//-----------------------
static const char* pFilename[] =
{
	"data/FILE/Particle4.bin",
	"data/FILE/Particle4.bin",
	"data/FILE/Particle4.bin",
};
// PFile と TYPEEFFECT の数をチェック
static_assert((sizeof(pFilename) / sizeof(pFilename[0])) == MAX_TYPE_EFFECT, "error ファイルの数と列挙型の数が一致してません");

//********************************************************************************
// テクスチャの読み込み処理
//********************************************************************************
Effect LoadTex(EFFECT_TEX type)
{
	return Effect();
}

//********************************************************************************
// エフェクトの読み込み処理
//********************************************************************************
Effect LoadEffect(EFFECT_TYPE type)
{
	assert(type >= 0 && type < MAX_TYPE_EFFECT);

	FILE* pFile;
	Effect effect;

	ZeroMemory(&effect, sizeof(effect));

	// ファイルを開く
	pFile = fopen(pFilename[type], "rb");

	fread(&effect, sizeof(Effect),1,pFile);

	fclose(pFile);
	return effect;
}