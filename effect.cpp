//**************************************************************************************************
//
// �G�t�F�N�g�̐ݒ菈��
// Auther�FKarasaki Yuuto
// Auther�FYuda Kaito
//
// ���l
//	�G�t�F�N�g�̏����̎厲.cpp�B
//	���l�̑�����A�C�x���g�̊Ǘ�����ɒS��
//	
//	2D���̊�Ղ𓂍肪�쐬�B
//	�O���ǂݍ��݂̊g��B�l�ύX�̎�ފg��B3D�̈ڐA�𓒓c���쐬�B
//	
//	��{�I�ɒl�͊O���t�@�C������ǂݍ��݂��厲�B
//	�O���t�@�C���̓ǂݍ��݂Ɋւ��Ă�LoadEffect.cpp���S���B
//
//**************************************************************************************************
//*******************************************************************************
// include
//*******************************************************************************
#include "effect.h"
#include "spawn_effect.h"
#include "LoadEffect.h"
#include "input.h"
#include "common.h"
#include <assert.h>
#include <time.h>

//*******************************************************************************
// �}�N����`
//*******************************************************************************
#define MAX_EFFECT	(4096)		// �G�t�F�N�g�̍ő吔

//*******************************************************************************
// �v���g�^�C�v�錾
//*******************************************************************************
//-----------------------
// �G�t�F�N�g����
//-----------------------
void StatChange(Effect* effect);			// ���l�̑�������
void AddStat(SFluctFloat* fluct);			// ���l�̉��Z����
void MoveEffect(Effect* effect);			// �ړ�����
bool DiedCriteriaEffect(Effect* effect);	// ���S��������
void DiedEffect(Effect* effect);			// ���S������
void OffScreenEffect(Effect* effect);		// ��ʊO����

//*******************************************************************************
// �ÓI�ϐ�
//*******************************************************************************
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_EFFECT_TYPE_TEX] = {};		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};						// ���_�o�b�t�@�ւ̃|�C���^
static Effect s_aEffect[MAX_EFFECT];								// �G�t�F�N�g�̏��
static Effect s_aTypeEffect[MAX_TYPE_EFFECT];						// �t�@�C����ǂݍ��񂾃G�t�F�N�g

//-----------------------
// �e�N�X�`����
//-----------------------
static char pTexturename[64][1024] = {};
static Effect pTypeEffect[64] = {};

//********************************************************************************
// �G�t�F�N�g�̏���������
// ���l
//	�������̎厲�B������LoadEffect�������Ă���\��B
//	�G�t�F�N�g�̃^�C�v���Ƃɕۊǂ���z����쐬�\��B
//********************************************************************************
void InitEffect(void)
{
	srand((unsigned int)time(0));	// �����̏�����

	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾
	
	FILE* pFile;

	char read[255] = {};
	int nCnt;

	// �t�@�C�����J��
	pFile = fopen("data/FILE/Load.txt", "r");

	// �e�N�X�`���̓ǂݍ���
	fscanf(pFile, "%s", &read);
	if (strcmp(&read[0], "TEXTURE") == 0)
	{
		fscanf(pFile, "%s", &read);
		fscanf(pFile, "%d", &nCnt);

		for (int i = 0; i < nCnt; i++)
		{
			fscanf(pFile, "%s", pTexturename[i]);
			D3DXCreateTextureFromFile(pDevice
				, &pTexturename[i][0]
				, &s_pTexture[i]);
		}
	}
	// �t�@�C���̓ǂݍ���
	fscanf(pFile, "%s", &read);
	if (strcmp(&read[0], "FILE") == 0)
	{
		fscanf(pFile, "%s", &read);
		fscanf(pFile, "%d", &nCnt);
		for (int i = 0; i < nCnt; i++)
		{
			pTypeEffect[i]  = LoadEffect((EFFECT_TYPE)i);
		}
	}

	// �G�t�F�N�g�̏���������
	ZeroMemory(s_aEffect, sizeof(s_aEffect));

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,		// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

	Effect* pEffect = NULL;	// �G�t�F�N�g�̃|�C���^�^

	// ���_���ւ̃|�C���^�𐶐�
	VERTEX_3D *pVtx;

	// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
	s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_�o�b�t�@�[�̐ݒ�
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		pEffect = &(s_aEffect[i]);

		// ���_���W�̐ݒ�
		pVtx[0].pos.x = sinf(pEffect->rot.z + (D3DX_PI + pEffect->fAngele)) * pEffect->fLength;
		pVtx[0].pos.y = cosf(pEffect->rot.z + (D3DX_PI + pEffect->fAngele)) * pEffect->fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = sinf(pEffect->rot.z + (D3DX_PI - pEffect->fAngele)) * pEffect->fLength;
		pVtx[1].pos.y = cosf(pEffect->rot.z + (D3DX_PI - pEffect->fAngele)) * pEffect->fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = sinf(pEffect->rot.z - (0 + pEffect->fAngele)) * pEffect->fLength;
		pVtx[2].pos.y = cosf(pEffect->rot.z - (0 + pEffect->fAngele)) * pEffect->fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = sinf(pEffect->rot.z - (0 - pEffect->fAngele)) * pEffect->fLength;
		pVtx[3].pos.y = cosf(pEffect->rot.z - (0 - pEffect->fAngele)) * pEffect->fLength;
		pVtx[3].pos.z = 0.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = pEffect->col;
		pVtx[1].col = pEffect->col;
		pVtx[2].col = pEffect->col;
		pVtx[3].col = pEffect->col;

		//�e���_�̖@���̐ݒ�@���@�x�N�g���̑傫����1�ɂ���K�v������
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[2].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[3].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// �e�N�X�`�����W
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;	// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N
	s_pVtxBuff->Unlock();
}

//********************************************************************************
// �G�t�F�N�g�̏I������
// ���l
// ���l
//	�I�������̎厲�B
//	���̂Ƃ���ύX�\��ӏ��Ȃ��B
//********************************************************************************
void UninitEffect(void)
{
	// �e�N�X�`���̔j��	 
	for (int i = 0; i < MAX_EFFECT_TYPE_TEX; i++)
	{
		if (s_pTexture[i] != NULL)
		{
			s_pTexture[i]->Release();

			s_pTexture[i] = NULL;
		}
	}

	// ���_�o�b�t�@��j��
	if (s_pVtxBuff != NULL)
	{
		s_pVtxBuff->Release();

		s_pVtxBuff = NULL;
	}
}

//********************************************************************************
// �G�t�F�N�g�̍X�V����
// ���l
//	�X�V�̎厲�B
//	�e�N�X�`���A�j���[�V�����Ɋւ��ẮA��Ŋ֐����Bcommon.cpp�Ɉڂ��\��B
//	�֐��݂̂ɂ���\��B
//********************************************************************************
void UpdateEffect(void)
{
	Effect* pEffect = NULL;
	for (int i = 0; i < MAX_EFFECT; i++)
	{
		pEffect = &(s_aEffect[i]);

		// �G�t�F�N�g���g�p����Ă��Ȃ��Ȃ�J��Ԃ������X�L�b�v
		if (!(pEffect->bUse))
		{
			continue;
		}

		StatChange(pEffect);	// �e��p�����[�^�̐ݒ�

		MoveEffect(pEffect);	// �ړ��̍X�V

		pEffect->speedY.fValue -= pEffect->fGravity;	// �d��

		pEffect->pos += pEffect->move;	// �ʒu�̍X�V

		// �Ίp���̒����Ɗp�x���Z�o����
		DiagonalLine(&pEffect->fLength, &pEffect->fAngele, pEffect->width.fValue, pEffect->height.fValue);

		// ���S�����̊m�F��A���S����
		if(!(DiedCriteriaEffect(pEffect)))
		{
			DiedEffect(pEffect);
		}
		// window��ʈȊO�ɏo���ꍇ�̏���
		OffScreenEffect(pEffect);

		// ���_�o�b�t�@�[�̐ݒ�
		VERTEX_3D *pVtx;

		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * i;

		// ���_���W�̐ݒ�
		pVtx[0].pos.x = sinf(pEffect->rot.z + (D3DX_PI + pEffect->fAngele)) * pEffect->fLength;
		pVtx[0].pos.y = cosf(pEffect->rot.z + (D3DX_PI + pEffect->fAngele)) * pEffect->fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = sinf(pEffect->rot.z + (D3DX_PI - pEffect->fAngele)) * pEffect->fLength;
		pVtx[1].pos.y = cosf(pEffect->rot.z + (D3DX_PI - pEffect->fAngele)) * pEffect->fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = sinf(pEffect->rot.z + -pEffect->fAngele) * pEffect->fLength;
		pVtx[2].pos.y = cosf(pEffect->rot.z + -pEffect->fAngele) * pEffect->fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = sinf(pEffect->rot.z + pEffect->fAngele) * pEffect->fLength;
		pVtx[3].pos.y = cosf(pEffect->rot.z + pEffect->fAngele) * pEffect->fLength;
		pVtx[3].pos.z = 0.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = pEffect->col;
		pVtx[1].col = pEffect->col;
		pVtx[2].col = pEffect->col;
		pVtx[3].col = pEffect->col;

		//�e�N�X�`���ݒ�
		pEffect->nCntAnim++;
		if ((pEffect->nCntAnim % pEffect->nDivisionMAX) == 0)
		{
			pEffect->nPatternAnim = (pEffect->nPatternAnim + 1) % pEffect->nDivisionMAX;

			float fLeft = 1.0f / pEffect->nDivisionU * (pEffect->nPatternAnim % pEffect->nDivisionU);
			float fRight = 1.0f / pEffect->nDivisionU *(pEffect->nPatternAnim % pEffect->nDivisionU) + 1.0f / pEffect->nDivisionU;
			float fUp = 1.0f / pEffect->nDivisionV * (pEffect->nPatternAnim / pEffect->nDivisionU);
			float fDown = 1.0f / pEffect->nDivisionV * (pEffect->nPatternAnim / pEffect->nDivisionU + 1.0f / pEffect->nDivisionV * pEffect->nDivisionV);

			pVtx[0].tex = D3DXVECTOR2(fLeft, fUp);
			pVtx[1].tex = D3DXVECTOR2(fRight, fUp);
			pVtx[2].tex = D3DXVECTOR2(fLeft, fDown);
			pVtx[3].tex = D3DXVECTOR2(fRight, fDown);
		}

		// ���_�o�b�t�@���A�����b�N
		s_pVtxBuff->Unlock();

	}
}

//********************************************************************************
// �X�e�[�^�X�̕ύX����
// �����P�@�ύX����G�t�F�N�g
// ���l
//	�����̕����������s�����C������B
//	�����Ɉڂ������͌�Ő�������B
//********************************************************************************
void StatChange(Effect * effect)
{
	// �Ίp���̒l���Z�o
	DiagonalLine(&effect->fLength,&effect->fAngele, effect->width.fValue, effect->height.fValue);

	effect->randLife.nValue--;	// ����

	AddStat(&(effect->rotZ));
	AddStat(&(effect->width));
	AddStat(&(effect->height));
	AddStat(&(effect->colR));
	AddStat(&(effect->colG));
	AddStat(&(effect->colB));
	AddStat(&(effect->colA));

	// �F�̐ݒ�
	effect->col = D3DXCOLOR(effect->colR.fValue, effect->colG.fValue, effect->colB.fValue, effect->colA.fValue);
}

//********************************************************************************
// �X�e�[�^�X�̉��Z����
// �����P�@���Z����l����Z�l
// ���l
//	�l�X�ȉ��Z�v�Z�����̏����ɂ܂Ƃ߂����B
//	�^�C�v���������\��B
//********************************************************************************
void AddStat(SFluctFloat* fluct)
{
	if (fluct->Add.bIsRandom)
	{
		fluct->fValue += fluct->fAddValue;
	}
}

//********************************************************************************
// �ړ�����
// �����P�@�ύX����G�t�F�N�g
// ���l
//	move�̌��肪�܂��������B�ˏo�p�x��Z�������l�����Ă��Ȃ��B�s������邩��
//	effect->Injection �̏������s�����A��������Ƌ@�\���Ȃ��B
//********************************************************************************
void MoveEffect(Effect * effect)
{
	// �ړ��̌���
	effect->move.x = sinf((D3DX_PI * effect->shotAngleZ.fValue) + effect->rot.z) * effect->speedX.fValue;
	effect->move.y = tanf((D3DX_PI * effect->shotAngleZ.fValue) + effect->rot.z) * effect->speedY.fValue;
	effect->move.z = cosf((D3DX_PI * effect->shotAngleZ.fValue) + effect->rot.z) * effect->speedZ.fValue;

	if (effect->Injection == PARTICLMODE_CONVERGENCE)	// ���k
	{
		float fRotMove, fRotDest, fRotDiff;
		D3DXVECTOR3 PosDifference(effect->posCenter.x - effect->pos.x, effect->posCenter.y - effect->pos.y, 0.0f);

		fRotMove = atan2f(effect->move.x, effect->move.y);		// ���݂̈ړ�����(�p�x)
		fRotDest = atan2f(PosDifference.x, PosDifference.y);	// �ړI�̈ړ�����
		fRotDiff = fRotDest - fRotMove;							// �ړI�̈ړ������܂ł̍���

		// �p�x�̐��K��
		NormalizeRot(&fRotDiff);

		fRotMove += fRotDiff * 1.0f;	// �ړ�����(�p�x)�̕␳

		// �p�x�̐��K��
		NormalizeRot(&fRotMove);

		// �ʒu�̍X�V
		effect->move.x = sinf(fRotMove) * effect->speedX.fValue;
		effect->move.y = cosf(fRotMove) * effect->speedX.fValue;
	}
	if (effect->Injection == PARTICLMODE_ROTATION)	// ���[�e�[�V����
	{
		effect->rot.z += D3DXToRadian(1);

		// �ړ��ʂ̎Z�o
		effect->move.x = sinf(effect->rot.z) * effect->speedX.fValue;
		effect->move.y = cosf(effect->rot.z) * effect->speedX.fValue;
	}

}

//********************************************************************************
// ���S��������
// �����P�@�ύX����G�t�F�N�g
// ���l
//	�l�X�ȏ��ł�������𕪂��Ă����B��{�A�����ŏ������邪�A�^�C�v��������B
//********************************************************************************
bool DiedCriteriaEffect(Effect* effect)
{
	if (effect->randLife.nValue <= 0)
	{// ������0�̎�
		effect->bUse = false;	// �g�p���ĂȂ�
	}
	return effect->bUse;
}

//********************************************************************************
// �G�t�F�N�g�̎��S����
// �����P�@�ύX����G�t�F�N�g
// ���l
//	���S���̏������܂Ƃ߂鏈���B�^�C�v�����͏I������B
//	ON_DIED_POP�ŃG�t�F�N�g����������Ȃ��B�C������
//********************************************************************************
void DiedEffect(Effect* effect)
{
	switch (effect->onDied)
	{
	case ON_DIED_NONE:
		break;
	case ON_DIED_POP:
		SetEffect(effect->pos, effect->onDiedPopType);
		break;
	case MAX_ON_DIED:
	default:
		assert(false);
		break;
	}
}

//********************************************************************************
// �G�t�F�N�g�̕`�揈��
// ���l
//	�`��̏����̎厲�B
//	�����_�[�X�e�[�g���ӂ̃R�[�h�������B���������B
//	RectDraw() �Ƃ��� common.cpp �̏�������߂āA�ʂ̏����ɕς������B
//	�t�s��͊֐��������邩�Y�ݒ�
//********************************************************************************
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	D3DXMATRIX mtxRot, mtxTrans;	// �v�Z�p�}�g���b�N�X

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		// �p�[�e�B�N�����̎擾
		Effect *pEffect = &(s_aEffect[i]);

		if (!(pEffect->bUse))
		{
			continue;
		}
		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&pEffect->mtxWorld);	// �s�񏉊����֐�(��1�����̍s���P�ʍs��ɏ�����)

		D3DXMATRIX mtxView;
		pDevice->GetTransform(D3DTS_VIEW, &mtxView);

		// �J�����̋t�s���ݒ�
		if (pEffect->bIsBillboard)
		{
			pEffect->mtxWorld._11 = mtxView._11;
			pEffect->mtxWorld._12 = mtxView._21;
			pEffect->mtxWorld._13 = mtxView._31;
			pEffect->mtxWorld._21 = mtxView._12;
			pEffect->mtxWorld._22 = mtxView._22;
			pEffect->mtxWorld._23 = mtxView._32;
			pEffect->mtxWorld._31 = mtxView._13;
			pEffect->mtxWorld._32 = mtxView._23;
			pEffect->mtxWorld._33 = mtxView._33;
		}
		else
		{
			// �����𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, pEffect->rot.y, pEffect->rot.x, pEffect->rot.z);	// �s���]�֐�(��1�����Ƀ��[(y)�s�b�`(x)���[��(z)�����̉�]�s����쐬)
			D3DXMatrixMultiply(&pEffect->mtxWorld, &pEffect->mtxWorld, &mtxRot);						// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)
		}
																								// �ʒu�𔽉f
		D3DXMatrixTranslation(&mtxTrans, pEffect->pos.x, pEffect->pos.y, pEffect->pos.z);		// �s��ړ��֐�(��P������X,Y,Z�����̈ړ��s����쐬)
		D3DXMatrixMultiply(&pEffect->mtxWorld, &pEffect->mtxWorld, &mtxTrans);					// �s��|���Z�֐�(��2�����~��3��������P�����Ɋi�[)

		// ���[���h�}�g���b�N�X�̐ݒ�
		pDevice->SetTransform(D3DTS_WORLD, &pEffect->mtxWorld);	// ���[���h���W�s��̐ݒ�

		// ���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, s_pVtxBuff, 0, sizeof(VERTEX_3D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_3D);

		// ���C�g�𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, false);

		// Z�o�b�t�@�̏㏑���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ZENABLE, false);

		// �J�����O�̐ݒ�
		if (pEffect->bIsCulling)
		{
			pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		}

		// �A���t�@�e�X�g��L��
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		switch (pEffect->blend)
		{
		case BLENDMODE_ADDITION:
			// ���u�����f�B���O�����Z�����ɐݒ�
			AddSynthesis(pDevice);
			break;
		case BLENDMODE_SUBTRACT:
			// ���u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
			break;
		default:
			break;
		}

		// �`��
		RectDraw(pDevice, s_pTexture[pEffect->tex], i * 4);

		// ���C�g��L���ɂ���
		pDevice->SetRenderState(D3DRS_LIGHTING, true);

		// Z�o�b�t�@�̏㏑���𖳌��ɂ���
		pDevice->SetRenderState(D3DRS_ZENABLE, true);

		// �J�����O�̐ݒ�
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		// �A���t�@�e�X�g��L��
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 1);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);

		// �e�N�X�`���̉���
		pDevice->SetTexture(0, NULL);

	}
	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//********************************************************************************
// �G�t�F�N�g�̐ݒ菈��
// �����P�@�G�t�F�N�g���o������ʒu
// �����Q�@�G�t�F�N�g�̎��
// ���l
//	���A�����Ńt�@�C���ǂݍ��݂����Ă��邪�A�������œǂݍ��݂����B
//	�����̒l�ݒ���������Y��ȏ����ɐ؂�ւ������B
//	���_�o�b�t�@�[�̐ݒ�����C���G�t�F�N�g�̎��ɁA�؂�ւ�����悤�ɂȂ肽���B
//********************************************************************************
void SetEffect(D3DXVECTOR3 pos,EFFECT_TYPE type)
{
	// �ϐ��錾
	Effect* pEffect;	// �O���[�o���ϐ����|�C���^�^�ɕϊ��p

	for (int i = 0; i < MAX_EFFECT; i++)
	{
		pEffect = &(s_aEffect[i]);	// �|�C���^�^�ɕϊ�

		if (pEffect->bUse)
		{
			continue;
		}

#ifndef TEST_MODE	// �e�X�g�����ۂ�
		// �t�@�C���̓ǂݍ���
		s_aEffect[i] = pTypeEffect[type];
#else	// �G�t�F�N�g�e�X�g���̂ݗL��
		*pEffect = *GetParticle();
#endif																	

		pEffect->posCenter = pos;	// �X�|�[���ʒu
		pEffect->pos = pos;			// �ʒu
		// �ʒu���������炷
		if (pEffect->bIsPosRand)
		{
			pEffect->pos.x += rand() % (int)(pEffect->posRand.x + 1) - pEffect->posRand.x;
			pEffect->pos.y += rand() % (int)(pEffect->posRand.y + 1) - pEffect->posRand.y;
			pEffect->pos.z += rand() % (int)(pEffect->posRand.z + 1) - pEffect->posRand.z;
		}
											   
		SetRandom(&pEffect->colR.initial, &pEffect->colR.fValue);				// Red�̏o�����̐��l
		SetRandom(&pEffect->colG.initial, &pEffect->colG.fValue);				// Green�̏o�����̐��l
		SetRandom(&pEffect->colB.initial, &pEffect->colB.fValue);				// Blue�̏o�����̐��l
		SetRandom(&pEffect->colA.initial, &pEffect->colA.fValue);				// Alpha�̏o�����̐��l
		SetRandom(&pEffect->speedX.initial, &pEffect->speedX.fValue);			// �ړ���(X��)
		SetRandom(&pEffect->speedY.initial, &pEffect->speedY.fValue);			// �ړ���(Y��)
		SetRandom(&pEffect->speedZ.initial, &pEffect->speedZ.fValue);			// �ړ���(Z��)
		SetRandom(&pEffect->randLife.initial, &pEffect->randLife.nValue);		// ����
		SetRandom(&pEffect->width.initial, &pEffect->width.fValue);				// ��
		SetRandom(&pEffect->height.initial, &pEffect->height.fValue);			// ����
		SetRandom(&pEffect->shotAngleX.initial, &(pEffect->shotAngleX.fValue));	// ���ˊp�x(X��)
		SetRandom(&pEffect->shotAngleY.initial, &(pEffect->shotAngleY.fValue));	// ���ˊp�x(Y��)
		SetRandom(&pEffect->shotAngleZ.initial, &(pEffect->shotAngleZ.fValue));	// ���ˊp�x(Z��)

		SetRandom(&pEffect->colR.Add, &pEffect->colR.fAddValue);				// Red�̏o�����̐��l
		SetRandom(&pEffect->colG.Add, &pEffect->colG.fAddValue);				// Green�̏o�����̐��l
		SetRandom(&pEffect->colB.Add, &pEffect->colB.fAddValue);				// Blue�̏o�����̐��l
		SetRandom(&pEffect->colA.Add, &pEffect->colA.fAddValue);				// Alpha�̏o�����̐��l
		SetRandom(&pEffect->speedX.Add, &pEffect->speedX.fAddValue);			// �ړ���(X��)
		SetRandom(&pEffect->speedY.Add, &pEffect->speedY.fAddValue);			// �ړ���(Y��)
		SetRandom(&pEffect->speedZ.Add, &pEffect->speedZ.fAddValue);			// �ړ���(Z��)
		SetRandom(&pEffect->width.Add, &pEffect->width.fAddValue);				// ��
		SetRandom(&pEffect->height.Add, &pEffect->height.fAddValue);			// ����
		SetRandom(&pEffect->randLife.Add, &pEffect->randLife.nValue);			// ����
		SetRandom(&pEffect->shotAngleX.Add, &pEffect->shotAngleX.fAddValue);	// ���ˊp�x(X��)
		SetRandom(&pEffect->shotAngleY.Add, &pEffect->shotAngleY.fAddValue);	// ���ˊp�x(Y��)
		SetRandom(&pEffect->shotAngleZ.Add, &pEffect->shotAngleZ.fAddValue);	// ���ˊp�x(Z��)

		pEffect->nDivisionMAX = pEffect->nDivisionU * pEffect->nDivisionV;	// �������̌v�Z

		// �p�x�̑��
		pEffect->shotAngle.x = pEffect->shotAngleX.fValue;
		pEffect->shotAngle.y = pEffect->shotAngleY.fValue;
		pEffect->shotAngle.z = pEffect->shotAngleZ.fValue;

		// �F�̑��
		pEffect->col.r = pEffect->colR.fValue;
		pEffect->col.g = pEffect->colG.fValue;
		pEffect->col.b = pEffect->colB.fValue;
		pEffect->col.a = pEffect->colA.fValue;

		// �Ίp���̒l���Z�o
		DiagonalLine(&pEffect->fLength, &pEffect->fAngele, pEffect->width.fValue, pEffect->height.fValue);

		// ���_���ւ̃|�C���^�𐶐�
		VERTEX_3D *pVtx;
		// ���_�o�b�t�@�����b�N���A���_���ւ̃|�C���^���擾
		s_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		pVtx += 4 * i;	// ���_�f�[�^�̃|�C���^��4���i�߂�
		
		// ���_���W�̐ݒ�
		pVtx[0].pos.x = sinf(pEffect->rot.z + (D3DX_PI + pEffect->fAngele)) * pEffect->fLength;
		pVtx[0].pos.y = cosf(pEffect->rot.z + (D3DX_PI + pEffect->fAngele)) * pEffect->fLength;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = sinf(pEffect->rot.z + (D3DX_PI - pEffect->fAngele)) * pEffect->fLength;
		pVtx[1].pos.y = cosf(pEffect->rot.z + (D3DX_PI - pEffect->fAngele)) * pEffect->fLength;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = sinf(pEffect->rot.z + -pEffect->fAngele) * pEffect->fLength;
		pVtx[2].pos.y = cosf(pEffect->rot.z + -pEffect->fAngele) * pEffect->fLength;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = sinf(pEffect->rot.z + pEffect->fAngele) * pEffect->fLength;
		pVtx[3].pos.y = cosf(pEffect->rot.z + pEffect->fAngele) * pEffect->fLength;
		pVtx[3].pos.z = 0.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = pEffect->col;
		pVtx[1].col = pEffect->col;
		pVtx[2].col = pEffect->col;
		pVtx[3].col = pEffect->col;

		// ���_�o�b�t�@���A�����b�N
		s_pVtxBuff->Unlock();

		pEffect->bUse = true;				// �g�p���Ă���
		break;
	}
}

//********************************************************************************
// �G�t�F�N�g�̉�ʊO����
// �����P�@�ύX����G�t�F�N�g
// ���l
//	2D�̎��̂̂Ȃ���B�ʂ����Ďg����������Ȃ��̂ňꉞ�c���Ă���B
//********************************************************************************
void OffScreenEffect(Effect* effect)
{
	//// window��ʈȊO�ɏo���ꍇ�̏���
	//if (effect->pos.y + effect->randRadius.fValue >= SCREEN_HEIGHT)
	//{// Y���W�̈ړ������𔽓]
	//	effect->rot.z = D3DX_PI + effect->rot.z * -0.8f;
	//	effect->move.y *= -0.8f;
	//}
	//else if (effect->pos.x - effect->randRadius.fValue < 0 || effect->pos.x + effect->randRadius.fValue > SCREEN_WIDH)
	//{// X���W�̈ړ������𔽓]
	//	effect->rot.z *= -1.0f;
	//	effect->move.x *= -1.0f;
	//}
}

//********************************************************************************
// SRandFloat�̗������ʂ𔽉f
// �����P�@�������ݒ�o���� float�^
// �����Q�@�l
// ���l
//	�������g�p����ꍇ�́A�����Q�̒l�𗐐��̒l�ɕύX����B
//	�������g�p���Ȃ��ꍇ�́A�����l���ύX���Ȃ��B
//********************************************************************************
void SetRandom(FRandFloat* fluct,float* fValue)
{
	if (fluct->bIsRandom)	// �������g�p����Ă���
	{
		*fValue = (rand() / (float)RAND_MAX) * (fluct->fMax - fluct->fMin) + fluct->fMin;
	}
}

//********************************************************************************
// SRandInt�̗������ʂ𔽉f
// �����P�@�������ݒ�o���� int�^
// �����Q�@�l
// ���l
//	�������g�p����ꍇ�́A�����Q�̒l�𗐐��̒l�ɕύX����B
//	�������g�p���Ȃ��ꍇ�́A�����l���ύX���Ȃ��B
//********************************************************************************
void SetRandom(FRandInt* fluct, int* nValue)
{
	if (fluct->bIsRandom)	// �������g�p����Ă���
	{
		*nValue = (int)((rand() / (float)RAND_MAX) * (fluct->nMax - fluct->nMin)) + fluct->nMin;
	}
}
