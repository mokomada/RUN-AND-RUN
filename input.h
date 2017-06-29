

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _INPUT_H_
#define _INPUT_H_


/*******************************************************************************
* �C���v�b�g�N���X
*******************************************************************************/

class CInput{
private:
	HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
	void UninitInput(void);
	void UpdateInput(void);

	LPDIRECTINPUT8 m_Input; // DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8 m_DevKeyboard; // ���̓f�o�C�X�i�L�[�{�[�h�j�ւ̃|�C���^
	BYTE m_KeyState[256]; // �L�[�{�[�h�̓��͏�񃏁[�N
	BYTE m_KeyStateTrigger[256]; // �L�[�g���K�[���
	BYTE m_KeyStateRelease[256]; // �L�[�����[�X���
	BYTE m_KeyStateRepeat[256]; // �L�[���s�[�g���
	int m_KeyStateRepeatCount[256]; // �L�[���s�[�g�J�E���^

public:
	CInput( ); // �R���X�g���N�^
	~CInput( ); // �f�X�g���N�^

	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit(void);
	void Update(void);
	bool GetKeyboardPress(int nKey);
	bool GetKeyboardTrigger(int nKey);
	bool GetKeyboardRelease(int nKey);
	bool GetKeyboardRepeat(int nKey);
};

#endif
/*** �C���N���[�h�K�[�h�I�� ***/