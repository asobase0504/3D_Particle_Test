//**************************************************************************************************
//
// �G�t�F�N�g�̃X�|�[���ݒ菈������
// Auther�F�������l
// Auther�FYuda Kaito
//
//**************************************************************************************************
#ifndef _SPAWN_EFFECT_H_
#define _SPAWN_EFFECT_H_

#include "effect.h"

//**********************************************************
// �}�N����`
//**********************************************************
#define TEST_MODE

#define POS_SPAWN_EFFECT_X		(640.0f)		// �X�|�[���G�t�F�N�g�̏����ʒu(X)
#define POS_SPAWN_EFFECT_Y		(360.0f)		// �X�|�[���G�t�F�N�g�̏����ʒu(Y)
#define SPEED_SPAWN_EFFECT		(1.0f)			// �ړ����x
#define DAMPING_COEFFICIENT		(0.15f)			// �����W��
#define MAX_PARTICLE			(5)				// �p�[�e�B�N���̍ő吔
#define POS_RAND_X				(20)			// �ʒu�̗����͈�[X]
#define POS_RAND_Y				(20)			// �ʒu�̗����͈�[Y]
#define MAX_PARTICLE_CATEGORY	(128)			// �p�[�e�B�N���̎��

////**********************************************************
//// �ˏo���[�h�̗񋓌^
////**********************************************************
//typedef enum
//{
//	PARTICLMODE_DIVERGENT = 0,		// ���U
//	PARTICLMODE_CONVERGENCE,		// ����
//	PARTICLMODE_ROTATION,			// ��]
//	MAX_PARTICLMODE					// �p�[�e�B�N�����[�h�̍ő吔
//}INJECTIONMODE;
//
////**********************************************************
//// �u�����h���[�h�̗񋓌^
////**********************************************************
//typedef enum
//{
//	BLENDMODE_ADDITION = 0,		// ���Z����
//	BLENDMODE_SUBTRACT,			// ���Z����
//	MAX_BLENDMODE				// �u�����h���[�h�̍ő吔
//}BLENDMODE;
//

//**********************************************************
// �u�����h���[�h�̗񋓌^
//**********************************************************
typedef enum
{
	ADJUST_INFO_ONESHOT = 0,	// ��x�ɏo�����鐔
	ADJUST_INFO_ROT,			// �{�̂̊p�x
	ADJUST_INFO_SPREAD,			// ���ˊp�x
	ADJUST_INFO_MOVE,			// �ړ����x
	ADJUST_INFO_RADIUS,			// ���a
	ADJUST_INFO_LIFE,			// ����
	ADJUST_INFO_COLR,			// �ԐF
	ADJUST_INFO_COLG,			// �ΐF
	ADJUST_INFO_COLB,			// �F
	ADJUST_INFO_COLA,			// ����
	ADJUST_INFO_GRAVITY,		// �d��
	MAX_ADJUST_INFO				// �u�����h���[�h�̍ő吔
}ADJUST_INFO;

//**********************************************************
// �G�t�F�N�g�o���ʒu�̍\����
//**********************************************************
typedef struct
{
	D3DXVECTOR3		pos;			// �ʒu
	D3DXVECTOR3		move;			// �ړ�����
	float			fSpeed;			// �ړ����x
	bool			bUse;			// �g�p���Ă��邩
	int				nCountParticle;
}SpawnPoint;

////************************************************************
//// �������[�h�\����
////************************************************************
//typedef struct
//{
//	bool bRandLife;	// �����̗���
//	bool bRandMove;	// �ړ��ʂ̗���
//}RandMode;

//************************************************************
// �v���g�^�C�v�錾
//************************************************************
void InitSpawnEffect(void);			// �X�|�[���G�t�F�N�g�̏���������
void UninitSpawnEffect(void);		// �X�|�[���G�t�F�N�g�̏I������
void UpdateSpawnEffect(void);		// �X�|�[���G�t�F�N�g�̍X�V����
void AdjustCountSpawnEffect(void);	// �X�|�[���G�t�F�N�g�̐��̒���
void MoveSpawnEffect(void);			// �X�|�[���G�t�F�N�g�̈ړ�����
void DrawSpawnEffect(void);			// �X�|�[���G�t�F�N�g�̕`�揈��
void SetSpawnEffect(void);			// �X�|�[���G�t�F�N�g�̐ݒ菈��
Effect *GetParticle(void);		// �p�[�e�B�N���̏��擾����
SpawnPoint *GetSpawnEffect(void);	// �X�|�[���G�t�F�N�g�̏��擾����
int GetMaxSwitch(void);
int GetSwitch(void);
int GetMode(void);
int GetAdjustInfo(void);

#endif
