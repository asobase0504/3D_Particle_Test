//=========================================
// 
// キーボードのヘッダーファイル
// Author YudaKaito
// 
//=========================================
#ifndef  _INPUT_H_
#define	_INPUT_H_

//------------------------------------
// キーの種類の列挙型
//------------------------------------
typedef enum
{
	JOYKEY_UP = 0,		// 十字キー（上）
	JOYKEY_DOWN,		// 十字キー（下）
	JOYKEY_LEFT,		// 十字キー（左）
	JOYKEY_RIGHT,		// 十字キー（右）
	JOYKEY_START,		// STARTボタン
	JOYKEY_BACK,		// BACKボタン
	JOYKEY_PUSHLSTICK,	// 左スティック押し込み
	JOYKEY_PUSHRSTICK,	// 右スティック押し込み
	JOYKEY_L_B,			// LBキー
	JOYKEY_R_B,			// RBキー
	JOYKEY_NONE0,		// 割り当てキーなし
	JOYKEY_NONE1,		// 割り当てキーなし
	JOYKEY_A,			// Aボタン
	JOYKEY_B,			// Bボタン
	JOYKEY_X,			// Xボタン
	JOYKEY_Y,			// Yボタン
	JOYKEY_L_STICK,		// Lスティック
	JOYKEY_R_STICK,		// Rスティック
	MAX_JOYKEY,			// 最大数
}JOYKEY;

//------------------------------------
// ジョイスティックの構造体
//------------------------------------
typedef struct
{
	int nTypeJoyStick;	// スティックのタイプ
	int nStickDeadLine;	// スティックの判定領域
	float fStickAngle;	// スティックの倒された角度
	bool bUse;			// スティックが使われているかの判定
}JOYSTICK;

//------------------------------------
// プロトタイプ宣言
//------------------------------------
// キーボード関数
HRESULT InitKeyboard(HINSTANCE hInstance, HWND hWnd);	// キーボードの初期化処理
void UninitKeyboard(void);								// キーボードの終了処理
void UpdateKeyboard(void);								// キーボードの更新処理
bool GetKeyboardPress(int nKey);						// キーボードのPress情報取得
bool GetKeyboardTrigger(int nKey);						// キーボードのTrigger情報取得
bool GetKeyboardRelease(int nKey);						// キーボードのRelease情報取得

// パッド関数
HRESULT InitJoypad(void);					// ジョイパッドの初期化処理
void UninitJoypad(void);					// ジョイパッドの終了処理
void UpdateJoypad(void);					// ジョイパッドの更新処理
bool GetJoypadPress(JOYKEY key);			// ジョイパッドのPress情報取得
bool GetJoypadTrigger(JOYKEY key);			// ジョイパッドのTrigger情報取得
bool GetJoypadRelease(JOYKEY key);			// ジョイパッドのRelease情報取得
D3DXVECTOR3 GetJoypadStick(JOYKEY nKey);	// ジョイパッドのスティック情報取得
float GetJoyStickAngle(void);				// ジョイパッドの角度情報取得
bool IsUseJoyPad(void);						// ジョイパッドを使用しているかどうか

#endif //  _INPUT_H_
