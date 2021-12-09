//=========================================
// 
// win�v���W�F�N�g�̍쐬
// Author YudaKaito
// 
//=========================================

//-----------------------------------------
// include
//-----------------------------------------
#include "main.h"
#include "input.h"
#include "polygon.h"
#include "camera.h"
#include "light.h"
#include "wall.h"
#include "billboard.h"
#include "spawn_effect.h"
#include "effect.h"
#include <stdio.h>

//-----------------------------------------
// �}�N����`
//-----------------------------------------
#define CLASS_NAME	"windowclass"			// �E�C���h�E�N���X�̖��O
#define WINDOW_NAME	"3D�A�N�V����"		// �E�C���h�E�N���X�̖��O�i�L���v�V�����ɕ\���j
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//-----------------------------------------
// �v���g�^�C�v�錾
//-----------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWmd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);

//-----------------------------------------
// �O���[�o���ϐ�
//-----------------------------------------
LPDIRECT3D9	g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
LPD3DXFONT g_pFont = NULL;	// �t�H���g�ւ̃|�C���^
int g_nCountFPS = 0;		// FPS�J�E���^

//=========================================
// ���C���֐�
//=========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;	// ���ݎ���
	DWORD dwExecLastTime;	// �Ō�ɏ�����������
	DWORD dwFrameCount;		// �t���[���J�E���g
	DWORD dwFPSLastTime;	// �Ō��FPS���v����������

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					// WNDCLASSEX�̃������T�C�Y
		CS_CLASSDC,							// �E�B���h�E�̃X�^�C��
		WindowProc,							// �E�B���h�E�v���V�[�W��
		0,									// 0�ɂ���(�ʏ�͎g�p���Ȃ�)
		0,									// 0�ɂ���(�ʏ�͎g�p���Ȃ�)
		hInstance,							// �C���X�^���X�o���h��
		LoadIcon(NULL,IDI_APPLICATION),		// �^�X�N�o�[�̃A�C�R��
		LoadCursor(NULL,IDC_ARROW),			// �}�E�X�J�[�\��
		(HBRUSH)(COLOR_WINDOW + 1),			// �C���X�^���X
		NULL,								// ���j���[�o�[
		CLASS_NAME,							// �E�B���h�E�N���X�̖��O
		LoadIcon(NULL,IDI_APPLICATION)		// �t�@�C���̃A�C�R��
	};

	HWND hWnd;		// �E�B���h�E�n���h��(���ʎq)
	MSG msg;		// ���b�Z�[�W�Ɗi�[����ϐ�

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// �E�B���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	// �N���C�A���g�̈���w��̃T�C�Y�ɒ���
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// �E�B���h�E�̐���
	hWnd = CreateWindowEx(0,		// �g���E�C���h�E�X�^�C��
		CLASS_NAME,					// �E�B���h�E�N���X�̖��O
		WINDOW_NAME,				// �E�B���h�E�̖��O
		WS_OVERLAPPEDWINDOW,		// �E�B���h�E�̃X�^�C��
		CW_USEDEFAULT,				// �E�B���h�E�̍���X���W
		CW_USEDEFAULT,				// �@�@""�@�@����Y���W
		(rect.right - rect.left),	// �@�@""�@�@��
		(rect.bottom - rect.top),	// �@�@""�@�@����
		NULL,						// �e�E�B���h�E�̃n���h��
		NULL,						// ���j���[�n���h���܂��͎q�E�B���h�EID
		hInstance,					// �C���X�^���X�n���h��
		NULL);						// �E�B���h�E�쐬�f�[�^

	if (FAILED(Init(hInstance, hWnd, true)))	// true = window,false = fullscreen
	{// ���������������s�����ꍇ
		return -1;
	}

	// ����\��ݒ�
	timeBeginPeriod(1);
	dwCurrentTime = 0;					// ����������
	dwExecLastTime = timeGetTime();		// ���ݎ������擾(�ۑ�)
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// �E�B���h�E�̕\��
	ShowWindow(hWnd, nCmdShow);	// �E�B���h�E�̕\����Ԃ�ݒ�
	UpdateWindow(hWnd);			// �N���C�A���g�̈���X�V

	// ���b�Z�[�W���[�v
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windows�̏���
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// ���b�Z�[�W�̐ݒ�
				TranslateMessage(&msg);	// ���z���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);	// �E�B���h�E�v���V�[�W���փ��b�Z�[�W�𑗐M
			}
		}
		else
		{// DirectX�̏���
			dwCurrentTime = timeGetTime();	// ���ݎ������擾

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5�b�o��
			 // FPS���v��
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// FPS�𑪒肵��������ۑ�
				dwFrameCount = 0;				// �t���[���J�E���g���N���A
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60����1�b�o��
				dwExecLastTime = dwCurrentTime;	// �����J�n�̎���[���ݎ���]��ۑ�

				// �X�V����
				Update();

				// �`�揈��
				Draw();

				dwFrameCount++;	// �t���[���J�E���g�����Z

			}
		}
	}

	// �I������
	Uninit();

	// ����\��߂�
	timeEndPeriod(1);

	// �E�B���h�E�N���X�̓o�^������
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=========================================
// �E�B���h�E�v���V�[�W��
//=========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:	// �E�B���h�E�j���̃��b�Z�[�W
		// WM_QUIT���b�Z�[�W�𑗂�
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:	// �L�[�����̃��b�Z�[�W
		switch (wParam)
		{
		case VK_ESCAPE:	// [ESC]�L�[�������ꂽ
			// �E�B���h�E��j������(WM_DESTOROY���b�Z�[�W�𑗂�)
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_CLOSE:		// ����{�^�������̃��b�Z�[�W
		// �E�B���h�E��j������
		DestroyWindow(hWnd);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// �K��̏�����Ԃ�
}

//=========================================
// ������
//=========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// �f�B�X�v���C���[�h
	D3DPRESENT_PARAMETERS d3dpp;	// �v���[���e�[�V�����p�����[�^
									// Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// ���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// �f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));		// �p�����[�^�̃[���N���A

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// �Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// �Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;						// �o�b�N�o�b�t�@�̌`��
	d3dpp.BackBufferCount = 1;									// �o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// �_�u���o�b�t�@�̐؂�ւ��i�f���M���ɓ����j
	d3dpp.EnableAutoDepthStencil = TRUE;						// �f�v�X�o�b�t�@�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// �f�v�X�o�b�t�@�Ƃ���16bit���g��
	d3dpp.Windowed = bWindow;									// �E�B���h�E���[�h
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// ���t���b�V�����[�g
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// �C���^�[�o��

																// Direct3D�f�o�C�X�̐���(�`�揈���ƒ��_�������n�[�h�E�F�A�ōs��)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		// Direct3D�f�o�C�X�̐���(�`�揈���̓n�[�h�E�F�A�A���_������CPU�ōs�Ȃ�)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// Direct3D�f�o�C�X�̐���(�`�揈���̓n�[�h�E�F�A�A���_������CPU�ōs�Ȃ�)
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				return E_FAIL;
			}

		}
	}

	// �����_�[�X�e�[�g�ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// �J�����O�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// �A���t�@�u�����h�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// �A���t�@�u�����h�̐ݒ�
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// �A���t�@�u�����h�̐ݒ�

	// �T���v���[�X�e�[�g�̐ݒ�
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���̑傫����ς��Ă��Y��ɓ\��
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// �e�N�X�`���̑傫����ς��Ă��Y��ɓ\��
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// �e�N�X�`�����J��Ԃ��ē\��
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// �e�N�X�`�����J��Ԃ��ē\��

	// �e�N�X�`���X�e�[�W�X�e�[�g
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// �|���S���ƃe�N�X�`���̃A���t�@�l��������B
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// �|���S���ƃe�N�X�`���̃A���t�@�l��������B�e�N�X�`���w��
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// �|���S���ƃe�N�X�`���̃A���t�@�l��������B�|���S���w��

	// �f�o�b�O�\���p�t�H���g�̐���
	D3DXCreateFont(g_pD3DDevice, 32, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	// �L�[�{�[�h�̏���������
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// �|���S���̏���������
	InitPolygon();

	// �J�����̏���������
	InitCamera();

	// ���C�g�̏���������
	InitLight();

	// �r���{�[�h�̏���������
	InitBillboard();

	// �G�t�F�N�g�̏���������
	InitEffect();

	// �G�t�F�N�g�̏���������
	InitSpawnEffect();

	SetBillboard(D3DXVECTOR3(0.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// �ǂ̏���������
	InitWall();

	// �ǂ̐ݒ菈��
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetWall(D3DXVECTOR3(0.0f, 0.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	SetWall(D3DXVECTOR3(50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	SetWall(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));


	return S_OK;
}

//=========================================
// �I��
//=========================================
void Uninit(void)
{
	// �f�o�b�O�\���p�t�H���g�̔j��
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
	// Direct3D�f�o�C�X�̔j��
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3D�I�u�W�F�N�g�̔j��
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �|���S���̏I������
	UninitPolygon();

	// �J�����̏I������
	UninitCamera();

	// ���C�g�̏I������
	UninitLight();

	// �r���{�[�h�̏I������
	UninitBillboard();

	// �G�t�F�N�g�̏I������
	UninitEffect();

	// �G�t�F�N�g�̏���������
	UninitSpawnEffect();

	// �ǂ̏I������
	UninitWall();
}

//=========================================
// �X�V
//=========================================
void Update(void)
{	
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �J�����̍X�V
	UpdateCamera();

	// ���C�g�̍X�V
	UpdateLight();

	// �|���S���̍X�V
	UpdatePolygon();

	// �r���{�[�h�̍X�V
	UpdateBillboard();

	// �G�t�F�N�g�̍X�V����
	UpdateEffect();

	// �G�t�F�N�g�̏���������
	UpdateSpawnEffect();

	// �ǂ̍X�V
	UpdateWall();

	if (GetKeyboardTrigger(DIK_F1))
	{
		// ���C���[�t���[�����[�h�̐ݒ�
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else if (GetKeyboardTrigger(DIK_F2))
	{
		// ���C���[�t���[�����[�h�̐ݒ�����Ƃɖ߂�
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

//=========================================
// �`��
//=========================================
void Draw(void)
{
	// ��ʃN���A(�o�b�N�o�b�t�@��Z�o�b�t�@�̃N���A)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �`��J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// 	�`��J�n�����������ꍇ

		// �J�����̐ݒ菈��
		SetCamera();

		// �|���S���̕`�揈��
		DrawPolygon();

		// �r���{�[�h�̕`�揈��
		DrawBillboard();

		// �G�t�F�N�g�̕`�揈��
		DrawEffect();

		// �ǂ̕`�揈��
//		DrawWall();

#ifdef _DEBUG
		// FPS�̕\��
		DrawFPS();
#endif // _DEBUG

		// �`��I��
		g_pD3DDevice->EndScene();
	}
	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


//=========================================
// �f�o�C�X�̎擾
//=========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=========================================
// FPS�̕\��
//=========================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[1280];
	char aStrCopy[128];

	//�p�[�e�B�N�����̎擾
	Effect *pParticle;						//�p�[�e�B�N�����ւ̃|�C���^
	pParticle = GetParticle();

	//�����p�[�e�B�N�������̎擾
	int nMaxSwitch;							//�����p�[�e�B�N�������ւ̃|�C���^
	nMaxSwitch = GetMaxSwitch();

	//�����p�[�e�B�N�����̎擾
	int nSwitch;							//�����p�[�e�B�N�����ւ̃|�C���^
	nSwitch = GetSwitch();

	//���݂̃��[�h���̎擾
	int nMode;								//���݂̃��[�h���ւ̃|�C���^
	nMode = GetMode();

	//���݂̕ҏW���̏��̎擾
	int nAdjustInfo = GetAdjustInfo();

	pParticle += (nSwitch * 1);

	//������̑��
	wsprintf(&aStr[0], "+------------------------+\n");
	strcat(&aStr[0], "|   �p�[�e�B�N��������   |\n");
	strcat(&aStr[0], "+------------------------+\n");

	wsprintf(&aStrCopy[0], "FPS %d\n", g_nCountFPS);
	strcat(&aStr[0], &aStrCopy[0]);

	if (nMode == 0)
	{
		wsprintf(&aStrCopy[0], "�ݒ胂�[�h[F1]\n");
		strcat(&aStr[0], &aStrCopy[0]);

		wsprintf(&aStrCopy[0], "�����G�t�F�N�g��[ 9 / 0 ]:%d\n", nMaxSwitch);
		strcat(&aStr[0], &aStrCopy[0]);

		wsprintf(&aStrCopy[0], "�����G�t�F�N�g[ TUB / LSIFT ]:%d\n", nSwitch + 1);
		strcat(&aStr[0], &aStrCopy[0]);

		wsprintf(&aStrCopy[0], "[F3]��[%d]��ۑ�\n", nSwitch + 1);
		strcat(&aStr[0], &aStrCopy[0]);

		switch (nAdjustInfo)
		{
		case ADJUST_INFO_ONESHOT:		// ��x�ɏo������G�t�F�N�g��

			wsprintf(&aStrCopy[0], "�G�t�F�N�g��[ J / L ]:%d\n", pParticle->nPopCnt);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_ROT:			// �G�t�F�N�g�̊p�x��������

			sprintf(&aStrCopy[0], "�{�̊p�x[ G / H ]:%0.2f\n", pParticle->randAangle.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_SPREAD:		// �G�t�F�N�g�̔��ˊp�x��������

			sprintf(&aStrCopy[0], "�g�U�p�x[ Z / C ]:%0.2f\n", pParticle->randAangle.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_MOVE:			// �G�t�F�N�g�̈ړ��ʒ�������
			sprintf(&aStrCopy[0], "�ʏ펞�̈ړ����x[ B / N ]:%0.2f\n", pParticle->randSpeed.fValue);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗���[ R ]:%s\n", pParticle->randSpeed.initial.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗����ŏ���[ T / Y ]:%0.2f\n", pParticle->randSpeed.initial.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗����ő吔[ U / I ]:%0.2f\n", pParticle->randSpeed.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗���[ F ]:%s\n", pParticle->randSpeed.Add.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗����ŏ���[ G / H ]:%0.2f\n", pParticle->randSpeed.Add.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗����ő吔[ J / K ]:%0.2f\n", pParticle->randSpeed.Add.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_RADIUS:		// �G�t�F�N�g�̔��a��������
			sprintf(&aStrCopy[0], "�ʏ펞�̔��a[ B / N ]:%0.2f\n", pParticle->randRadius.fValue);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗���[ R ]:%s\n", pParticle->randRadius.initial.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗����ŏ���[ T / Y ]:%0.2f\n", pParticle->randRadius.initial.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗����ő吔[ U / I ]:%0.2f\n", pParticle->randRadius.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗���[ F ]:%s\n", pParticle->randRadius.Add.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗����ŏ���[ G / H ]:%0.2f\n", pParticle->randRadius.Add.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗����ő吔[ J / K ]:%0.2f\n", pParticle->randRadius.Add.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_LIFE:			// �G�t�F�N�g�̎�����������

			sprintf(&aStrCopy[0], "�ʏ펞�̎���[ B / N ]:%d\n", pParticle->randLife.nValue);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗���[ R ]:%s\n", pParticle->randLife.initial.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗����ŏ���[ T / Y ]:%d\n", pParticle->randLife.initial.nMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗����ő吔[ U / I ]:%d\n", pParticle->randLife.initial.nMax);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗���[ F ]:%s\n", pParticle->randLife.Add.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗����ŏ���[ G / H ]:%d\n", pParticle->randLife.Add.nMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗����ő吔[ J / K ]:%d\n", pParticle->randLife.Add.nMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_COLR:			// �G�t�F�N�g�̐F��������
			sprintf(&aStrCopy[0], "�ʏ펞�̃J���[(Red)[ B / N ]:%0.2f\n", pParticle->colR.fValue);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗���[ R ]:%s\n", pParticle->colR.initial.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗����ŏ���[ T / Y ]:%0.2f\n", pParticle->colR.initial.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗����ő吔[ U / I ]:%0.2f\n", pParticle->colR.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗���[ F ]:%s\n", pParticle->colR.Add.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗����ŏ���[ G / H ]:%0.2f\n", pParticle->colR.Add.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗����ő吔[ J / K ]:%0.2f\n", pParticle->colR.Add.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_COLG:			// �G�t�F�N�g�̐F��������
			sprintf(&aStrCopy[0], "�ʏ펞�̃J���[(Green)[ B / N ]:%0.2f\n", pParticle->colG.fValue);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗���[ R ]:%s\n", pParticle->colG.initial.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗����ŏ���[ T / Y ]:%0.2f\n", pParticle->colG.initial.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗����ő吔[ U / I ]:%0.2f\n", pParticle->colG.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗���[ F ]:%s\n", pParticle->colG.Add.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗����ŏ���[ G / H ]:%0.2f\n", pParticle->colG.Add.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗����ő吔[ J / K ]:%0.2f\n", pParticle->colG.Add.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_COLB:			// �G�t�F�N�g�̐F��������
			sprintf(&aStrCopy[0], "�ʏ펞�̃J���[(Blue)[ B / N ]:%0.2f\n", pParticle->colB.fValue);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗���[ R ]:%s\n", pParticle->colB.initial.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗����ŏ���[ T / Y ]:%0.2f\n", pParticle->colB.initial.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗����ő吔[ U / I ]:%0.2f\n", pParticle->colB.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗���[ F ]:%s\n", pParticle->colB.Add.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗����ŏ���[ G / H ]:%0.2f\n", pParticle->colB.Add.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗����ő吔[ J / K ]:%0.2f\n", pParticle->colB.Add.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_COLA:			// �G�t�F�N�g�̐F��������
			sprintf(&aStrCopy[0], "�ʏ펞�̃J���[(Alpha)[ B / N ]:%0.2f\n", pParticle->colA.fValue);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗���[ R ]:%s\n", pParticle->colA.initial.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗����ŏ���[ T / Y ]:%0.2f\n", pParticle->colA.initial.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "�o�����̗����ő吔[ U / I ]:%0.2f\n", pParticle->colA.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗���[ F ]:%s\n", pParticle->colA.Add.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗����ŏ���[ G / H ]:%0.2f\n", pParticle->colA.Add.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "���Z���̗����ő吔[ J / K ]:%0.2f\n", pParticle->colA.Add.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_GRAVITY:		// �G�t�F�N�g�̏d�͂̒���

			sprintf(&aStrCopy[0], "�d��[ B / M ]:%0.3f\n", pParticle->fGravity);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		default:
			break;
		}

	}
	else if (nMode == 1)
	{
		wsprintf(&aStrCopy[0], "�e�X�g���[�h[T]\n");
		strcat(&aStr[0], &aStrCopy[0]);

		wsprintf(&aStrCopy[0], "�����G�t�F�N�g[ TUB / LSIFT ]:%d\n", nSwitch + 1);
		strcat(&aStr[0], &aStrCopy[0]);

		wsprintf(&aStrCopy[0], "[F3]��[%d]��ۑ�\n", nSwitch + 1);
		strcat(&aStr[0], &aStrCopy[0]);

		wsprintf(&aStrCopy[0], "�p�[�e�B�N�������J�E���g[ �� / �� ]:%d\n", pParticle->nCountParticle);
		strcat(&aStr[0], &aStrCopy[0]);
	}

	//�e�L�X�g�`��
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}