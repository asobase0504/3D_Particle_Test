//=========================================
// 
// ˆ—‚ÌÝ’è
// Author YudaKaito
// 
//=========================================
//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "setup.h"

//=========================================
// Šp“x‚Ì³‹K‰»
//=========================================
float NormalizeRot(float rot)
{
	if (rot > D3DX_PI)
	{
		rot -= D3DX_PI * 2;
	}
	if (rot < -D3DX_PI)
	{
		rot += D3DX_PI * 2;
	}

	return rot;
}

//=========================================
// ‰ÁŽZ‡¬‚È‚µ‚Ì•`ŽÊ(’·•ûŒ`)
//=========================================
void RectDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt)
{
	// ƒeƒNƒXƒ`ƒƒ‚ÌÝ’è
	pDevice->SetTexture(0, Texture);

	// ƒ|ƒŠƒSƒ“‚Ì•`‰æ
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCnt, 2);
}

//=========================================
// ‰ÁŽZ‡¬—L‚è‚Ì•`ŽÊ(’·•ûŒ`)
//=========================================
void RectAddDraw(LPDIRECT3DDEVICE9 pDevice, LPDIRECT3DTEXTURE9 Texture, int nCnt)
{
	// aƒuƒŒƒ“ƒfƒBƒ“ƒO‚ð‰ÁŽZ‡¬‚ÉÝ’è
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	RectDraw(pDevice, Texture, nCnt);

	// aƒuƒŒƒ“ƒfƒBƒ“ƒO‚ðŒ³‚É–ß‚·
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

}
