


/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "main.h"
#include "renderer.h"
#include "input.h"


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/


/*******************************************************************************
* グローバル変数
*******************************************************************************/


/*******************************************************************************
* 関数名：CInput
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CInput::CInput( ) {

}


/*******************************************************************************
* 関数名：~CInput
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CInput::~CInput( ) {

}


/*******************************************************************************
* 関数名：InitInput
* 引数：hInstance, hWnd
* 戻り値：E_FAIL or S_OK : 処理が正常に行われたか否か
* 説明：入力設定の初期化処理
*******************************************************************************/

HRESULT CInput::InitInput(HINSTANCE hInstance, HWND hWnd) {
	if(m_Input == NULL) {
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_Input, NULL) ) ) {
			return E_FAIL;
		}
	}
	return S_OK;
}


/*******************************************************************************
* 関数名：UninitInput
* 引数：なし
* 戻り値：なし
* 説明：入力設定の終了処理
*******************************************************************************/

void CInput::UninitInput(void) {
	if(m_Input != NULL) {
		m_Input -> Release( );
		m_Input = NULL;
	}
}


/*******************************************************************************
* 関数名：UpdateInput
* 引数：なし
* 戻り値：なし
* 説明：入力設定の更新処理
*******************************************************************************/

void CInput::UpdateInput(void) {

}


/*******************************************************************************
* 関数名：InitKeyboard
* 引数：hInstance, hWnd
* 戻り値：E_FAIL or S_OK : 処理が正常に行われたか否か
* 説明：キーボード入力設定の初期化処理
*******************************************************************************/

HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd) {
	m_Input = NULL;
	m_DevKeyboard = NULL;

	if(FAILED(InitInput(hInstance, hWnd) ) ) {
		return E_FAIL;
	}

	if(FAILED(m_Input -> CreateDevice(GUID_SysKeyboard, &m_DevKeyboard, NULL) ) ) {
		return E_FAIL;
	}

	if(FAILED(m_DevKeyboard -> SetDataFormat(&c_dfDIKeyboard) ) ) {
		return E_FAIL;
	}

	if(FAILED(m_DevKeyboard -> SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE) ) ) ) {
		return E_FAIL;
	}

	m_DevKeyboard -> Acquire( ); // アクセス権を取得

	return S_OK;
}


/*******************************************************************************
* 関数名：UninitKeyboard
* 引数：なし
* 戻り値：なし
* 説明：キーボード入力設定の終了処理
*******************************************************************************/

void CInput::Uninit(void) {
	if(m_DevKeyboard != NULL) {
		m_DevKeyboard -> Unacquire( ); // アクセス権を開放
		m_DevKeyboard -> Release( );
		m_DevKeyboard = NULL;
	}
	UninitInput( );
}


/*******************************************************************************
* 関数名：UpdateKeyboard
* 引数：なし
* 戻り値：なし
* 説明：キーボード入力設定の更新処理
*******************************************************************************/

void CInput::Update(void) {
	BYTE aKeyState[256];
	if(SUCCEEDED(m_DevKeyboard -> GetDeviceState(sizeof(aKeyState), &aKeyState[0]) ) ) {
		for(int nCntKey = 0; nCntKey < 256; nCntKey++) {
			m_KeyStateTrigger[nCntKey] = aKeyState[nCntKey] & ~m_KeyState[nCntKey]; // トリガー処理
			m_KeyStateRelease[nCntKey] = ~aKeyState[nCntKey] & m_KeyState[nCntKey]; // リリース処理

/*			if(m_KeyStateRepeatCount[nCntKey]++ && (m_KeyStateTrigger[nCntKey] || (aKeyState[nCntKey] && m_KeyStateRepeatCount[nCntKey] > 20) ) ) {
				m_KeyStateRepeat[nCntKey] = aKeyState[nCntKey];
			}
			if(m_KeyStateRelease[nCntKey] ) {
				m_KeyStateRepeatCount[nCntKey] = 0;
				m_KeyStateRepeat[nCntKey] = aKeyState[nCntKey];
			}*/ // リピート処理（短くならないか実験中）
			if(aKeyState[nCntKey]) {
				m_KeyStateRepeatCount[nCntKey]++;
				if( (m_KeyStateRepeatCount[nCntKey] == 1 ) || ( m_KeyStateRepeatCount[nCntKey] > 20) ) {
					m_KeyStateRepeat[nCntKey] = aKeyState[nCntKey];
				}
				else {
					m_KeyStateRepeat[nCntKey] = false;
				}
			}
			if(m_KeyStateRelease[nCntKey] ) {
				m_KeyStateRepeatCount[nCntKey] = 0;
				m_KeyStateRepeat[nCntKey] = aKeyState[nCntKey];
			}

			m_KeyState[nCntKey] = aKeyState[nCntKey];
		}
	}
	else {
		for(int nCntKey = 0; nCntKey < 256; nCntKey++) {
			m_KeyState[nCntKey] = false;
			m_KeyStateTrigger[nCntKey] = false;
			m_KeyStateRelease[nCntKey] = false;
			m_KeyStateRepeat[nCntKey] = false;
		}

		m_DevKeyboard -> Acquire( );
	}
}


/*******************************************************************************
* 関数名：GetKeyboardPress
* 引数：nKey : 入力状況を知りたいキーの番号
* 戻り値：true or false : 知りたいキーが押されているか否か
* 説明：キーの入力判定を返す
*******************************************************************************/

bool CInput::GetKeyboardPress(int nKey) {
	return(m_KeyState[nKey] & 0x80) ? true : false;
}


/*******************************************************************************
* 関数名：GetKeyboardTrigger
* 引数：nKey : トリガー状況を知りたいキーの番号
* 戻り値：true or false : 知りたいキーのトリガー状況
* 説明：キーのトリガー判定を返す
*******************************************************************************/

bool CInput::GetKeyboardTrigger(int nKey) {
	return(m_KeyStateTrigger[nKey] & 0x80) ? true : false;
}


/*******************************************************************************
* 関数名：GetKeyboardRelease
* 引数：nKey : リリース状況を知りたいキーの番号
* 戻り値：true or false : 知りたいキーのリリース状況
* 説明：キーのリリース判定を返す
*******************************************************************************/

bool CInput::GetKeyboardRelease(int nKey) {
	return(m_KeyStateRelease[nKey] & 0x80) ? true : false;
}


/*******************************************************************************
* 関数名：GetKeyboardRepeat
* 引数：nKey : リピート状況を知りたいキーの番号
* 戻り値：true or false : 知りたいキーのリピート状況
* 説明：キーのリピート判定を返す
*******************************************************************************/

bool CInput::GetKeyboardRepeat(int nKey) {
	return(m_KeyStateRepeat[nKey] & 0x80) ? true : false;
}
