//**************************************************************************************************
//
// �G�t�F�N�g�̐ݒ菈��
// Auther�FKarasaki Yuuto
// Auther�FYuda Kaito
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
// �}�N����`
//************************************************************

//************************************************************
// �v���g�^�C�v�錾
//************************************************************

static void ConfigSpawnPoint(void);

static void SelectModeChange(void);			// ���[�h�`�F���W
static void RandModeChange(void);			// �������A�������[�h�̐؂�ւ�
static void InjectionModeChange(void);		// �������A�ˏo���[�h�̐؂�ւ�
static void BlendModeChange(void);			// �������A�u�����h���[�h�̐؂�ւ�
static void TestParticle(void);				// ���݂̂̏o���̃e�X�g
static void MovingLimit(void);				// �ړ�����

//*******************************************************************************
// static �ϐ�
//*******************************************************************************
static SpawnPoint g_aSpawnPoint[MAX_PARTICLE_CATEGORY];		// �X�|�[���G�t�F�N�g�̏��
static Effect g_aParticle[MAX_PARTICLE_CATEGORY];			// �p�[�e�B�N�����
static int s_nPointCnt = -1;								// �f�[�^�؂�ւ��̕ϐ�
static int g_nMaxSwitch = 0;								// �؂�ւ��\�ȃf�[�^�̐�
static int g_nMode = 0;										// ���[�h�̐؂�ւ�
static int s_nAdjustInfo = 0;								// �؂�ւ��f�[�^�̕ύX

//********************************************************************************
// �G�t�F�N�g�̏���������
//********************************************************************************
void InitSpawnEffect(void)
{
	// �����̏�����
	srand((unsigned int)time(0));

	// �X�|�[���G�t�F�N�g�̊e���l�̏�����
	ZeroMemory(g_aSpawnPoint, sizeof(g_aSpawnPoint));
	// �p�[�e�B�N���̊e���l�̏�����
	ZeroMemory(g_aParticle, sizeof(g_aParticle));
}

//********************************************************************************
// �G�t�F�N�g�̏I������
//********************************************************************************
void UninitSpawnEffect(void)
{
}

//********************************************************************************
// �G�t�F�N�g�̍X�V����
//********************************************************************************
void UpdateSpawnEffect(void)
{
	// ���[�h�̐؂�ւ��ϐ�
	if (GetKeyboardTrigger(DIK_F9))
	{// [T]�L�[�������ꂽ�Ƃ�
		g_nMode++;

		if (g_nMode >= 2)
		{// 2�ȏ�̎�
			g_nMode = 0;
		}
	}

#ifndef TEST_MODE
	SetEffect(D3DXVECTOR3(SCREEN_WIDH / 2.0f, SCREEN_HEIGHT / 2.0f, 0.0f), TYPE01_EFFECT);
#else
	// ����Particle��؂�ւ���
	if (s_nPointCnt != -1)
	{
		int nNextParticle = s_nPointCnt;	// �؂�ւ��̒l��ۑ�

		if (GetKeyboardTrigger(DIK_TAB))
		{// [TAB]�L�[�������ꂽ�Ƃ�
			nNextParticle++;
		}
		else if (GetKeyboardTrigger(DIK_LSHIFT))
		{// [LSHIFT]�L�[�������ꂽ�Ƃ�
			nNextParticle += g_nMaxSwitch - 1;
		}

		nNextParticle %= g_nMaxSwitch;
		s_nPointCnt = nNextParticle;
	}

	if (g_nMode == 0)
	{// ���[�h��0�̎�)
	 // �G�t�F�N�g���̐ݒ�
		AdjustCountSpawnEffect();

		Effect* pParticle = &(g_aParticle[s_nPointCnt]);
		// �G�t�F�N�g�̐ݒ�
		if (g_aSpawnPoint[s_nPointCnt].bUse && pParticle->bUse)
		{
			// �ҏW�������ύX
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
				// ��x�ɏo������G�t�F�N�g��
				AdjustOneShotEffect(pParticle);
				break;
			case ADJUST_INFO_ROT:
				// �G�t�F�N�g�̊p�x��������
				AdjustRotEffect(pParticle);
				break;
			case ADJUST_INFO_SPREAD:
				// �G�t�F�N�g�̔��ˊp�x��������
				AdjustSpreadEffect(pParticle);
				break;
			case ADJUST_INFO_MOVE:
				// �G�t�F�N�g�̈ړ��ʒ�������
				AdjustMoveEffect(pParticle);
				break;
			case ADJUST_INFO_RADIUS:
				// �G�t�F�N�g�̔��a��������
				AdjustRadiusEffect(pParticle);
				break;
			case ADJUST_INFO_LIFE:
				// �G�t�F�N�g�̎�����������
				AdjustLifeEffect(pParticle);
				break;
			case ADJUST_INFO_COLR:
				// �G�t�F�N�g�̐F(Red)��������
				AdjustSFluctFloat(&(pParticle->colR), 0.01f, 1.0f, 0.0f);
				break;											  
			case ADJUST_INFO_COLG:								  
				// �G�t�F�N�g�̐F(Green)��������				
				AdjustSFluctFloat(&(pParticle->colG), 0.01f, 1.0f, 0.0f);
				break;
			case ADJUST_INFO_COLB:
				// �G�t�F�N�g�̐F(Blue)��������
				AdjustSFluctFloat(&(pParticle->colB), 0.01f, 1.0f, 0.0f);
				break;
			case ADJUST_INFO_COLA:
				// �G�t�F�N�g�̐F(Alpha)��������
				AdjustSFluctFloat(&(pParticle->colA), 0.01f, 1.0f, 0.0f);
				break;
			case ADJUST_INFO_GRAVITY:
				// �G�t�F�N�g�̏d�͂̒���
				AdjustGravityEffect(pParticle);
				break;
			default:
				break;
			}

			// �������[�h�̐؂�ւ�
			RandModeChange();
			// �ˏo���[�h�̐؂�ւ�
			InjectionModeChange();
			// �u�����h���[�h�̐؂�ւ�
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
					// �G�t�F�N�g�̐ݒ�
					SetEffect(D3DXVECTOR3(0.0f,0.0f,0.0f), TEST_TYPE_EFFECT);
				}
			}

			// �X�|�[���G�t�F�N�g�̐ݒ�
			ConfigSpawnPoint();
		}
	}
	if (g_nMode == 1)
	{
		TestParticle();
	}

	// �f�[�^�̕ۑ�
	if (GetKeyboardTrigger(DIK_F3))
	{// [F3]�L�[�������ꂽ�Ƃ�
		SaveEffect(g_aParticle[s_nPointCnt]);
	}
#endif // !TEAT_MODE
}

//********************************************************************************
// �X�|�[���G�t�F�N�g�̍X�V����
//********************************************************************************
void ConfigSpawnPoint(void)
{
	// �G�t�F�N�g�̈ړ�����
	MoveSpawnEffect();

	// SpawnPoint���|�C���^�^�ɕϊ�
	SpawnPoint* SpawnPoint = &(g_aSpawnPoint[s_nPointCnt]);

	// �ړ��ʂ̍X�V
	SpawnPoint->pos += SpawnPoint->move;

	// �ړ��ʂ̍X�V(����������)
	SpawnPoint->move.x += (0.0f - SpawnPoint->move.x) * DAMPING_COEFFICIENT;
	SpawnPoint->move.y += (0.0f - SpawnPoint->move.y) * DAMPING_COEFFICIENT;

	// �o���ʒu�̃X�N���[������
	MovingLimit();

}

//********************************************************************************
// �X�|�[���G�t�F�N�g�̐��̒�������
//********************************************************************************
void AdjustCountSpawnEffect(void)
{
	Effect* pParticle = &(g_aParticle[s_nPointCnt]);
	if (GetKeyboardTrigger(DIK_0) && !(g_nMaxSwitch >= MAX_PARTICLE_CATEGORY))
	{// [0]�L�[�������ꂽ�Ƃ�
		SetSpawnEffect();

		g_nMaxSwitch++;
		s_nPointCnt = g_nMaxSwitch - 1;
		pParticle->Injection = (INJECTIONMODE)0;
	}
	else if (GetKeyboardTrigger(DIK_9) && g_nMaxSwitch >= 0)
	{// [9]�L�[�������ꂽ�Ƃ�
		g_aSpawnPoint[g_nMaxSwitch - 1].bUse = false;	// �g�p���Ă��Ȃ�
		g_aParticle[g_nMaxSwitch - 1].bUse = false;		// �g�p���Ă��Ȃ�

		g_nMaxSwitch--;
		s_nPointCnt = g_nMaxSwitch - 1;
		pParticle->Injection = (INJECTIONMODE)0;
	}
}

//********************************************************************************
// �X�|�[���G�t�F�N�g�̈ړ�����
//********************************************************************************
void MoveSpawnEffect(void)
{
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	if (GetKeyboardPress(DIK_A))
	{	// [A]�L�[�������ꂽ�Ƃ�
		// �ʒu�̍X�V
		vec.x += sinf(D3DX_PI * -0.5f);
		vec.y += cosf(D3DX_PI * -0.5f);
	}
	if (GetKeyboardPress(DIK_D))
	{	// [D]�L�[�������ꂽ�Ƃ�
		// �ʒu�̍X�V
		vec.x += sinf(D3DX_PI * 0.5f);
		vec.y += cosf(D3DX_PI * 0.5f);
	}
	if (GetKeyboardPress(DIK_W))
	{	// [W]�L�[�������ꂽ�Ƃ�
		// �ʒu�̍X�V
		vec.x += sinf(D3DX_PI);
		vec.y += cosf(D3DX_PI);
	}

	if (GetKeyboardPress(DIK_S))
	{	// [S]�L�[�������ꂽ�Ƃ�
		// �ʒu�̍X�V
		vec.x += sinf(D3DX_PI - D3DX_PI);
		vec.y += cosf(D3DX_PI - D3DX_PI);
	}

	// �x�N�g�������Z����Ă�����
	if (!(D3DXVec3Length(&vec) == 0.0f))
	{
		D3DXVec3Normalize(&vec, &vec);	// ���K������(�傫���P�̃x�N�g���ɂ���)
		g_aSpawnPoint[s_nPointCnt].move += vec *  g_aSpawnPoint[s_nPointCnt].fSpeed;
	}
}

//********************************************************************************
// �������[�h��؂�ւ�������
//********************************************************************************
void RandModeChange(void)
{
	Effect* pParticle = &(g_aParticle[s_nPointCnt]);

	if (GetKeyboardTrigger(DIK_G))
	{// [G]�L�[�������ꂽ�Ƃ�
	}
}

//********************************************************************************
// �ˏo���[�h��؂�ւ�������
//********************************************************************************
void InjectionModeChange(void)
{
	if (GetKeyboardTrigger(DIK_F4))
	{// [H]�L�[�������ꂽ�Ƃ�
		Effect* pParticle = &(g_aParticle[s_nPointCnt]);

		pParticle->Injection = (INJECTIONMODE)(pParticle->Injection + 1);

		if (pParticle->Injection >= MAX_PARTICLMODE)
		{// �������Ă���
			pParticle->Injection = (INJECTIONMODE)0;
		}

#if 0
		int nextMode;
		nextMode = pParticle->Injection + 1; // �ӂ₷�Ƃ��B
		nextMode = pParticle->Injection + MAX_PARTICLMODE - 1; // �ւ炷�Ƃ��B
		nextMode %= MAX_PARTICLMODE;
		pParticle->Injection = (INJECTIONMODE)nextMode;
#endif
	}
}

//********************************************************************************
// �u�����h���[�h��؂�ւ�������
//********************************************************************************
void BlendModeChange(void)
{
	// �u�����h���[�h�̐؂�ւ�
	if (GetKeyboardTrigger(DIK_F2))
	{// [Y]�L�[�������ꂽ�Ƃ�
		Effect* pParticle = &(g_aParticle[s_nPointCnt]);

		pParticle->blend = (BLENDMODE)(pParticle->blend + 1);

		if (pParticle->blend >= MAX_BLENDMODE)
		{// ���Z����
			pParticle->blend = (BLENDMODE)0;
		}
	}
}

//********************************************************************************
// ��x���݂̏o���e�X�g
//********************************************************************************
void TestParticle(void)
{
	if (GetKeyboardPress(DIK_DOWN))
	{// [��]�L�[�������ꂽ�Ƃ�
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
	{// [��]�L�[�������ꂽ�Ƃ�
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
	{// [Enter]�L�[�������ꂽ�Ƃ�
		for (int nCntParticle = 0; nCntParticle < MAX_PARTICLE_CATEGORY; nCntParticle++)
		{
			if (g_aParticle[nCntParticle].bUse)
			{
				for (int nCountEffect = 0; nCountEffect < g_aParticle[nCntParticle].nPopCnt; nCountEffect++)
				{// Particle�̐ݒ�
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
// �o���ʒu�̈ړ�����
//********************************************************************************
void MovingLimit(void)
{
	if (g_aSpawnPoint[s_nPointCnt].pos.x < 0.0f + POS_RAND_X)
	{
		g_aSpawnPoint[s_nPointCnt].pos.x = 0.0f + POS_RAND_X;

		// �ړ��ʂ̍X�V
		g_aSpawnPoint[s_nPointCnt].move.x = 0.0f;
		g_aSpawnPoint[s_nPointCnt].move.y = 0.0f;
	}
	else if (g_aSpawnPoint[s_nPointCnt].pos.x > SCREEN_WIDTH - POS_RAND_X)
	{
		g_aSpawnPoint[s_nPointCnt].pos.x = SCREEN_WIDTH - POS_RAND_X;

		// �ړ��ʂ̍X�V
		g_aSpawnPoint[s_nPointCnt].move.x = 0.0f;
		g_aSpawnPoint[s_nPointCnt].move.y = 0.0f;
	}
	else if (g_aSpawnPoint[s_nPointCnt].pos.y < 0.0f + POS_RAND_Y)
	{
		g_aSpawnPoint[s_nPointCnt].pos.y = 0.0f + POS_RAND_Y;

		// �ړ��ʂ̍X�V
		g_aSpawnPoint[s_nPointCnt].move.x = 0.0f;
		g_aSpawnPoint[s_nPointCnt].move.y = 0.0f;
	}
	else if (g_aSpawnPoint[s_nPointCnt].pos.y > SCREEN_HEIGHT - POS_RAND_Y)
	{
		g_aSpawnPoint[s_nPointCnt].pos.y = SCREEN_HEIGHT - POS_RAND_Y;

		// �ړ��ʂ̍X�V
		g_aSpawnPoint[s_nPointCnt].move.x = 0.0f;
		g_aSpawnPoint[s_nPointCnt].move.y = 0.0f;
	}
}

//********************************************************************************
// �G�t�F�N�g�̈�x�̏o���ɏo�����鐔�̒���
//********************************************************************************
void AdjustPopEffect(void)
{
	if (GetKeyboardPress(DIK_DOWN))
	{// [��]�L�[�������ꂽ�Ƃ�
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
	{// [��]�L�[�������ꂽ�Ƃ�
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
// �G�t�F�N�g�̕`�揈��
//********************************************************************************
void DrawSpawnEffect(void)
{

}

//********************************************************************************
// �G�t�F�N�g�̐ݒ菈��
//********************************************************************************
void SetSpawnEffect(void)
{
	// �ϐ��錾
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

		// �X�|�[���G�t�F�N�g�̊e���l�̏�����
		pSpawnEffect->pos = D3DXVECTOR3(POS_SPAWN_EFFECT_X, POS_SPAWN_EFFECT_Y, 0.0f);	//�ʒu
		pSpawnEffect->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);								//�ړ���
		pSpawnEffect->fSpeed = SPEED_SPAWN_EFFECT;										//�ړ����x
		pSpawnEffect->bUse = true;														// �g�p���Ă���

		//�p�[�e�B�N�����̏�����
		g_aParticle[nCntParticle] = LoadEffect(TEST_TYPE_EFFECT);

		SetRandom(&pParticle->colR.initial, &pParticle->colR.fValue);				// Red�̏o�����̐��l
		SetRandom(&pParticle->colG.initial, &pParticle->colG.fValue);				// Green�̏o�����̐��l
		SetRandom(&pParticle->colB.initial, &pParticle->colB.fValue);				// Blue�̏o�����̐��l
		SetRandom(&pParticle->colA.initial, &pParticle->colA.fValue);				// Alpha�̏o�����̐��l
		SetRandom(&pParticle->speedX.initial, &pParticle->speedX.fValue);
		SetRandom(&pParticle->width.initial, &pParticle->width.fValue);
		SetRandom(&pParticle->height.initial, &pParticle->height.fValue);
		SetRandom(&pParticle->shotAngleZ.initial, &pParticle->shotAngleZ.fValue);
		//SetRandom(&pParticle->randLife, &pParticle->randLife.fValue);
		pParticle->col.r = pParticle->colR.fValue;
		pParticle->col.g = pParticle->colG.fValue;
		pParticle->col.b = pParticle->colB.fValue;
		pParticle->col.a = pParticle->colA.fValue;
		pParticle->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// �ʒu
		pParticle->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);							// ����
		pParticle->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);						// �ړ���
		pParticle->Injection = PARTICLMODE_DIVERGENT;							// ���[�h
		pParticle->blend = BLENDMODE_ADDITION;									// �u�����h���[�h

		pParticle->nPopCnt = 1;					// �G�t�F�N�g�̍ő吔
		pParticle->nCountParticle = 60;			// �p�[�e�B�N���̔����J�E���g
		pParticle->bUse = true;					// �g�p���Ă���

		break;
	}
}

//********************************************************************************
// �p�[�e�B�N���̏��擾
//********************************************************************************
Effect *GetParticle(void)
{
	return &g_aParticle[0];
}

//********************************************************************************
// �X�|�[���p�[�e�B�N���̏��擾
//********************************************************************************
SpawnPoint *GetSpawnEffect(void)
{
	return &g_aSpawnPoint[0];
}

//********************************************************************************
// �����p�[�e�B�N�����̏��擾
//********************************************************************************
int GetMaxSwitch(void)
{
	return g_nMaxSwitch;
}

//********************************************************************************
// �����p�[�e�B�N�����̏��擾
//********************************************************************************
int GetSwitch(void)
{
	return s_nPointCnt;
}

//********************************************************************************
// ���݂̃��[�h�̏��擾
//********************************************************************************
int GetMode(void)
{
	return g_nMode;
}

//********************************************************************************
// �������鑮���̏��擾
//********************************************************************************
int GetAdjustInfo(void)
{
	return s_nAdjustInfo;
}