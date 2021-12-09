//**************************************************************************************************
//
// �G�t�F�N�g�̃X�|�[���ݒ菈������
// Auther�FYuda Kaito
//
//**************************************************************************************************
#ifndef _ADJUSTEFFECT_H_
#define _ADJUSTEFFECT_H_

#include "effect.h"

//**********************************************************
// �}�N����`
//**********************************************************

//************************************************************
// �v���g�^�C�v�錾
//************************************************************
void AdjustMoveEffect(Effect* pParticle);		// �G�t�F�N�g�̈ړ��ʒ�������
void AdjustOneShotEffect(Effect* pParticle);	// ��x�ɔ�������G�t�F�N�g�̐���������
void AdjustRadiusEffect(Effect* pParticle);		// �G�t�F�N�g�̔��a��������
void AdjustLifeEffect(Effect* pParticle);		// �G�t�F�N�g�̎�����������
void AdjustGravityEffect(Effect* pParticle);	// �G�t�F�N�g�̏d�͒�������
void AdjustRotEffect(Effect* pParticle);		// �G�t�F�N�g�̊p�x��������
void AdjustSpreadEffect(Effect* pParticle);		// �G�t�F�N�g�̔�U�p�x�̒�������
void AdjustSFluctFloat(SFluctFloat* Fluct, float Add, float Max, float Min);	// SFluctFloat�̒l�̒�������

#endif
