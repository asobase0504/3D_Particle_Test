//=========================================
// 
// ���ʏ���(�w�b�_�[�t�@�C��)
// Author YudaKaito
// 
//=========================================
#ifndef _COMMON_H_
#define _COMMON_H_

//------------------------------------
// �v���g�^�C�v�錾
//------------------------------------
// �`��n���̏���
void RectDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt);		// �ʏ�̕`��
LPDIRECT3DDEVICE9 NoneAlphaBlend(LPDIRECT3DDEVICE9 pDevice);	// �ʏ퍇���`��
LPDIRECT3DDEVICE9 AddAlphaBlend(LPDIRECT3DDEVICE9 pDevice);		// ���Z�����`��

// �v�Z�n���̏���
void DiagonalLine(float* Length, float* Angele, float width, float height);			// �Ίp���̌v�Z
void VtxCol(VERTEX_3D *pVtx,D3DXCOLOR col);			// ���_�J���[�̐ݒ�
void NormalizeRot(float* rot);

// �J�����n���̏���

#endif // !_COMMON_H_
