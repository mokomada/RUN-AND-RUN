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
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "manager.h"
#include "input.h"


/*******************************************************************************
* �֐����FCScene2D
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CScene2D::CScene2D(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* �֐����F~CScene2D
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CScene2D::~CScene2D( ) {

}


/*******************************************************************************
* �֐����FInit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F����������
*******************************************************************************/

void CScene2D::Init(void) {
	// �ϐ��̐錾
	int i; // for�����[�v�p
	VERTEX_2D *pVtx;

	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer = CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾

	// �����o�ϐ�������
	m_TexturePolygon = NULL;
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// �e�N�X�`���ݒ�
	D3DXCreateTextureFromFile(device, "data/TEXTURE/akira000.png", &m_TexturePolygon);

	// ���_�o�b�t�@�̐���
	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_VertexBuffer, NULL);

	// ���_���ւ̃|�C���^���擾
	m_VertexBuffer -> Lock(0, 0, (void**)&pVtx, 0);

	// ���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(POLYGON_X, POLYGON_Y, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(POLYGON_X + POLYGON_WIDTH, POLYGON_Y, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(POLYGON_X, POLYGON_Y + POLYGON_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(POLYGON_X + POLYGON_WIDTH, POLYGON_Y + POLYGON_HEIGHT, 0.0f);

	for(i = 0; i < VERTEX_NUM; i++) {
		pVtx[i].rhw = 1.0f; // �W���ݒ�

		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 0); // ���_�J���[�ݒ�
	}

	// �e�N�X�`��UV���W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_VertexBuffer -> Unlock( );
}


/*******************************************************************************
* �֐����FUninit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�I������
*******************************************************************************/

void CScene2D::Uninit(void) {
	if(m_TexturePolygon != NULL) {
		m_TexturePolygon -> Release( ); // �e�N�X�`���̊J��
		m_TexturePolygon = NULL;
	}
}


/*******************************************************************************
* �֐����FUpdate
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�X�V����
*******************************************************************************/

void CScene2D::Update(void) {
	VERTEX_2D *pVtx;

	m_position.x += 0.1f;

	// ���_���ւ̃|�C���^���擾
	m_VertexBuffer -> Lock(0, 0, (void**)&pVtx, 0);

	// ���W�ݒ�
	pVtx[0].pos += m_position;
	pVtx[1].pos += m_position;
	pVtx[2].pos += m_position;
	pVtx[3].pos += m_position;

	m_VertexBuffer -> Unlock( );



}


/*******************************************************************************
* �֐����FDraw
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�`�揈��
*******************************************************************************/

void CScene2D::Draw(void) {
	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer= CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_VertexBuffer, 0, sizeof(VERTEX_2D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	device -> SetTexture(0, m_TexturePolygon);

	// ���C�g�ݒ�I�t
	device -> SetRenderState(D3DRS_LIGHTING, FALSE);

	// 2D�|���S���̕`��
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		TRIANGLE_NUM);

	// ���C�g�ݒ�I��
	device -> SetRenderState(D3DRS_LIGHTING, TRUE);

}


/*******************************************************************************
* �֐����FCreate
* �����Fscene2D : �A�h���X
* �߂�l�F�Ȃ�
* �����F�I�u�W�F�N�g�̐���
*******************************************************************************/

CScene2D *CScene2D::Create(void) {
	CScene2D *scene2D;
	scene2D = new CScene2D;
	scene2D -> Init( );
	return scene2D;
}