/*******************************************************************************
* �^�C�g�� �}�l�[�W���[�N���X
* �t�@�C���� manager.cpp
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/05/11
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "mode.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "title.h"
#include "fade.h"
#include "game.h"
#include "tutorial.h"


/*******************************************************************************
* �X�^�e�B�b�N�ϐ�������
*******************************************************************************/

CRenderer *CManager::m_renderer = NULL; // �����_���[
CInput *CManager::m_input = NULL; // ���͏��
CSound *CManager::m_sound = NULL; // �T�E���h���
CMode *CManager::m_mode = NULL; // ���[�h���
CFade *CManager::m_fade = NULL; // �t�F�[�h���
CCamera *CManager::m_camera = NULL; // �J�������
bool CManager::m_firstTimeFlag = true; // 1�T�ڃt���O



/*******************************************************************************
* �֐����FCManager
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CManager::CManager( ) {

}


/*******************************************************************************
* �֐����F~CManager
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CManager::~CManager( ) {

}


/*******************************************************************************
* �֐����FInit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�J�����̏�����
*******************************************************************************/

HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow) {
	// �����ݒ�
	srand( (unsigned) time(NULL) );

	// �I�u�W�F�N�g����
	m_renderer = new CRenderer;
	m_renderer -> Init(hInstance, hWnd, bWindow);

	m_input = new CInput;
	m_input -> Init(hInstance, hWnd);

	m_sound = new CSound;
	m_sound -> Init(hWnd);

	m_fade = new CFade;
	m_fade -> Init( );

	m_camera = new CCamera;
	m_camera -> Init( );

	SetMode(new CTitle);

	return S_OK;
}


/*******************************************************************************
* �֐����FUninit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�J�����̏I������
*******************************************************************************/

void CManager::Uninit(void) {
	// �I�u�W�F�N�g�j��
	if(m_renderer != NULL) {
		m_renderer -> Uninit( );
		delete m_renderer;
		m_renderer = NULL;
	}

	if(m_input != NULL) {
		m_input -> Uninit( );
		delete m_input;
		m_input = NULL;
	}

	if(m_sound != NULL) {
		m_sound -> Uninit( );
		delete m_sound;
		m_sound = NULL;
	}

	if(m_fade != NULL) {
		m_fade -> Uninit( );
		delete m_fade;
		m_fade = NULL;
	}

	if(m_camera != NULL) {
		m_camera -> Uninit( );
		delete m_camera;
		m_camera = NULL;
	}
}

/*******************************************************************************
* �֐����FUpdate
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�J�����̍X�V����
*******************************************************************************/

void CManager::Update(void) {
	m_input -> Update( );

	m_renderer -> Update( );

	m_mode -> Update( );

	m_fade -> Update( );
}


/*******************************************************************************
* �֐����FDraw
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�`�揈��
*******************************************************************************/

void CManager::Draw(void) {

/*#ifdef _DEBUG
	DrawFPS( );
#endif*/
	m_renderer -> StartDraw( );

	m_mode -> Draw( );

	m_fade -> Draw( );

	m_renderer -> EndDraw( );
}


/*******************************************************************************
* �֐����FGetRenderer
* �����F�Ȃ�
* �߂�l�Fm_renderer
* �����Fm_renderer�̃A�h���X��Ԃ�
*******************************************************************************/

CRenderer *CManager::GetRenderer(void) {
	return m_renderer;
}


/*******************************************************************************
* �֐����FGetInput
* �����F�Ȃ�
* �߂�l�Fm_input
* �����Fm_input�̃A�h���X��Ԃ�
*******************************************************************************/

CInput *CManager::GetInput(void) {
	return m_input;
}


/*******************************************************************************
* �֐����FGetMode
* �����F�Ȃ�
* �߂�l�F
* �����F
*******************************************************************************/

CMode *CManager::GetMode(void) {
	return m_mode;
}

/*******************************************************************************
* �֐����FGetFade
* �����F�Ȃ�
* �߂�l�F
* �����F
*******************************************************************************/

CFade *CManager::GetFade(void) {
	return m_fade;
}


/*******************************************************************************
* �֐����FGetCamera
* �����F�Ȃ�
* �߂�l�F
* �����F
*******************************************************************************/

CCamera *CManager::GetCamera(void) {
	return m_camera;
}


/*******************************************************************************
* �֐����FSetMode
* �����F
* �߂�l�F�Ȃ�
* �����F
*******************************************************************************/

void CManager::SetMode(CMode *mode) {
	if(m_mode != NULL) {
		m_mode -> Uninit( );
		delete m_mode;
	}

	m_mode = mode;
	m_mode -> Init( );
}

