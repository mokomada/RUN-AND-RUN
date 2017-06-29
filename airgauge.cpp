/*******************************************************************************
* �^�C�g�� GM31 2D�|���S���`��
* �t�@�C���� scene2D.cpp
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/04/12
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "airgauge.h"
#include "input.h"


bool CAirgauge::m_drawGaugeFlag = false;
LPDIRECT3DVERTEXBUFFER9 CAirgauge::m_pVtxBuffAirgauge = NULL;

/*******************************************************************************
* �֐����FCAirgauge
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CAirgauge::CAirgauge(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* �֐����F~CAirgauge
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CAirgauge::~CAirgauge( ) {

}


/*******************************************************************************
* �֐����FInit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F����������
*******************************************************************************/

void CAirgauge::Init(void) {
	// �ϐ��̐錾
	int i; // for�����[�v�p
	VERTEX_2D *pVtx;

	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer = CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾

	// �����o�ϐ�������
	m_TextureAirgauge = NULL;
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�N�X�`���ݒ�
	D3DXCreateTextureFromFile(device, NULL, &m_TextureAirgauge);

	// ���_�o�b�t�@�̐���
	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * AIRGAUGE_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffAirgauge, NULL);

	// ���_���ւ̃|�C���^���擾
	m_pVtxBuffAirgauge -> Lock(0, 0, (void**)&pVtx, 0);

	// ���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(AIRGAUGE_X, AIRGAUGE_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(AIRGAUGE_X + AIRGAUGE_WIDTH, AIRGAUGE_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(AIRGAUGE_X, AIRGAUGE_Y + AIRGAUGE_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(AIRGAUGE_X + AIRGAUGE_WIDTH, AIRGAUGE_Y + AIRGAUGE_HEIGHT, 0.0f);

	for(i = 0; i < AIRGAUGE_VERTEX_NUM; i++) {
		pVtx[i].rhw = 1.0f; // �W���ݒ�

		pVtx[i].col = D3DCOLOR_RGBA(64, 64, 255, 255); // ���_�J���[�ݒ�
	}

	// �e�N�X�`��UV���W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuffAirgauge -> Unlock( );

	m_drawGaugeFlag = false;
}


/*******************************************************************************
* �֐����FUninit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�I������
*******************************************************************************/

void CAirgauge::Uninit(void) {
	if(m_TextureAirgauge != NULL) {
		m_TextureAirgauge -> Release( ); // �e�N�X�`���̊J��
		m_TextureAirgauge = NULL;
	}
}


/*******************************************************************************
* �֐����FUpdate
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�X�V����
*******************************************************************************/

void CAirgauge::Update(void) {

}


/*******************************************************************************
* �֐����FDraw
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�`�揈��
*******************************************************************************/

void CAirgauge::Draw(void) {
	if(m_drawGaugeFlag) {
		CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
		renderer= CManager::GetRenderer( ); // �����_���̃A�h���X�擾
		LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾

		// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
		device -> SetStreamSource(0, m_pVtxBuffAirgauge, 0, sizeof(VERTEX_2D) );

		// ���_�t�H�[�}�b�g�̐ݒ�
		device -> SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		device -> SetTexture(0, m_TextureAirgauge);

		// ���C�g�ݒ�I�t
//		device -> SetRenderState(D3DRS_LIGHTING, FALSE);

		// 2D�|���S���̕`��
		device -> DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			AIRGAUGE_TRIANGLE_NUM);

		// ���C�g�ݒ�I��
//		device -> SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}


/*******************************************************************************
* �֐����FCreate
* �����Fscene2D : �A�h���X
* �߂�l�F�Ȃ�
* �����F�I�u�W�F�N�g�̐���
*******************************************************************************/

CAirgauge *CAirgauge::Create(void) {
	CAirgauge *scene2D;
	scene2D = new CAirgauge;
	scene2D -> Init( );
	return scene2D;
}


/*******************************************************************************
* �֐����FSetGauge
* �����Fheight : �Q�[�W�̍���
* �߂�l�F�Ȃ�
* �����F�Q�[�W�̍����ݒ�
*******************************************************************************/

void CAirgauge::SetGauge(float height) {
	if(height >= 100) {
		m_drawGaugeFlag = false;
	}
	else {
		m_drawGaugeFlag = true;

		VERTEX_2D *pVtx;

		// ���_���ւ̃|�C���^���擾
		m_pVtxBuffAirgauge -> Lock(0, 0, (void**)&pVtx, 0);

		// ���W�ݒ�
		pVtx[0].pos = D3DXVECTOR3(AIRGAUGE_X, AIRGAUGE_Y - height, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(AIRGAUGE_X + AIRGAUGE_WIDTH, AIRGAUGE_Y - height, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(AIRGAUGE_X, AIRGAUGE_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(AIRGAUGE_X + AIRGAUGE_WIDTH, AIRGAUGE_Y, 0.0f);

		m_pVtxBuffAirgauge -> Unlock( );
	}
}