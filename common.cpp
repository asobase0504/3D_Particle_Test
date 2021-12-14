//=========================================
// 
// �����̐ݒ�
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
void RectAddDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt)
{
	// a�u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	RectDraw(pDevice, Texture, nCnt);

	// a�u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

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

