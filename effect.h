//=========================================
//
// �G�t�F�N�g�̐ݒ菈������
// Auther�F�������l
// Auther�FYuda Kaito
//
//=========================================
#ifndef _EFFECT_H_
#define _EFFECT_H_

//------------------------------------
// include
//------------------------------------
#include "main.h"

//------------------------------------
// �G�t�F�N�g�̎�ޗ񋓌^
//------------------------------------
typedef enum
{
	TEST_TYPE_EFFECT = 0,
	TYPE01_EFFECT,
	MAX_TYPE_EFFECT
}EFFECT_TYPE;

//------------------------------------
// �G�t�F�N�g�̃e�N�X�`����ޗ񋓌^
//------------------------------------
typedef enum
{
	EFFECT_TEX_RECT = 0,		// �e�N�X�`���Ȃ�
	EFFECT_TEX_CIRCLE,			// ��
	EFFECT_TEX_LINE_CIRCLE,		// ���̊�
	EFFECT_TEX_DOUBLE_CIRCLE,	// ��d��
	EFFECT_TEX_THIN_SQUARE,		// �ׂ��l�p
	EFFECT_TEX_THIN_TRIANGLE,	// �O�p
	MAX_EFFECT_TYPE_TEX
}EFFECT_TEX;

//------------------------------------
// �G�t�F�N�g�̕`���ޗ񋓌^
//------------------------------------
typedef enum
{
	TRIANGLESTRIP = 0,	// �ʕ\�`��
	LINESTRIP,			// ���C���G�t�F�N�g(��)
	TRIANGLESTRIP_LINE,	// ���C���G�t�F�N�g(�e�N�X�`������)
	MAX_DRAW_EFFECT		// �ő吔
}EFFECT_TYPE_DRAW;

//------------------------------------
// �G�t�F�N�g�̎��S��ޗ񋓌^
//------------------------------------
typedef enum
{
	DIED_LIFE = 0,		// �����ł̎��S
	DIED_RADIUS,		// �T�C�Y�ł̎��S
	MAX_DIED_EFFECT		// �ő吔
}EFFECT_DIED;

//------------------------------------
// �G�t�F�N�g�̎��S����ޗ񋓌^
//------------------------------------
typedef enum
{
	ON_DIED_NONE = 0,	// ������������
	ON_DIED_POP,		// �G�t�F�N�g�̏o��
	MAX_ON_DIED			// �ő吔
}EFFECT_ON_DIED;

//------------------------------------
// �ˏo���[�h�̗񋓌^
//------------------------------------
typedef enum
{
	PARTICLMODE_DIVERGENT = 0,		// ���U
	PARTICLMODE_CONVERGENCE,		// ����
	PARTICLMODE_ROTATION,			// ��]
	MAX_PARTICLMODE					// �p�[�e�B�N�����[�h�̍ő吔
}INJECTIONMODE;

//------------------------------------
// �u�����h���[�h�̗񋓌^
//------------------------------------
typedef enum
{
	BLENDMODE_ADDITION = 0,		// ���Z����
	BLENDMODE_SUBTRACT,			// ���Z����
	MAX_BLENDMODE				// �u�����h���[�h�̍ő吔
}BLENDMODE;

//------------------------------------
// �������g�p�ł��� float�^
//------------------------------------
typedef struct
{
	float fMax;			// �ő�l
	float fMin;			// �ŏ��l
	bool bIsRandom;		// �������g�p���邩
} FRandFloat;

//------------------------------------
// �������g�p�ł��� int�^
//------------------------------------
typedef struct
{
	int nMax;			// �ő�l
	int nMin;			// �ŏ��l
	bool bIsRandom;		// �������g�p���邩
} FRandInt;

//------------------------------------
// �ϓ����g�p���� float�^
//------------------------------------
typedef struct
{
	float fValue;		// ���ݒl
	float fAddValue;	// ���Z�l
	FRandFloat initial;	// �����l
	FRandFloat Add;		// ���Z�l
} SFluctFloat;

//------------------------------------
// �ϓ����g�p���� int�^
//------------------------------------
typedef struct
{
	int nValue;			// ���ݒl
	float fAdd;			// ���Z�l
	FRandInt initial;	// �����l
	FRandInt Add;		// ���Z�l
} SFluctInt;

//------------------------------------
// �p�[�e�B�N���\����
//------------------------------------
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 spaen_pos;	// �o���ʒu
	D3DXVECTOR3 rand_pos;	// ���ˈʒu
	EFFECT_TEX tex;			// �e�N�X�`�����
	float fLength;			// �Ίp���̒���
	float fAngele;			// �Ίp���̊p�x
	SFluctFloat width;		// ��
	SFluctFloat height;		// ����
	SFluctInt randLife;		// ����

	// �ˏo�n���ϐ�
	INJECTIONMODE Injection;	// �ˏo���[�h
	D3DXVECTOR3 shotAngle;		// �ˏo�p�x
	SFluctFloat shotAngleX;		// �ˏo�p�x(X)
	SFluctFloat shotAngleY;		// �ˏo�p�x(Y)
	SFluctFloat shotAngleZ;		// �ˏo�p�x(Z)

	// �����n���ϐ�
	D3DXVECTOR3		rot;	// ����
	SFluctFloat		rotX;	// X�̌���
	SFluctFloat		rotY;	// Y�̌���
	SFluctFloat		rotZ;	// Z�̌���
	
	// �ړ��n���ϐ�
	float fGravity;			// �d��
	D3DXVECTOR3 move;		// �ړ���
	SFluctFloat speedX;		// �ړ����x(X��)
	SFluctFloat speedY;		// �ړ����x(Y��)
	SFluctFloat speedZ;		// �ړ����x(Z��)

	// �F�n���ϐ�
	D3DXCOLOR col;		// ���_�J���[
	SFluctFloat colR;	// r�l
	SFluctFloat colG;	// g�l
	SFluctFloat colB;	// b�l
	SFluctFloat colA;	// a�l

	// ���S���n���ϐ�
	EFFECT_DIED died;		// ���S����
	EFFECT_ON_DIED onDied;	// ���S��


	// �A�j���[�V�����n���ϐ�
	int nCntAnim;		// �A�j���[�V�����J�E���^�[
	int nPatternAnim;	// ���ڂ̃A�j���[�V������
	int nDivisionU;		// �A�j���[�V�����̕�����(U)
	int nDivisionV;		// �A�j���[�V�����̕�����(V)
	int nDivisionMAX;	// �A�j���[�V�����̑�������

	int nPopCnt;			// ��x�ɏo���G�t�F�N�g��
	int nCountParticle;		// �p�[�e�B�N���̔����J�E���g
	bool bUse;				// �g�p���Ă��邩
	
	// �@�\��ON/OFF
	bool bIsBillboard;
	BLENDMODE blend;	// �u�����h���[�h

	//3D���g�p
	D3DXMATRIX mtxWorld;	// ���[���h�}�g���b�N�X
}Effect;

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
void InitEffect(void);
void UninitEffect(void);
void UpdateEffect(void);
void DrawEffect(void);
void SetEffect(D3DXVECTOR3 pos,EFFECT_TYPE type);

void SetRandom(FRandFloat* fluct, float* fValue);	// float�^�̗����K�p
void SetRandom(FRandInt* fluct, int* nValue);		// int�^�̗����K�p

#endif
