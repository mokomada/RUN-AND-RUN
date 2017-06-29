/*******************************************************************************
* �^�C�g�� �J��������
* �t�@�C���� camare.cpp
* �쐬�� AT-12C-245 �H�ԗY��
* �쐬�� 2016/04/27
*******************************************************************************/

#include "main.h"
#include "renderer.h"
#include "light.h"
#include "manager.h"


/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/



/*******************************************************************************
* �֐����FCLight
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CLight::CLight( ) {

}


/*******************************************************************************
* �֐����F~CLight
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CLight::~CLight( ) {

}


/*******************************************************************************
* �֐����FInit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�J�����̏�����
*******************************************************************************/

void CLight::Init(void) {
	D3DXVECTOR3 vecDir; // �x�N�g�����K���p

	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer = CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾

	// ���C�g�ݒ�
	device -> SetRenderState(D3DRS_LIGHTING, TRUE); // ��d�� FALSE�őS���C�gOFF

	// ��1���C�g�ݒ�
	ZeroMemory(&m_aLight[0], sizeof(D3DLIGHT9) ); // ������
	m_aLight[0].Type = D3DLIGHT_DIRECTIONAL; // ���s����
	m_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // ���̐F
	vecDir = D3DXVECTOR3(0.5f, -0.5f, 0.5f); // ���̌����i-1.0�`1.0�j
//	vecDir = D3DXVECTOR3(0.0f, -1.0f, -0.0f); // ���̌����i-1.0�`1.0�j
	D3DXVec3Normalize( (D3DXVECTOR3*) &m_aLight[0].Direction, &vecDir); // �x�N�g���𐳋K��
	device -> SetLight(0, &m_aLight[0]); // ���C�g�̐ݒ�
	device -> LightEnable(0, TRUE); // ���C�g��L���ɂ���

	// ��2���C�g�ݒ�
	ZeroMemory(&m_aLight[1], sizeof(D3DLIGHT9) ); // ������
	m_aLight[1].Type = D3DLIGHT_DIRECTIONAL; // ���s����
	m_aLight[1].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f); // ���̐F
	vecDir = D3DXVECTOR3(-0.5f, 0.4f, 0.6f); // ���̌����i-1.0�`1.0�j
	D3DXVec3Normalize( (D3DXVECTOR3*) &m_aLight[1].Direction, &vecDir); // �x�N�g���𐳋K��
	device -> SetLight(1, &m_aLight[1]); // ���C�g�̐ݒ�
	device -> LightEnable(1, TRUE); // ���C�g��L���ɂ���

	// ��3���C�g�ݒ�
	ZeroMemory(&m_aLight[2], sizeof(D3DLIGHT9) ); // ������
	m_aLight[2].Type = D3DLIGHT_DIRECTIONAL; // ���s����
	m_aLight[2].Diffuse = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f); // ���̐F
	vecDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f); // ���̌����i-1.0�`1.0�j
	D3DXVec3Normalize( (D3DXVECTOR3*) &m_aLight[2].Direction, &vecDir); // �x�N�g���𐳋K��
	device -> SetLight(2, &m_aLight[2]); // ���C�g�̐ݒ�
	device -> LightEnable(2, TRUE); // ���C�g��L���ɂ���
}


/*******************************************************************************
* �֐����FUninit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�J�����̏I������
*******************************************************************************/

void CLight::Uninit(void) {

}

/*******************************************************************************
* �֐����FUpdate
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�J�����̍X�V����
*******************************************************************************/

void CLight::Update(void) {

}
