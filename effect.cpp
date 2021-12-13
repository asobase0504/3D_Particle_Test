//**************************************************************************************************
//
// �G�t�F�N�g�̐ݒ菈��
// Auther�F�������l
// Auther�FYuda Kaito
//
//**************************************************************************************************
//*******************************************************************************
// include
//*******************************************************************************
#include "effect.h"
#include "spawn_effect.h"
#include "LoadEffect.h"
#include "input.h"
#include "setup.h"
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

//-----------------------
// ���[�h�֌W
//-----------------------
void LoadFluct(FILE** file ,SFluctFloat* fluct);
void LoadFluct(FILE* file, SFluctInt* fluct);

//-----------------------
// �����f�[�^����
//-----------------------
void InitRandom(FRandFloat* inRandomFloat, float fValue);	// �����̏�����

//*******************************************************************************
// �ÓI�ϐ�
//*******************************************************************************
static LPDIRECT3DTEXTURE9 s_pTexture[MAX_EFFECT_TYPE_TEX] = {};		// �e�N�X�`���ւ̃|�C���^
static LPDIRECT3DVERTEXBUFFER9 s_pVtxBuff = {};						// ���_�o�b�t�@�ւ̃|�C���^
static Effect s_aEffect[MAX_EFFECT];								// �G�t�F�N�g�̏��

//-----------------------
// �e�N�X�`����
//-----------------------
static const char* pTexturename[] =
{
	NULL,							// �e�N�X�`���Ȃ�
	"data/TEXTURE/effect000.jpg",	// ��
	"data/TEXTURE/effect100.jpg",	// ���̊�
	"data/TEXTURE/effect101.jpg",	// ��d��
	"data/TEXTURE/effect102.jpg",	// �ׂ��l�p
	"data/TEXTURE/effect103.jpg",	// �O�p
};
// pTexture �� EFFECT_TYPE_TEX �̐����`�F�b�N
static_assert((sizeof(pTexturename) / sizeof(pTexturename[0])) == MAX_EFFECT_TYPE_TEX, "error �e�N�X�`���̐��Ɨ񋓌^�̐�����v���Ă܂���");

//********************************************************************************
// �G�t�F�N�g�̏���������
//********************************************************************************
void InitEffect(void)
{
	srand((unsigned int)time(0));	// �����̏�����

	// �f�o�C�X�ւ̃|�C���^
	LPDIRECT3DDEVICE9 pDevice = GetDevice();	// �f�o�C�X�̎擾

	// �|���S���ɒ���t����e�N�X�`���̓ǂݍ���
	for (int i = 0; i < MAX_EFFECT_TYPE_TEX; i++)
	{
		D3DXCreateTextureFromFile(pDevice
			,pTexturename[i]
			,&s_pTexture[i]);
	}

	// �G�t�F�N�g�̏���������
	ZeroMemory(s_aEffect, sizeof(s_aEffect));

	Effect* pEffect;	// �G�t�F�N�g�̃|�C���^�^

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 4 * MAX_EFFECT,		// �m�ۂ���o�b�t�@�T�C�Y
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&s_pVtxBuff,
		NULL);

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
//********************************************************************************
void UpdateEffect(void)
{
	Effect* pEffect;
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

		// �Ίp���̒������Z�o����
		pEffect->fLength = sqrtf(((pEffect->width.fValue * pEffect->width.fValue) + (pEffect->height.fValue * pEffect->height.fValue)) / 2.0f);
		// �Ίp���̊p�x���Z�o
		pEffect->fAngele = atan2f(pEffect->width.fValue, pEffect->height.fValue);
		
		// ���_�o�b�t�@�[�̐ݒ�
		VERTEX_3D *pVtx;	// ���_���ւ̃|�C���^�𐶐�

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

		// ���S�����𖞂����Ă��邩�`�F�b�N
		if (!(DiedCriteriaEffect(pEffect)))
		{
			// ���S����
			DiedEffect(pEffect);
		}

		// window��ʈȊO�ɏo���ꍇ�̏���
		OffScreenEffect(pEffect);
	}
}

//********************************************************************************
// �X�e�[�^�X�̕ύX����
//********************************************************************************
void StatChange(Effect * effect)
{
	// �Ίp���̒������Z�o����
	effect->fLength = sqrtf(((effect->width.fValue * effect->width.fValue) + (effect->height.fValue * effect->height.fValue)) / 2.0f);
	// �Ίp���̊p�x���Z�o
	effect->fAngele = atan2f(effect->width.fValue, effect->height.fValue);

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
//********************************************************************************
void MoveEffect(Effect * effect)
{
	// �ړ��̌���
	effect->move.x = sinf((D3DX_PI * effect->shotAngleZ.fValue) + effect->rot.z) * effect->speedX.fValue;
	effect->move.y = tanf((D3DX_PI * effect->shotAngleZ.fValue) + effect->rot.z) * effect->speedY.fValue;
	effect->move.z = cosf((D3DX_PI * effect->shotAngleZ.fValue) + effect->rot.z) * effect->speedZ.fValue;

	//for (int nHeight = 0; nHeight <= s_aMesh[0].nSurfaceHeight; nHeight++)
	//{
	//	float fRotHeight = D3DX_PI / s_aMesh[0].nSurfaceHeight;
	//	float fHeight = cosf(fRotHeight * nHeight) * s_aMesh[0].fLineHeight;
	//	for (int nWidth = 0; nWidth <= s_aMesh[0].nSurfaceWidth; nWidth++)
	//	{
	//		float fRotWidth = 2.0f * D3DX_PI / s_aMesh[0].nSurfaceWidth * nWidth;
	//		NormalizeRot(fRotWidth);

	//		effect->move.x = cosf(fRotWidth) * sinf(fRotHeight) * effect->speedX.fValue;
	//		effect->move.y = cosf(fRotHeight) * effect->speedX.fValue;
	//		effect->move.z = sinf(fRotWidth) * sinf(fRotHeight) * effect->speedX.fValue;

	//		effect->move.x = -sinf((D3DX_PI * nCntParticle / 10) - (s_aParticle[nCntParticle].fAngle / (5 * s_aParticle[nCntParticle].fAttenuation)));
	//		effect->move.y -= tanf(D3DX_PI) * s_aParticle[nCntParticle].fRadius / 2;
	//		effect->move.z = -cosf((D3DX_PI * nCntParticle / 10) - (s_aParticle[nCntParticle].fAngle / (5 * s_aParticle[nCntParticle].fAttenuation)));
	//	}
	//}

	if (effect->Injection == PARTICLMODE_CONVERGENCE)	// ���k
	{
		float fRotMove, fRotDest, fRotDiff;
		D3DXVECTOR3 PosDifference = D3DXVECTOR3(effect->spaen_pos.x - effect->pos.x, effect->spaen_pos.y - effect->pos.y, 0.0f);

		fRotMove = atan2f(effect->move.x, effect->move.y);		// ���݂̈ړ�����(�p�x)
		fRotDest = atan2f(PosDifference.x, PosDifference.y);	// �ړI�̈ړ�����
		fRotDiff = fRotDest - fRotMove;							// �ړI�̈ړ������܂ł̍���

		// �p�x�̐��K��
		if (fRotDiff > D3DX_PI)
		{// �ړI�̍��������~��(+����)�𒴂���ꍇ
			fRotDiff -= (D3DX_PI * 2);
		}
		else if (fRotDiff < -D3DX_PI)
		{// �ړI�̍��������~��(-����)�𒴂���ꍇ
			fRotDiff += (D3DX_PI * 2);
		}

		fRotMove += fRotDiff * 1.0f;	// �ړ�����(�p�x)�̕␳

		// �p�x�̐��K��
		if (fRotMove > D3DX_PI)
		{// �ړ�����(�p�x)�����~��(+����)�𒴂���ꍇ
			fRotMove -= (D3DX_PI * 2);
		}
		else if (fRotMove < -D3DX_PI)
		{// �ړ�����(�p�x)�����~��(-����)�𒴂���ꍇ
			fRotMove += (D3DX_PI * 2);
		}

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
//********************************************************************************
void DiedEffect(Effect * effect)
{

}

//********************************************************************************
// �G�t�F�N�g�̕`�揈��
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
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		
		// �A���t�@�e�X�g��L��
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

		if (pEffect->blend == BLENDMODE_ADDITION)
		{
			// ���u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);			// �u�����h�I�y���[�V����(�v�Z����)
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
		}
		else if (pEffect->blend == BLENDMODE_SUBTRACT)
		{
			// ���u�����f�B���O�����Z�����ɐݒ�
			pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
			pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
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
		s_aEffect[i] = LoadEffect(type);
#else	// �G�t�F�N�g�e�X�g���̂ݗL��
		*pEffect = *GetParticle();
#endif																	

		pEffect->pos = pos;			// �ʒu
		pEffect->spaen_pos = pos;	// �X�|�[���ʒu

		SetRandom(&pEffect->colR.initial, &pEffect->colR.fValue);				// Red�̏o�����̐��l
		SetRandom(&pEffect->colG.initial, &pEffect->colG.fValue);				// Green�̏o�����̐��l
		SetRandom(&pEffect->colB.initial, &pEffect->colB.fValue);				// Blue�̏o�����̐��l
		SetRandom(&pEffect->colA.initial, &pEffect->colA.fValue);				// Alpha�̏o�����̐��l
		SetRandom(&pEffect->speedX.initial, &(pEffect->speedX.fValue));			// �ړ���(X��)
		SetRandom(&pEffect->speedY.initial, &(pEffect->speedY.fValue));			// �ړ���(Y��)
		SetRandom(&pEffect->speedZ.initial, &(pEffect->speedZ.fValue));			// �ړ���(Z��)
		SetRandom(&pEffect->randLife.initial, &(pEffect->randLife.nValue));		// ����
		SetRandom(&pEffect->width.initial, &(pEffect->width.fValue));			// ��
		SetRandom(&pEffect->height.initial, &(pEffect->height.fValue));			// ����
		SetRandom(&pEffect->shotAngleX.initial, &(pEffect->shotAngleX.fValue));	// ���ˊp�x(X��)
		SetRandom(&pEffect->shotAngleY.initial, &(pEffect->shotAngleY.fValue));	// ���ˊp�x(Y��)
		SetRandom(&pEffect->shotAngleZ.initial, &(pEffect->shotAngleZ.fValue));	// ���ˊp�x(Z��)

		SetRandom(&pEffect->colR.Add, &pEffect->colR.fAddValue);				// Red�̏o�����̐��l
		SetRandom(&pEffect->colG.Add, &pEffect->colG.fAddValue);				// Green�̏o�����̐��l
		SetRandom(&pEffect->colB.Add, &pEffect->colB.fAddValue);				// Blue�̏o�����̐��l
		SetRandom(&pEffect->colA.Add, &pEffect->colA.fAddValue);				// Alpha�̏o�����̐��l
		SetRandom(&pEffect->speedX.Add, &(pEffect->speedX.fAddValue));			// �ړ���(X��)
		SetRandom(&pEffect->speedY.Add, &(pEffect->speedY.fAddValue));			// �ړ���(Y��)
		SetRandom(&pEffect->speedZ.Add, &(pEffect->speedZ.fAddValue));			// �ړ���(Z��)
		SetRandom(&pEffect->width.Add, &(pEffect->width.fAddValue));			// ��
		SetRandom(&pEffect->height.Add, &(pEffect->height.fAddValue));			// ����
		SetRandom(&pEffect->randLife.Add, &(pEffect->randLife.nValue));			// ����
		SetRandom(&pEffect->shotAngleX.Add, &(pEffect->shotAngleX.fAddValue));	// ���ˊp�x(X��)
		SetRandom(&pEffect->shotAngleY.Add, &(pEffect->shotAngleY.fAddValue));	// ���ˊp�x(Y��)
		SetRandom(&pEffect->shotAngleZ.Add, &(pEffect->shotAngleZ.fAddValue));	// ���ˊp�x(Z��)

		pEffect->nDivisionU = 1;
		pEffect->nDivisionV = 1;

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

		// �Ίp���̒������Z�o����
		pEffect->fLength = sqrtf(((pEffect->width.fValue * pEffect->width.fValue) + (pEffect->height.fValue * pEffect->height.fValue)) / 2.0f);
		// �Ίp���̊p�x���Z�o
		pEffect->fAngele = atan2f(pEffect->width.fValue, pEffect->height.fValue);

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
// SFluctFloat�̒l�̓ǂݍ���
//********************************************************************************
void LoadFluct(FILE** file, SFluctFloat* fluct)
{
}

//********************************************************************************
// SFluctInt�̒l�̓ǂݍ���
//********************************************************************************
void LoadFluct(FILE* file, SFluctInt* fluct)
{
}

//********************************************************************************
// SRandFloat�̏�����
//********************************************************************************
void InitRandom(FRandFloat* inRandomFloat, float fValue)
{
	inRandomFloat->bIsRandom = false;
}

//********************************************************************************
// SRandFloat�̗������ʂ𔽉f
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
//********************************************************************************
void SetRandom(FRandInt* fluct, int* nValue)
{
	if (fluct->bIsRandom)	// �������g�p����Ă���
	{
		*nValue = (rand() / RAND_MAX) * (fluct->nMax - fluct->nMin) + fluct->nMin;
	}
}