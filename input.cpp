


/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "main.h"
#include "renderer.h"
#include "input.h"


/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/


/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/


/*******************************************************************************
* �֐����FCInput
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CInput::CInput( ) {

}


/*******************************************************************************
* �֐����F~CInput
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CInput::~CInput( ) {

}


/*******************************************************************************
* �֐����FInitInput
* �����FhInstance, hWnd
* �߂�l�FE_FAIL or S_OK : ����������ɍs��ꂽ���ۂ�
* �����F���͐ݒ�̏���������
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
* �֐����FUninitInput
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F���͐ݒ�̏I������
*******************************************************************************/

void CInput::UninitInput(void) {
	if(m_Input != NULL) {
		m_Input -> Release( );
		m_Input = NULL;
	}
}


/*******************************************************************************
* �֐����FUpdateInput
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F���͐ݒ�̍X�V����
*******************************************************************************/

void CInput::UpdateInput(void) {

}


/*******************************************************************************
* �֐����FInitKeyboard
* �����FhInstance, hWnd
* �߂�l�FE_FAIL or S_OK : ����������ɍs��ꂽ���ۂ�
* �����F�L�[�{�[�h���͐ݒ�̏���������
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

	m_DevKeyboard -> Acquire( ); // �A�N�Z�X�����擾

	return S_OK;
}


/*******************************************************************************
* �֐����FUninitKeyboard
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�L�[�{�[�h���͐ݒ�̏I������
*******************************************************************************/

void CInput::Uninit(void) {
	if(m_DevKeyboard != NULL) {
		m_DevKeyboard -> Unacquire( ); // �A�N�Z�X�����J��
		m_DevKeyboard -> Release( );
		m_DevKeyboard = NULL;
	}
	UninitInput( );
}


/*******************************************************************************
* �֐����FUpdateKeyboard
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�L�[�{�[�h���͐ݒ�̍X�V����
*******************************************************************************/

void CInput::Update(void) {
	BYTE aKeyState[256];
	if(SUCCEEDED(m_DevKeyboard -> GetDeviceState(sizeof(aKeyState), &aKeyState[0]) ) ) {
		for(int nCntKey = 0; nCntKey < 256; nCntKey++) {
			m_KeyStateTrigger[nCntKey] = aKeyState[nCntKey] & ~m_KeyState[nCntKey]; // �g���K�[����
			m_KeyStateRelease[nCntKey] = ~aKeyState[nCntKey] & m_KeyState[nCntKey]; // �����[�X����

/*			if(m_KeyStateRepeatCount[nCntKey]++ && (m_KeyStateTrigger[nCntKey] || (aKeyState[nCntKey] && m_KeyStateRepeatCount[nCntKey] > 20) ) ) {
				m_KeyStateRepeat[nCntKey] = aKeyState[nCntKey];
			}
			if(m_KeyStateRelease[nCntKey] ) {
				m_KeyStateRepeatCount[nCntKey] = 0;
				m_KeyStateRepeat[nCntKey] = aKeyState[nCntKey];
			}*/ // ���s�[�g�����i�Z���Ȃ�Ȃ����������j
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
* �֐����FGetKeyboardPress
* �����FnKey : ���͏󋵂�m�肽���L�[�̔ԍ�
* �߂�l�Ftrue or false : �m�肽���L�[��������Ă��邩�ۂ�
* �����F�L�[�̓��͔����Ԃ�
*******************************************************************************/

bool CInput::GetKeyboardPress(int nKey) {
	return(m_KeyState[nKey] & 0x80) ? true : false;
}


/*******************************************************************************
* �֐����FGetKeyboardTrigger
* �����FnKey : �g���K�[�󋵂�m�肽���L�[�̔ԍ�
* �߂�l�Ftrue or false : �m�肽���L�[�̃g���K�[��
* �����F�L�[�̃g���K�[�����Ԃ�
*******************************************************************************/

bool CInput::GetKeyboardTrigger(int nKey) {
	return(m_KeyStateTrigger[nKey] & 0x80) ? true : false;
}


/*******************************************************************************
* �֐����FGetKeyboardRelease
* �����FnKey : �����[�X�󋵂�m�肽���L�[�̔ԍ�
* �߂�l�Ftrue or false : �m�肽���L�[�̃����[�X��
* �����F�L�[�̃����[�X�����Ԃ�
*******************************************************************************/

bool CInput::GetKeyboardRelease(int nKey) {
	return(m_KeyStateRelease[nKey] & 0x80) ? true : false;
}


/*******************************************************************************
* �֐����FGetKeyboardRepeat
* �����FnKey : ���s�[�g�󋵂�m�肽���L�[�̔ԍ�
* �߂�l�Ftrue or false : �m�肽���L�[�̃��s�[�g��
* �����F�L�[�̃��s�[�g�����Ԃ�
*******************************************************************************/

bool CInput::GetKeyboardRepeat(int nKey) {
	return(m_KeyStateRepeat[nKey] & 0x80) ? true : false;
}
