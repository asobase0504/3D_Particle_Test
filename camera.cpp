//=========================================
// 
// カメラの作成
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "camera.h"
#include "input.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define CAMERA_MOVE			(1.0f)
#define CAMERA_ROT_VOLUME	(0.03f)

//-----------------------------------------
// 静的変数
//-----------------------------------------
static Camera s_camera;	// カメラ情報

//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
static void InputCamera(void);	// カメラの入力処理

//=========================================
// 初期化
//=========================================
void InitCamera(void)
{
	// 視点・注視点・上方向を設定する
	s_camera.posV = D3DXVECTOR3(0.0f,50.0f,-40.0f);	// 視点
	s_camera.posR = D3DXVECTOR3(0.0f,0.0f,0.0f);	// 注視点
	s_camera.vecU = D3DXVECTOR3(0.0f,1.0f,0.0f);	// 上方向ベクトル
	s_camera.rot = ZERO_VECTOR;	// 向き

	s_camera.rot.y = atan2f((s_camera.posR.x - s_camera.posV.x),(s_camera.posR.z - s_camera.posV.z));
	s_camera.rot.x = atan2f((s_camera.posR.z - s_camera.posV.z),(s_camera.posR.y - s_camera.posV.y));

	D3DXVECTOR3 vec = s_camera.posV - s_camera.posR;
	s_camera.fDistance = D3DXVec3Length(&vec);
}

//=========================================
// 終了
//=========================================
void UninitCamera(void)
{
}

//=========================================
// 更新
//=========================================
void UpdateCamera(void)
{
	Camera* pCamera = &(s_camera);

	InputCamera();

	// 角度の正規化
	if (pCamera->rot.y > D3DX_PI)
	{
		pCamera->rot.y -= D3DX_PI * 2;
	}
	if (pCamera->rot.y < -D3DX_PI)
	{
		pCamera->rot.y += D3DX_PI * 2;
	}

	pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;
	//pCamera->posV.y = pCamera->posR.y - tanf(-pCamera->rot.x) * pCamera->fDistance;

}

//=========================================
// 設定
//=========================================
void SetCamera(void)
{
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	Camera* camara = &(s_camera);

	// ビューマトリックスの初期化
	D3DXMatrixIdentity(&camara->mtxView);

	// ビューマトリックスの作成
	D3DXMatrixLookAtLH(&camara->mtxView, &camara->posV, &camara->posR, &camara->vecU);

	// ビューマトリックスの設定
	pDevice->SetTransform(D3DTS_VIEW, &camara->mtxView);	// ビュー座標行列の設定

	// プロジェクションマトリックスの初期化
	D3DXMatrixIdentity(&camara->mtxProjection);

	// プロジェクションマトリックスの作成
	D3DXMatrixPerspectiveFovLH(&camara->mtxProjection,
		D3DXToRadian(45.0f),						// 視野角
		(float)SCREEN_WIDTH / (float)SCREEN_HEIGHT,	// アスペクト比
		10.0f,150.0f);								// どこから(ニア)(第５引数)どこまで(ファー)(第６引数)をカメラで表示するか設定 

	// プロジェクションマトリックスの設定
	pDevice->SetTransform(D3DTS_PROJECTION, &camara->mtxProjection);
}

//=========================================
// 入力
//=========================================
void InputCamera(void)
{
	Camera* pCamera = &(s_camera);
	float fAngle;

	pCamera->vec = ZERO_VECTOR;

	// カメラの移動
	if (GetKeyboardPress(DIK_W))
	{
		pCamera->vec.x += sinf(pCamera->rot.y);
		pCamera->vec.z += cosf(pCamera->rot.y);
	}
	if (GetKeyboardPress(DIK_A))
	{
		pCamera->vec.x += sinf(D3DX_PI * -0.5f + pCamera->rot.y);
		pCamera->vec.z += cosf(D3DX_PI * -0.5f + pCamera->rot.y);
	}
	if (GetKeyboardPress(DIK_S))
	{
		pCamera->vec.x += sinf(D3DX_PI + pCamera->rot.y);
		pCamera->vec.z += cosf(D3DX_PI + pCamera->rot.y);
	}
	if (GetKeyboardPress(DIK_D))
	{
		pCamera->vec.x += sinf(D3DX_PI * 0.5f + pCamera->rot.y);
		pCamera->vec.z += cosf(D3DX_PI * 0.5f + pCamera->rot.y);
	}

	// 注視点角度の回転
	if (GetKeyboardPress(DIK_Z))
	{
		pCamera->rot.y += CAMERA_ROT_VOLUME;	// 回転量
		pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
		pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;
	}
	if (GetKeyboardPress(DIK_C))
	{
		pCamera->rot.y += -(CAMERA_ROT_VOLUME);	// 回転量
		pCamera->posV.x = pCamera->posR.x - sinf(pCamera->rot.y) * pCamera->fDistance;
		pCamera->posV.z = pCamera->posR.z - cosf(pCamera->rot.y) * pCamera->fDistance;
	}

	// 視点角度の回転
	if (GetKeyboardPress(DIK_Q))
	{
		pCamera->rot.y += -(CAMERA_ROT_VOLUME);	// 回転量
	}
	if (GetKeyboardPress(DIK_E))
	{
		pCamera->rot.y += CAMERA_ROT_VOLUME;	// 回転量
	}
	if (GetKeyboardPress(DIK_R))
	{
		pCamera->rot.x += -(CAMERA_ROT_VOLUME);	// 回転量
	}
	if (GetKeyboardPress(DIK_V))
	{
		pCamera->rot.x += CAMERA_ROT_VOLUME;	// 回転量
	}

	D3DXVec3Normalize(&pCamera->vec, &pCamera->vec);	// 正規化する(大きさ１のベクトルにする)
	pCamera->vec *= CAMERA_MOVE;
	pCamera->posV += pCamera->vec;

	pCamera->posR.x = pCamera->posV.x + sinf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posR.z = pCamera->posV.z + cosf(pCamera->rot.y) * pCamera->fDistance;
	pCamera->posR.y = pCamera->posV.y + tanf(-pCamera->rot.x + (D3DX_PI * 0.5f)) * pCamera->fDistance;
}

//=========================================
// カメラの角度情報取得
//=========================================
D3DXVECTOR3 GetRotCamera(void)
{
	return s_camera.rot;
}