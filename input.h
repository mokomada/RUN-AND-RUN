

/*** インクルードガード開始 ***/
#ifndef _INPUT_H_
#define _INPUT_H_


/*******************************************************************************
* インプットクラス
*******************************************************************************/

class CInput{
private:
	HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
	void UninitInput(void);
	void UpdateInput(void);

	LPDIRECTINPUT8 m_Input; // DirectInputオブジェクトへのポインタ
	LPDIRECTINPUTDEVICE8 m_DevKeyboard; // 入力デバイス（キーボード）へのポインタ
	BYTE m_KeyState[256]; // キーボードの入力情報ワーク
	BYTE m_KeyStateTrigger[256]; // キートリガー情報
	BYTE m_KeyStateRelease[256]; // キーリリース情報
	BYTE m_KeyStateRepeat[256]; // キーリピート情報
	int m_KeyStateRepeatCount[256]; // キーリピートカウンタ

public:
	CInput( ); // コンストラクタ
	~CInput( ); // デストラクタ

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetKeyboardPress(int nKey);
	bool GetKeyboardTrigger(int nKey);
	bool GetKeyboardRelease(int nKey);
	bool GetKeyboardRepeat(int nKey);
};

#endif
/*** インクルードガード終了 ***/