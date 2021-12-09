//=========================================
// 
// winプロジェクトの作成
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
// マクロ定義
//-----------------------------------------
#define CLASS_NAME	"windowclass"			// ウインドウクラスの名前
#define WINDOW_NAME	"3Dアクション"		// ウインドウクラスの名前（キャプションに表示）
#define FVF_VERTEX_2D	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

//-----------------------------------------
// プロトタイプ宣言
//-----------------------------------------
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWmd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);
void DrawFPS(void);

//-----------------------------------------
// グローバル変数
//-----------------------------------------
LPDIRECT3D9	g_pD3D = NULL;
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;
LPD3DXFONT g_pFont = NULL;	// フォントへのポインタ
int g_nCountFPS = 0;		// FPSカウンタ

//=========================================
// メイン関数
//=========================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, LPSTR lpCmdLine, int nCmdShow)
{
	DWORD dwCurrentTime;	// 現在時刻
	DWORD dwExecLastTime;	// 最後に処理した時刻
	DWORD dwFrameCount;		// フレームカウント
	DWORD dwFPSLastTime;	// 最後にFPSを計測した時刻

	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),					// WNDCLASSEXのメモリサイズ
		CS_CLASSDC,							// ウィンドウのスタイル
		WindowProc,							// ウィンドウプロシージャ
		0,									// 0にする(通常は使用しない)
		0,									// 0にする(通常は使用しない)
		hInstance,							// インスタンスバンドル
		LoadIcon(NULL,IDI_APPLICATION),		// タスクバーのアイコン
		LoadCursor(NULL,IDC_ARROW),			// マウスカーソル
		(HBRUSH)(COLOR_WINDOW + 1),			// インスタンス
		NULL,								// メニューバー
		CLASS_NAME,							// ウィンドウクラスの名前
		LoadIcon(NULL,IDI_APPLICATION)		// ファイルのアイコン
	};

	HWND hWnd;		// ウィンドウハンドル(識別子)
	MSG msg;		// メッセージと格納する変数

	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };

	// ウィンドウクラスの登録
	RegisterClassEx(&wcex);

	// クライアント領域を指定のサイズに調整
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, FALSE);

	// ウィンドウの生成
	hWnd = CreateWindowEx(0,		// 拡張ウインドウスタイル
		CLASS_NAME,					// ウィンドウクラスの名前
		WINDOW_NAME,				// ウィンドウの名前
		WS_OVERLAPPEDWINDOW,		// ウィンドウのスタイル
		CW_USEDEFAULT,				// ウィンドウの左上X座標
		CW_USEDEFAULT,				// 　　""　　左上Y座標
		(rect.right - rect.left),	// 　　""　　幅
		(rect.bottom - rect.top),	// 　　""　　高さ
		NULL,						// 親ウィンドウのハンドル
		NULL,						// メニューハンドルまたは子ウィンドウID
		hInstance,					// インスタンスハンドル
		NULL);						// ウィンドウ作成データ

	if (FAILED(Init(hInstance, hWnd, true)))	// true = window,false = fullscreen
	{// 初期化処理が失敗した場合
		return -1;
	}

	// 分解能を設定
	timeBeginPeriod(1);
	dwCurrentTime = 0;					// 初期化処理
	dwExecLastTime = timeGetTime();		// 現在時刻を取得(保存)
	dwFrameCount = 0;
	dwFPSLastTime = timeGetTime();

	// ウィンドウの表示
	ShowWindow(hWnd, nCmdShow);	// ウィンドウの表示状態を設定
	UpdateWindow(hWnd);			// クライアント領域を更新

	// メッセージループ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{// windowsの処理
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				// メッセージの設定
				TranslateMessage(&msg);	// 仮想メッセージを文字メッセージへ変換
				DispatchMessage(&msg);	// ウィンドウプロシージャへメッセージを送信
			}
		}
		else
		{// DirectXの処理
			dwCurrentTime = timeGetTime();	// 現在時刻を取得

			if ((dwCurrentTime - dwFPSLastTime) >= 500)
			{// 0.5秒経過
			 // FPSを計測
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);

				dwFPSLastTime = dwCurrentTime;	// FPSを測定した時刻を保存
				dwFrameCount = 0;				// フレームカウントをクリア
			}
			if ((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
			{// 60分の1秒経過
				dwExecLastTime = dwCurrentTime;	// 処理開始の時刻[現在時刻]を保存

				// 更新処理
				Update();

				// 描画処理
				Draw();

				dwFrameCount++;	// フレームカウントを加算

			}
		}
	}

	// 終了処理
	Uninit();

	// 分解能を戻す
	timeEndPeriod(1);

	// ウィンドウクラスの登録を解除
	UnregisterClass(CLASS_NAME, wcex.hInstance);

	return (int)msg.wParam;
}

//=========================================
// ウィンドウプロシージャ
//=========================================
LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:	// ウィンドウ破棄のメッセージ
		// WM_QUITメッセージを送る
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:	// キー押下のメッセージ
		switch (wParam)
		{
		case VK_ESCAPE:	// [ESC]キーが押された
			// ウィンドウを破棄する(WM_DESTOROYメッセージを送る)
			DestroyWindow(hWnd);
			break;
		}
		break;

	case WM_CLOSE:		// 閉じるボタン押下のメッセージ
		// ウィンドウを破棄する
		DestroyWindow(hWnd);
		break;
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);	// 規定の処理を返す
}

//=========================================
// 初期化
//=========================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DDISPLAYMODE d3ddm;			// ディスプレイモード
	D3DPRESENT_PARAMETERS d3dpp;	// プレゼンテーションパラメータ
									// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	// 現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	// デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));		// パラメータのゼロクリア

	d3dpp.BackBufferWidth = SCREEN_WIDTH;						// ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;						// ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;						// バックバッファの形式
	d3dpp.BackBufferCount = 1;									// バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;					// ダブルバッファの切り替え（映像信号に同期）
	d3dpp.EnableAutoDepthStencil = TRUE;						// デプスバッファとステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;					// デプスバッファとして16bitを使う
	d3dpp.Windowed = bWindow;									// ウィンドウモード
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;	// リフレッシュレート
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル

																// Direct3Dデバイスの生成(描画処理と頂点処理をハードウェアで行う)
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&g_pD3DDevice)))
	{
		// Direct3Dデバイスの生成(描画処理はハードウェア、頂点処理はCPUで行なう)
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			// Direct3Dデバイスの生成(描画処理はハードウェア、頂点処理はCPUで行なう)
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

	// レンダーステート設定
	g_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);				// カリングの設定
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// アルファブレンドの設定
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		// アルファブレンドの設定
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	// アルファブレンドの設定

	// サンプラーステートの設定
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);	// テクスチャの大きさを変えても綺麗に貼る
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);	// テクスチャの大きさを変えても綺麗に貼る
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);	// テクスチャを繰り返して貼る
	g_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);	// テクスチャを繰り返して貼る

	// テクスチャステージステート
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);	// ポリゴンとテクスチャのアルファ値を混ぜる。
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);	// ポリゴンとテクスチャのアルファ値を混ぜる。テクスチャ指定
	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);	// ポリゴンとテクスチャのアルファ値を混ぜる。ポリゴン指定

	// デバッグ表示用フォントの生成
	D3DXCreateFont(g_pD3DDevice, 32, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &g_pFont);

	// キーボードの初期化処理
	if (FAILED(InitKeyboard(hInstance, hWnd)))
	{
		return E_FAIL;
	}

	// ポリゴンの初期化処理
	InitPolygon();

	// カメラの初期化処理
	InitCamera();

	// ライトの初期化処理
	InitLight();

	// ビルボードの初期化処理
	InitBillboard();

	// エフェクトの初期化処理
	InitEffect();

	// エフェクトの初期化処理
	InitSpawnEffect();

	SetBillboard(D3DXVECTOR3(0.0f, 0.0f, 25.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	// 壁の初期化処理
	InitWall();

	// 壁の設定処理
	SetWall(D3DXVECTOR3(0.0f, 0.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	SetWall(D3DXVECTOR3(0.0f, 0.0f, -50.0f), D3DXVECTOR3(0.0f, D3DX_PI, 0.0f));
	SetWall(D3DXVECTOR3(50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, D3DX_PI * 0.5f, 0.0f));
	SetWall(D3DXVECTOR3(-50.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, -D3DX_PI * 0.5f, 0.0f));


	return S_OK;
}

//=========================================
// 終了
//=========================================
void Uninit(void)
{
	// デバッグ表示用フォントの破棄
	if (g_pFont != NULL)
	{
		g_pFont->Release();
		g_pFont = NULL;
	}
	// Direct3Dデバイスの破棄
	if (g_pD3DDevice != NULL)
	{
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	// Direct3Dオブジェクトの破棄
	if (g_pD3D != NULL)
	{
		g_pD3D->Release();
		g_pD3D = NULL;
	}

	// キーボードの終了処理
	UninitKeyboard();

	// ポリゴンの終了処理
	UninitPolygon();

	// カメラの終了処理
	UninitCamera();

	// ライトの終了処理
	UninitLight();

	// ビルボードの終了処理
	UninitBillboard();

	// エフェクトの終了処理
	UninitEffect();

	// エフェクトの初期化処理
	UninitSpawnEffect();

	// 壁の終了処理
	UninitWall();
}

//=========================================
// 更新
//=========================================
void Update(void)
{	
	// キーボードの更新
	UpdateKeyboard();

	// カメラの更新
	UpdateCamera();

	// ライトの更新
	UpdateLight();

	// ポリゴンの更新
	UpdatePolygon();

	// ビルボードの更新
	UpdateBillboard();

	// エフェクトの更新処理
	UpdateEffect();

	// エフェクトの初期化処理
	UpdateSpawnEffect();

	// 壁の更新
	UpdateWall();

	if (GetKeyboardTrigger(DIK_F1))
	{
		// ワイヤーフレームモードの設定
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	}
	else if (GetKeyboardTrigger(DIK_F2))
	{
		// ワイヤーフレームモードの設定をもとに戻す
		g_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	}
}

//=========================================
// 描画
//=========================================
void Draw(void)
{
	// 画面クリア(バックバッファ＆Zバッファのクリア)
	g_pD3DDevice->Clear(0, NULL,
		(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER),
		D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 描画開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{// 	描画開始が成功した場合

		// カメラの設定処理
		SetCamera();

		// ポリゴンの描画処理
		DrawPolygon();

		// ビルボードの描画処理
		DrawBillboard();

		// エフェクトの描画処理
		DrawEffect();

		// 壁の描画処理
//		DrawWall();

#ifdef _DEBUG
		// FPSの表示
		DrawFPS();
#endif // _DEBUG

		// 描画終了
		g_pD3DDevice->EndScene();
	}
	// バックバッファとフロントバッファの入れ替え
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}


//=========================================
// デバイスの取得
//=========================================
LPDIRECT3DDEVICE9 GetDevice(void)
{
	return g_pD3DDevice;
}

//=========================================
// FPSの表示
//=========================================
void DrawFPS(void)
{
	RECT rect = { 0,0,SCREEN_WIDTH,SCREEN_HEIGHT };
	char aStr[1280];
	char aStrCopy[128];

	//パーティクル情報の取得
	Effect *pParticle;						//パーティクル情報へのポインタ
	pParticle = GetParticle();

	//扱うパーティクル数情報の取得
	int nMaxSwitch;							//扱うパーティクル数情報へのポインタ
	nMaxSwitch = GetMaxSwitch();

	//扱うパーティクル情報の取得
	int nSwitch;							//扱うパーティクル情報へのポインタ
	nSwitch = GetSwitch();

	//現在のモード情報の取得
	int nMode;								//現在のモード情報へのポインタ
	nMode = GetMode();

	//現在の編集中の情報の取得
	int nAdjustInfo = GetAdjustInfo();

	pParticle += (nSwitch * 1);

	//文字列の代入
	wsprintf(&aStr[0], "+------------------------+\n");
	strcat(&aStr[0], "|   パーティクル実験室   |\n");
	strcat(&aStr[0], "+------------------------+\n");

	wsprintf(&aStrCopy[0], "FPS %d\n", g_nCountFPS);
	strcat(&aStr[0], &aStrCopy[0]);

	if (nMode == 0)
	{
		wsprintf(&aStrCopy[0], "設定モード[F1]\n");
		strcat(&aStr[0], &aStrCopy[0]);

		wsprintf(&aStrCopy[0], "扱うエフェクト数[ 9 / 0 ]:%d\n", nMaxSwitch);
		strcat(&aStr[0], &aStrCopy[0]);

		wsprintf(&aStrCopy[0], "扱うエフェクト[ TUB / LSIFT ]:%d\n", nSwitch + 1);
		strcat(&aStr[0], &aStrCopy[0]);

		wsprintf(&aStrCopy[0], "[F3]で[%d]を保存\n", nSwitch + 1);
		strcat(&aStr[0], &aStrCopy[0]);

		switch (nAdjustInfo)
		{
		case ADJUST_INFO_ONESHOT:		// 一度に出現するエフェクト数

			wsprintf(&aStrCopy[0], "エフェクト数[ J / L ]:%d\n", pParticle->nPopCnt);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_ROT:			// エフェクトの角度調整処理

			sprintf(&aStrCopy[0], "本体角度[ G / H ]:%0.2f\n", pParticle->randAangle.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_SPREAD:		// エフェクトの発射角度調整処理

			sprintf(&aStrCopy[0], "拡散角度[ Z / C ]:%0.2f\n", pParticle->randAangle.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_MOVE:			// エフェクトの移動量調整処理
			sprintf(&aStrCopy[0], "通常時の移動速度[ B / N ]:%0.2f\n", pParticle->randSpeed.fValue);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数[ R ]:%s\n", pParticle->randSpeed.initial.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数最小数[ T / Y ]:%0.2f\n", pParticle->randSpeed.initial.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数最大数[ U / I ]:%0.2f\n", pParticle->randSpeed.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数[ F ]:%s\n", pParticle->randSpeed.Add.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数最小数[ G / H ]:%0.2f\n", pParticle->randSpeed.Add.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数最大数[ J / K ]:%0.2f\n", pParticle->randSpeed.Add.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_RADIUS:		// エフェクトの半径調整処理
			sprintf(&aStrCopy[0], "通常時の半径[ B / N ]:%0.2f\n", pParticle->randRadius.fValue);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数[ R ]:%s\n", pParticle->randRadius.initial.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数最小数[ T / Y ]:%0.2f\n", pParticle->randRadius.initial.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数最大数[ U / I ]:%0.2f\n", pParticle->randRadius.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数[ F ]:%s\n", pParticle->randRadius.Add.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数最小数[ G / H ]:%0.2f\n", pParticle->randRadius.Add.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数最大数[ J / K ]:%0.2f\n", pParticle->randRadius.Add.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_LIFE:			// エフェクトの寿命調整処理

			sprintf(&aStrCopy[0], "通常時の寿命[ B / N ]:%d\n", pParticle->randLife.nValue);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数[ R ]:%s\n", pParticle->randLife.initial.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数最小数[ T / Y ]:%d\n", pParticle->randLife.initial.nMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数最大数[ U / I ]:%d\n", pParticle->randLife.initial.nMax);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数[ F ]:%s\n", pParticle->randLife.Add.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数最小数[ G / H ]:%d\n", pParticle->randLife.Add.nMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数最大数[ J / K ]:%d\n", pParticle->randLife.Add.nMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_COLR:			// エフェクトの色調整処理
			sprintf(&aStrCopy[0], "通常時のカラー(Red)[ B / N ]:%0.2f\n", pParticle->colR.fValue);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数[ R ]:%s\n", pParticle->colR.initial.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数最小数[ T / Y ]:%0.2f\n", pParticle->colR.initial.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数最大数[ U / I ]:%0.2f\n", pParticle->colR.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数[ F ]:%s\n", pParticle->colR.Add.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数最小数[ G / H ]:%0.2f\n", pParticle->colR.Add.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数最大数[ J / K ]:%0.2f\n", pParticle->colR.Add.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_COLG:			// エフェクトの色調整処理
			sprintf(&aStrCopy[0], "通常時のカラー(Green)[ B / N ]:%0.2f\n", pParticle->colG.fValue);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数[ R ]:%s\n", pParticle->colG.initial.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数最小数[ T / Y ]:%0.2f\n", pParticle->colG.initial.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数最大数[ U / I ]:%0.2f\n", pParticle->colG.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数[ F ]:%s\n", pParticle->colG.Add.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数最小数[ G / H ]:%0.2f\n", pParticle->colG.Add.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数最大数[ J / K ]:%0.2f\n", pParticle->colG.Add.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_COLB:			// エフェクトの色調整処理
			sprintf(&aStrCopy[0], "通常時のカラー(Blue)[ B / N ]:%0.2f\n", pParticle->colB.fValue);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数[ R ]:%s\n", pParticle->colB.initial.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数最小数[ T / Y ]:%0.2f\n", pParticle->colB.initial.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数最大数[ U / I ]:%0.2f\n", pParticle->colB.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数[ F ]:%s\n", pParticle->colB.Add.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数最小数[ G / H ]:%0.2f\n", pParticle->colB.Add.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数最大数[ J / K ]:%0.2f\n", pParticle->colB.Add.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_COLA:			// エフェクトの色調整処理
			sprintf(&aStrCopy[0], "通常時のカラー(Alpha)[ B / N ]:%0.2f\n", pParticle->colA.fValue);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数[ R ]:%s\n", pParticle->colA.initial.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数最小数[ T / Y ]:%0.2f\n", pParticle->colA.initial.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "出現時の乱数最大数[ U / I ]:%0.2f\n", pParticle->colA.initial.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数[ F ]:%s\n", pParticle->colA.Add.bIsRandom ? "true" : "false");
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数最小数[ G / H ]:%0.2f\n", pParticle->colA.Add.fMin);
			strcat(&aStr[0], &aStrCopy[0]);
			sprintf(&aStrCopy[0], "加算時の乱数最大数[ J / K ]:%0.2f\n", pParticle->colA.Add.fMax);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		case ADJUST_INFO_GRAVITY:		// エフェクトの重力の調整

			sprintf(&aStrCopy[0], "重力[ B / M ]:%0.3f\n", pParticle->fGravity);
			strcat(&aStr[0], &aStrCopy[0]);
			break;
		default:
			break;
		}

	}
	else if (nMode == 1)
	{
		wsprintf(&aStrCopy[0], "テストモード[T]\n");
		strcat(&aStr[0], &aStrCopy[0]);

		wsprintf(&aStrCopy[0], "扱うエフェクト[ TUB / LSIFT ]:%d\n", nSwitch + 1);
		strcat(&aStr[0], &aStrCopy[0]);

		wsprintf(&aStrCopy[0], "[F3]で[%d]を保存\n", nSwitch + 1);
		strcat(&aStr[0], &aStrCopy[0]);

		wsprintf(&aStrCopy[0], "パーティクル発生カウント[ ↓ / ↑ ]:%d\n", pParticle->nCountParticle);
		strcat(&aStr[0], &aStrCopy[0]);
	}

	//テキスト描画
	g_pFont->DrawText(NULL, &aStr[0], -1, &rect, DT_LEFT, D3DCOLOR_RGBA(255, 0, 0, 255));
}