/*******************************************************************************
* �t�@�C���� manager.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/05/11
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _MANAGER_H_
#define _MANAGER_H_


#include <time.h>

/*******************************************************************************
* �}�N����`
*******************************************************************************/


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

enum MODE{
	MODE_TITLE = 0,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_RESULT
};


/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/

class CRenderer; // �O���錾
class CInput; // �O���錾
class CSound; // �O���錾
class CMode; // �O���錾
class CFade; // �O���錾
class CCamera; // �O���錾

/*******************************************************************************
* �}�l�[�W���N���X
*******************************************************************************/

class CManager {
private:
	static CRenderer *m_renderer; // �����_���[
	static CInput *m_input; // ���͏��
	static CSound *m_sound; // �T�E���h���
	static CMode *m_mode; // ���[�h���
	static CFade *m_fade; // �t�F�[�h���
	static CCamera *m_camera; // �J�������

public:
	CManager( );
	~CManager( );

	static HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	static void Uninit(void);
	static void Update(void);
	static void Draw(void);

	static CRenderer *GetRenderer(void);

	static CInput *GetInput(void);
	static void SetMode(CMode *mode);
	static CMode *GetMode(void);
	static CFade *GetFade(void);
	static CCamera *GetCamera(void);

	static bool m_firstTimeFlag; // 1���ڃt���O�i�`���[�g���A���̕\�����ԊǗ��p�j
};


#endif
/*** �C���N���[�h�K�[�h�I�� ***/