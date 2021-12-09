//**************************************************************************************************
//
// �G�t�F�N�g�̒����ݒ菈��
// Auther�FKarasaki Yuuto
// Auther�FYuda Kaito
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
// �}�N����`
//************************************************************
#define MAX_NUMBER_EFFECT	(10)		// �G�t�F�N�g�̍ő吔
#define MAX_SPEED_EFFECT	(200.0f)	// �G�t�F�N�g�̍ő�ړ���
#define MIN_RADIUS_EFFECT	(0.0f)		// �G�t�F�N�g�̍ŏ����a
#define MAX_RADIUS_EFFECT	(200.0f)	// �G�t�F�N�g�̍ő唼�a
#define MIN_LIFE_EFFECT		(0)			// �G�t�F�N�g�̍ŏ�����
#define MAX_LIFE_EFFECT		(1200)		// �G�t�F�N�g�̍ő����
#define MIN_COL_EFFECT		(0.0f)		// �G�t�F�N�g�̍ŏ��J���[
#define MAX_COL_EFFECT		(1.0f)		// �G�t�F�N�g�̍ő�J���[

//************************************************************
// �v���g�^�C�v�錾
//************************************************************

//*******************************************************************************
// �ÓI�ϐ�
//*******************************************************************************
															
//********************************************************************************
// �G�t�F�N�g�̈ړ��ʒ�������
//********************************************************************************
void AdjustMoveEffect(Effect* pParticle)
{
	AdjustSFluctFloat(&(pParticle->randSpeed), 0.1f, MAX_SPEED_EFFECT, 0.0f);
}

//********************************************************************************
// ��x�ɔ�������G�t�F�N�g�̐���������
//********************************************************************************
void AdjustOneShotEffect(Effect* pParticle)
{
	int* nPopCnt = &(pParticle->nPopCnt);

	if (GetKeyboardTrigger(DIK_B))
	{// [J]�L�[�������ꂽ�Ƃ�
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
	{// [L]�L�[�������ꂽ�Ƃ�
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
// �G�t�F�N�g�̔��a��������
//********************************************************************************
void AdjustRadiusEffect(Effect* pParticle)
{
	AdjustSFluctFloat(&(pParticle->randRadius), 0.1f, MAX_RADIUS_EFFECT, MIN_RADIUS_EFFECT);
}

//********************************************************************************
// �G�t�F�N�g�̎�����������
//********************************************************************************
void AdjustLifeEffect(Effect* pParticle)
{
	SFluctInt* Life = &(pParticle->randLife);

	// �ʏ펞�̒l
	if (GetKeyboardPress(DIK_B))
	{ // ���l������������
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
	{ // ���l��傫������
		if (Life->nValue < MAX_LIFE_EFFECT)
		{
			Life->nValue++;
		}
		else if (Life->nValue >= MAX_LIFE_EFFECT)
		{
			Life->nValue = MAX_LIFE_EFFECT;
		}
	}

	// �o�����̗���
	if (GetKeyboardTrigger(DIK_R))
	{ // �o�����ɗ������g�p���邩�ۂ�
		Life->initial.bIsRandom = !Life->initial.bIsRandom;
	}
	if (GetKeyboardPress(DIK_Y))
	{ // �����̍ŏ��l�̐��l������������
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
	{ // �����̍ŏ��l�̐��l��傫������
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
	{ // �����̍ő�l�̐��l������������
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
	{ // �����̍ő�l�̐��l��傫������
		if (Life->initial.nMax < MAX_LIFE_EFFECT)
		{
			Life->initial.nMax++;
		}
		else if (Life->initial.nMax >= MAX_LIFE_EFFECT)
		{
			Life->initial.nMax = MAX_LIFE_EFFECT;
		}
	}

	// ���Z���̗���
	if (GetKeyboardTrigger(DIK_F))
	{ // ���Z���ɗ������g�p���邩�ۂ�
		Life->Add.bIsRandom = !Life->Add.bIsRandom;
	}
	if (GetKeyboardPress(DIK_G))
	{ // �����̍ŏ��l�̐��l������������
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
	{ // �����̍ŏ��l�̐��l��傫������
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
	{ // �����̍ő�l�̐��l������������
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
	{ // �����̍ő�l�̐��l��傫������
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
// �G�t�F�N�g�̏d�͒�������
//********************************************************************************
void AdjustGravityEffect(Effect* pParticle)
{
	if (GetKeyboardPress(DIK_B))
	{// [B]�L�[�������ꂽ�Ƃ�
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
	{// [M]�L�[�������ꂽ�Ƃ�
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
// �G�t�F�N�g�̊p�x��������
//********************************************************************************
void AdjustRotEffect(Effect* pParticle)
{
	if (GetKeyboardPress(DIK_E))
	{// [E]�L�[�������ꂽ�Ƃ�
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
	{// [Q]�L�[�������ꂽ�Ƃ�
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
// �G�t�F�N�g�̔�U�p�x��������
//********************************************************************************
void AdjustSpreadEffect(Effect* pParticle)
{
	if (GetKeyboardPress(DIK_Z))
	{// [Z]�L�[�������ꂽ�Ƃ�
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
	{// [C]�L�[�������ꂽ�Ƃ�
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
// SFluctFloat�̒l�̃L�[���͏���
//********************************************************************************
void AdjustSFluctFloat(SFluctFloat* Fluct, float Add, float Max, float Min)
{
	// �ʏ펞�̒l
	if (GetKeyboardPress(DIK_B))
	{ // ���l������������
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
	{ // ���l��傫������
		if (Fluct->fValue < Max)
		{
			Fluct->fValue += Add;
		}
		else if (Fluct->fValue >= Max)
		{
			Fluct->fValue = Max;
		}
	}

	// �o�����̗���
	if (GetKeyboardTrigger(DIK_R))
	{ // �o�����ɗ������g�p���邩�ۂ�
		Fluct->initial.bIsRandom = !Fluct->initial.bIsRandom;
	}
	if (GetKeyboardPress(DIK_T))
	{ // �����̍ŏ��l�̐��l������������
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
	{ // �����̍ŏ��l�̐��l��傫������
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
	{ // �����̍ő�l�̐��l������������
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
	{ // �����̍ő�l�̐��l��傫������
		if (Fluct->initial.fMax < Max)
		{
			Fluct->initial.fMax += Add;
		}
		else if (Fluct->initial.fMax >= Max)
		{
			Fluct->initial.fMax = Max;
		}
	}

	// ���Z���̗���
	if (GetKeyboardTrigger(DIK_F))
	{ // ���Z���ɗ������g�p���邩�ۂ�
		Fluct->Add.bIsRandom = !Fluct->Add.bIsRandom;
	}
	if (GetKeyboardPress(DIK_G))
	{ // �����̍ŏ��l�̐��l������������
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
	{ // �����̍ŏ��l�̐��l��傫������
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
	{ // �����̍ő�l�̐��l������������
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
	{ // �����̍ő�l�̐��l��傫������
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
