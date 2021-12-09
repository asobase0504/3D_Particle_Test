//=========================================
// 
// キーボードの処置
// Author YudaKaito
// 
//=========================================
//------------------------------------
// include
//------------------------------------
#include "main.h"
#include "input.h"

//------------------------------------
// マクロ定義
//------------------------------------
#define NUM_KEY_MAX		(256)	// キーの最大数

//------------------------------------
// 静的変数
//------------------------------------
static LPDIRECTINPUT8 s_pInput = NULL;					// DirectInputオブジェクトへのポインタ
static LPDIRECTINPUTDEVICE8 s_pDevKeyboard = NULL;		// 入力デバイス(キーボード)へのポインタ
static BYTE s_aKeyState[NUM_KEY_MAX];					// キーボードのプレス情報
static BYTE s_akeyStateTrigger[NUM_KEY_MAX];			// キーボードのトリガー情報
static BYTE s_akeyStateRelese[NUM_KEY_MAX];				// キーボードのリリース情報

static XINPUT_STATE s_joyKeyState;						// ジョイパッドのプレス情報
static XINPUT_STATE s_joykeyStateTrigger;				// ジョイパッドのトリガー情報
static XINPUT_STATE s_joykeyStateRelese;				// ジョイパッドのリリース情報
static JOYSTICK s_JoyStick;								// ジョイスティック情報
static float s_JoypadLStickAngle;						// Lスティックの角度
static bool s_bUseJoyPad;								// ジョイパッドを使用してるか

//====================================
// キーボードの初期化
//====================================
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	// Directオブジェクトの生成
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&s_pInput, NULL)))
	{
		return E_FAIL;
	}

	// 入力デバイス(キーボード)の生成
	if (FAILED(s_pInput->CreateDevice(GUID_SysKeyboard, &s_pDevKeyboard, NULL)))
	{
		return E_FAIL;
	}

	// データフォーマットを設定
	if (FAILED(s_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(s_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得
	s_pDevKeyboard->Acquire();

	return S_OK;
}

//====================================
// キーボードの終了
//====================================
void UninitKeyboard(void)
{
	// 入力デバイス(キーボード)の破棄
	if (s_pDevKeyboard != NULL)
	{
		s_pDevKeyboard->Unacquire();	// キーボードへのアクセス権を放棄
		s_pDevKeyboard->Release();
		s_pDevKeyboard = NULL;
	}

	// DirectInuptオブジェクトの破棄
	if (s_pInput != NULL)
	{
		s_pInput->Release();
	}
}

//====================================
// キーボードの更新
//====================================
void UpdateKeyboard(void)
{
	BYTE aKeyState[NUM_KEY_MAX];	// キーボードの入力情報
	int nCntKey;

	// 入力デバイスからデータを取得
	if (SUCCEEDED(s_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			s_akeyStateTrigger[nCntKey] = ~s_aKeyState[nCntKey] & aKeyState[nCntKey];
			s_akeyStateRelese[nCntKey] = s_aKeyState[nCntKey] & ~aKeyState[nCntKey];
			s_aKeyState[nCntKey] = aKeyState[nCntKey];	// キーボードのプレス情報を保存
		}
	}
	else
	{
		s_pDevKeyboard->Acquire();	// キーボードのアクセス権を獲得
	}
}

//====================================
// キーボードのプレス情報取得
//====================================
bool GetKeyboardPress(int nKey)
{
	return (s_aKeyState[nKey] & 0x80) ? true : false;
}

//====================================
// キーボードのトリガー情報取得
//====================================
bool GetKeyboardTrigger(int nKey)
{
	return (s_akeyStateTrigger[nKey] & 0x80) ? true : false;
}

//====================================
// キーボードのリリース情報取得
//====================================
bool GetKeyboardRelease(int nKey)
{
	return (s_akeyStateRelese[nKey] & 0x80) ? true : false;
}

//====================================
// ジョイパットの初期化
//====================================
HRESULT InitJoypad(void)
{
	// メモリのクリア
	memset(&s_joyKeyState, 0, sizeof(XINPUT_STATE));

	// XInputのステートを設定(有効にする)
	XInputEnable(true);

	// ジョイスティック情報の初期化
	s_JoyStick.nTypeJoyStick = 0;
	s_JoyStick.nStickDeadLine = 0;
	s_JoyStick.fStickAngle = 0.0f;
	s_JoyStick.bUse = false;

	// ジョイスティックの角度を初期化
	s_JoypadLStickAngle = 0;

	// 使用状況の初期化
	s_bUseJoyPad = false;
	return S_OK;
}

//====================================
// ジョイパットの終了
//====================================
void UninitJoypad(void)
{
	// XInputのステートを設定(無効にする)
	XInputEnable(false);
}

//====================================
// ジョイパットの更新
//====================================
void UpdateJoypad(void)
{
	XINPUT_STATE joykeyState;	// ジョイパッドの入力処理

	// ジョイパッドの状態の取得
	if (XInputGetState(0, &joykeyState) == ERROR_SUCCESS)
	{
		s_joykeyStateTrigger.Gamepad.wButtons = ~s_joyKeyState.Gamepad.wButtons & joykeyState.Gamepad.wButtons;
		s_joykeyStateRelese.Gamepad.wButtons = s_joyKeyState.Gamepad.wButtons & ~joykeyState.Gamepad.wButtons;
		s_joyKeyState = joykeyState;	// ジョイパッドのプレス情報を保存
		// 使用状況の更新
		s_bUseJoyPad = true;
	}
	else
	{
		// 使用状況の更新
		s_bUseJoyPad = false;
	}

}

//====================================
// ジョイパットのプレス情報を取得
//====================================
bool GetJoypadPress(JOYKEY key)
{
	return(s_joyKeyState.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//====================================
// ジョイパットのトリガー情報取得
//====================================
bool GetJoypadTrigger(JOYKEY key)
{
	return(s_joykeyStateTrigger.Gamepad.wButtons & (0x01 << key)) ? true : false;
}

//====================================
// ジョイパッドのリリース情報取得
//====================================
bool GetJoypadRelease(JOYKEY key)
{
	return false;
}

//====================================
// ジョイパットのスティック処理
//====================================
D3DXVECTOR3 GetJoypadStick(JOYKEY nKey)
{
	// 変数宣言
	D3DXVECTOR3 stick;

	switch (nKey)
	{
	case JOYKEY_L_STICK:
		stick = D3DXVECTOR3(s_joyKeyState.Gamepad.sThumbLX / 30000.0f, -s_joyKeyState.Gamepad.sThumbLY / 30000.0f, 0.0f);

		s_JoypadLStickAngle = atan2f(s_joyKeyState.Gamepad.sThumbLX / 30000.0f, -s_joyKeyState.Gamepad.sThumbLY / 30000.0f);
		break;

	case JOYKEY_R_STICK:
		stick = D3DXVECTOR3(s_joyKeyState.Gamepad.sThumbRX / 30000.0f, -s_joyKeyState.Gamepad.sThumbRY / 30000.0f, 0.0f);
		break;
	}

	return stick;
}


//====================================
// ジョイパットの角度の取得
//====================================
float GetJoyStickAngle(void)
{
	return s_JoypadLStickAngle;
}

//====================================
// ジョイパットの使用情報取得
//====================================
bool IsUseJoyPad(void)
{
	return s_bUseJoyPad;
}