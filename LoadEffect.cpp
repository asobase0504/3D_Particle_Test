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
	"data/FILE/TestParticle.txt",
	"data/FILE/Particle.txt",
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
	pFile = fopen(pFilename[type], "r");

	char read[255] = {};

	// 読み込み処理
	while (1)
	{
		fscanf(pFile, "%s", &read);

		if (strcmp(&read[0], "TEXTURE") == 0)
		{
			fscanf(pFile, "%s", &read);			// = の除去
			fscanf(pFile, "%d", &effect.tex);	// 値を入れる

		}
		else if (strcmp(&read[0], "ColRed") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.colR.fValue);	// 値を入れる
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.colR.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.colR.initial.fMax);
					fscanf(pFile, "%f", &effect.colR.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.colR.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.colR.Add.fMax);
					fscanf(pFile, "%f", &effect.colR.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "ColBlue") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.colB.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.colB.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.colB.initial.fMax);
					fscanf(pFile, "%f", &effect.colB.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.colB.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.colB.Add.fMax);
					fscanf(pFile, "%f", &effect.colB.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "ColGreen") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.colG.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.colG.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.colG.initial.fMax);
					fscanf(pFile, "%f", &effect.colG.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.colG.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.colG.Add.fMax);
					fscanf(pFile, "%f", &effect.colG.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "ColAlpha") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.colA.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.colA.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.colA.initial.fMax);
					fscanf(pFile, "%f", &effect.colA.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.colA.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.colA.Add.fMax);
					fscanf(pFile, "%f", &effect.colA.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "Width") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.width.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.width.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.width.initial.fMax);
					fscanf(pFile, "%f", &effect.width.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.width.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.width.Add.fMax);
					fscanf(pFile, "%f", &effect.width.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "Height") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.height.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.height.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.height.initial.fMax);
					fscanf(pFile, "%f", &effect.height.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.height.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.height.Add.fMax);
					fscanf(pFile, "%f", &effect.height.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "Life") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%d", &effect.randLife.nValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.randLife.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%d", &effect.randLife.initial.nMax);
					fscanf(pFile, "%d", &effect.randLife.initial.nMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.randLife.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%d", &effect.randLife.Add.nMax);
					fscanf(pFile, "%d", &effect.randLife.Add.nMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "Speed_X") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.speedX.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.speedX.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.speedX.initial.fMax);
					fscanf(pFile, "%f", &effect.speedX.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.speedX.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.speedX.Add.fMax);
					fscanf(pFile, "%f", &effect.speedX.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "Speed_Y") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.speedY.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.speedY.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.speedY.initial.fMax);
					fscanf(pFile, "%f", &effect.speedY.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.speedY.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.speedY.Add.fMax);
					fscanf(pFile, "%f", &effect.speedY.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "Speed_Z") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.speedZ.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.speedZ.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.speedZ.initial.fMax);
					fscanf(pFile, "%f", &effect.speedZ.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.speedZ.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.speedZ.Add.fMax);
					fscanf(pFile, "%f", &effect.speedZ.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "Aangle_X") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.shotAngleX.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.shotAngleX.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.shotAngleX.initial.fMax);
					fscanf(pFile, "%f", &effect.shotAngleX.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.shotAngleX.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.shotAngleX.Add.fMax);
					fscanf(pFile, "%f", &effect.shotAngleX.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "Aangle_Y") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.shotAngleY.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.shotAngleZ.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.shotAngleY.initial.fMax);
					fscanf(pFile, "%f", &effect.shotAngleY.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.shotAngleY.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.shotAngleY.Add.fMax);
					fscanf(pFile, "%f", &effect.shotAngleY.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "Aangle_Z") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.shotAngleZ.fValue);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{
					effect.shotAngleZ.initial.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.shotAngleZ.initial.fMax);
					fscanf(pFile, "%f", &effect.shotAngleZ.initial.fMin);
				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{
					effect.shotAngleZ.Add.bIsRandom = true;
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.shotAngleZ.Add.fMax);
					fscanf(pFile, "%f", &effect.shotAngleZ.Add.fMin);
				}
				else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "gravity") == 0)
		{
			while (1)
			{
				fscanf(pFile, "%s", &read);
				if (strcmp(&read[0], "nValue") == 0)
				{
					fscanf(pFile, "%s", &read);					// = の除去
					fscanf(pFile, "%f", &effect.fGravity);
				}
				else if (strcmp(&read[0], "InitialRand") == 0)
				{

				}
				else if (strcmp(&read[0], "AddRand") == 0)
				{

				}
				else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
				{
					break;
				}
			}
		}
		else if (strcmp(&read[0], "DivisionU") == 0)
		{
			fscanf(pFile, "%s", &read);
			if (strcmp(&read[0], "nValue") == 0)
			{
				fscanf(pFile, "%s", &read);					// = の除去
				fscanf(pFile, "%d", &effect.nDivisionU);
			}
			else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
			{
				break;
			}
		}
		else if (strcmp(&read[0], "DivisionV") == 0)
		{
			fscanf(pFile, "%s", &read);
			if (strcmp(&read[0], "nValue") == 0)
			{
				fscanf(pFile, "%s", &read);					// = の除去
				fscanf(pFile, "%d", &effect.nDivisionV);
			}
			else if (strcmp(&read[0], "end") == 0)			// 属性付与終了
			{
				break;
			}
		}
		else if (strcmp(&read[0], "FileEnd") == 0)
		{
			break;
		}
	}

	fclose(pFile);
	return effect;
}