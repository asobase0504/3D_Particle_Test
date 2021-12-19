//=========================================
// 
// ���ʏ����̐ݒ�
// �v���W�F�N�g�S�̂Ŏg�p���鏈��
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "common.h"

//=========================================
// �p�x�̐��K��
//=========================================
void NormalizeRot(float* rot)
{
	if (*rot > D3DX_PI)
	{
		*rot -= D3DX_PI * 2;
	}
	if (*rot < -D3DX_PI)
	{
		*rot += D3DX_PI * 2;
	}
}

//=========================================
// �e�N�X�`���A�j���[�V��������
//=========================================
void TexAnim(VERTEX_3D *vtx,int U, int V, int Pattern)
{

	float fLeft = 1.0f / U * (Pattern % U);
	float fRight = 1.0f / U *(Pattern % U) + 1.0f / U;
	float fUp = 1.0f / V * (Pattern / U);
	float fDown = 1.0f / V * (Pattern / U + 1.0f / V * V);

	vtx[0].tex = D3DXVECTOR2(fLeft, fUp);
	vtx[1].tex = D3DXVECTOR2(fRight, fUp);
	vtx[2].tex = D3DXVECTOR2(fLeft, fDown);
	vtx[3].tex = D3DXVECTOR2(fRight, fDown);
}

//=========================================
// ���Z�����Ȃ��̕`��(�����`)
//=========================================
void RectDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt)
{
	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, Texture);

	// �|���S���̕`��
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt, 2);
}

//=========================================
// ���Z�����L��̕`��(�����`)
//=========================================
void AddSynthesis(LPDIRECT3DDEVICE9 pDevice)
{
	// a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

//=========================================
// �Ίp���̒l���Z�o(�����`)
//=========================================
void DiagonalLine(float* Length, float* Angele, float width, float height)
{
	// �Ίp���̒������Z�o����
	*Length = sqrtf((width * width + height * height) / 2.0f);
	// �Ίp���̊p�x���Z�o
	*Angele = atan2f(width, height);
}