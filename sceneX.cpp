/*******************************************************************************
* �^�C�g�� GM31 X�t�@�C���`��
* �t�@�C���� sceneX.cpp
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/04/26
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "sceneX.h"
#include "camera.h"
#include "input.h"
#include "meshfield.h"
#include "debugProc.h"
#include "sound.h"
#include "score.h"


//*****************************************************************************
// �X�^�e�B�b�N�ϐ��̐���
//*****************************************************************************



/*******************************************************************************
* �֐����FCSceneX
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CSceneX::CSceneX(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* �֐����F~CSceneX
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CSceneX::~CSceneX( ) {

}


/*******************************************************************************
* �֐����FInit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F����������
*******************************************************************************/

void CSceneX::Init(void) {

}


/*******************************************************************************
* �֐����FUninit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�I������
*******************************************************************************/

void CSceneX::Uninit(void) {

}


/*******************************************************************************
* �֐����FUpdate
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�X�V����
*******************************************************************************/

void CSceneX::Update(void) {

}


/*******************************************************************************
* �֐����FDraw
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�`�揈��
*******************************************************************************/

void CSceneX::Draw(void) {

}


/*******************************************************************************
* �֐����FCreate
* �����FsceneX : �A�h���X
* �߂�l�F�Ȃ�
* �����F�I�u�W�F�N�g�̐���
*******************************************************************************/

CSceneX *CSceneX::Create(void) {
	CSceneX *sceneX;
	sceneX = new CSceneX;
	sceneX -> Init( );
	return sceneX;
}

/*******************************************************************************
* �֐����FSetNewWay
* �����Fset : �ݒ肷��l
* �߂�l�F�Ȃ�
* �����F�w�肳�ꂽ�����ւ̈ړ��ʂ��v�Z����
*******************************************************************************/

void CSceneX::SetNewWay(float set) {
	// �J�������擾
	CCamera *get_camera;
	get_camera = CManager::GetCamera( );
	CAMERA camera = get_camera -> GetCameraData( );

	// �i�s�����̐ݒ�
	m_newRot = -camera.rot.y - set;

	// ���፷�ɂ��ړ��ʌ����v�Z
	// ���݈ʒu�̃t�B�[���h�̖@�����擾
	D3DXVECTOR3 field_norm = CMeshfield::GetFieldNorm(m_model.pos);
	// �����Ȗ@���ƃt�B�[���h�̖@���������p�x�Ƃɑ΂��Acos�Ƃ����߂�B�ǂ�������K������Ă���̂� ����/(����A * ����B) �̊��Z�ȉ��͏ȗ�
	float norm_cosangle = D3DXVec3Dot(&field_norm, &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );

	PrintDebugProc("cos�ƌv�Z�O(%f)\n", norm_cosangle);


	// �t�B�[���h�̌X���Ɛi�s��������A�����ʂ̊��������߂�
	D3DXVECTOR3 field_xynorm;
	// �t�B�[���h�@����XZ�������������o�����K��
	D3DXVec3Normalize(&field_xynorm, &D3DXVECTOR3(field_norm.x, 0.0f, field_norm.z) );

	// �ړ��ʂ��v�Z���Đ��K��
	D3DXVECTOR3 player_move;
	D3DXVec3Normalize(&player_move, &D3DXVECTOR3(-sinf(D3DX_PI * (camera.rot.y + set) ), 0.0f, cosf(D3DX_PI * (camera.rot.y + set) ) ) );
	
	PrintDebugProc("�v���C���[�ړ���(%f, %f, %f)\n", player_move.x, player_move.y, player_move.z);

	// �v���C���[�̈ړ��ʂƃt�B�[���h�@����xy�����̓��ρB���ɐ��K������Ă���̂ŏ�L���l�㔼�ȗ�
	float xynorm_cosangle = D3DXVec3Dot(&field_xynorm, &player_move);

	// ���߂����������Ō����ʂ𒲐�
	norm_cosangle = 1 - ( (1 - norm_cosangle) * abs(xynorm_cosangle) );

	PrintDebugProc("cos�ƌv�Z��(%f)\n", norm_cosangle);

	// �ړ��ʉ��Z
	m_model.move.x += sinf(D3DX_PI * (camera.rot.y + set) ) * MODEL_MOVE_SPEED * norm_cosangle;
	m_model.move.z -= cosf(D3DX_PI * (camera.rot.y + set) ) * MODEL_MOVE_SPEED * norm_cosangle;
}