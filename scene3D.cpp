/*******************************************************************************
* �^�C�g�� GM31 3D�|���S���`��
* �t�@�C���� scene3D.cpp
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/04/12
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene3D.h"
#include "manager.h"
#include "input.h"


/*******************************************************************************
* �֐����FCScene3D
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CScene3D::CScene3D(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* �֐����F~CScene3D
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CScene3D::~CScene3D( ) {

}


/*******************************************************************************
* �֐����FInit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F����������
*******************************************************************************/

void CScene3D::Init(void) {
	// �ϐ��̐錾
	int i; // for�����[�v�p
	VERTEX_3D *pVtx;
	D3DXVECTOR3 n0, v1, v2;

	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer = CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾

	// �����o�ϐ�������
	m_TexturePolygon = NULL;
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotation = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_scale = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	// �e�N�X�`���ݒ�
	D3DXCreateTextureFromFile(device, FIELD_TEXTURE_NAME, &m_TexturePolygon);

	// ���_�o�b�t�@�̐���
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_VertexBuffer, NULL);

	// ���_���ւ̃|�C���^���擾
	m_VertexBuffer -> Lock(0, 0, (void**)&pVtx, 0);

	// ���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(FIELD_X, 0.0f, FIELD_Z + FIELD_HEIGHT);
	pVtx[1].pos = D3DXVECTOR3(FIELD_X + FIELD_WIDTH, 100.0f, FIELD_Z + FIELD_HEIGHT);
	pVtx[2].pos = D3DXVECTOR3(FIELD_X, 100.0f, FIELD_Z);
	pVtx[3].pos = D3DXVECTOR3(FIELD_X + FIELD_WIDTH, 0.0f, FIELD_Z);

	// �@���v�Z
	v1 = pVtx[1].pos - pVtx[0].pos;
	v2 = pVtx[2].pos - pVtx[0].pos;

	D3DXVec3Cross(&n0, &v1, &v2);
	D3DXVec3Normalize(&n0, &n0);

	pVtx[0].nor = n0;

	v1 = pVtx[2].pos - pVtx[3].pos;
	v2 = pVtx[1].pos - pVtx[3].pos;

	D3DXVec3Cross(&n0, &v1, &v2);
	D3DXVec3Normalize(&n0, &n0);

	pVtx[3].nor = n0;

	n0 = (pVtx[0].nor + pVtx[3].nor) * 0.5f;
	D3DXVec3Normalize(&n0, &n0);
	pVtx[1].nor = n0;
	pVtx[2].nor = n0;


	for(i = 0; i < VERTEX_NUM; i++) {
		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255); // ���_�J���[�ݒ�
	}


	// �e�N�X�`��UV���W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(6.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 6.0f);
	pVtx[3].tex = D3DXVECTOR2(6.0f, 6.0f);

	m_VertexBuffer -> Unlock( );

}


/*******************************************************************************
* �֐����FUninit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�I������
*******************************************************************************/

void CScene3D::Uninit(void) {
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

void CScene3D::Update(void) {
//	VERTEX_3D *pVtx;
	CInput *input;
	input = CManager::GetInput( ); // ���͏��擾

//	m_position.x += 0.1f;

	// ���_���ւ̃|�C���^���擾
/*	m_VertexBuffer -> Lock(0, 0, (void**)&pVtx, 0);

	// ���W�ݒ�
	pVtx[0].pos += m_position;
	pVtx[1].pos += m_position;
	pVtx[2].pos += m_position;
	pVtx[3].pos += m_position;

	m_VertexBuffer -> Unlock( );*/

	if(input -> GetKeyboardPress(DIK_SPACE) ) {
		Uninit( );
		Release(SCENE3D_PRIORITY_NUM);
	}
}


/*******************************************************************************
* �֐����FDraw
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�`�揈��
*******************************************************************************/

void CScene3D::Draw(void) {
/*	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // �v�Z�p
	D3DXMATRIX mtxView;

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	// ���[���h�}�g���b�N�X�̍쐬
	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rotation.y, m_rotation.x, m_rotation.z); // ������ y, x, z �̏��Ԃɒ��ӁI�I�I ������
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_position.x, m_position.y, m_position.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_VertexBuffer, 0, sizeof(VERTEX_3D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	device -> SetTexture(0, m_TexturePolygon);

	// 3D�|���S���̕`��
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		TRIANGLE_NUM);*/
}


/*******************************************************************************
* �֐����FCreate
* �����Fscene3D : �A�h���X
* �߂�l�F�Ȃ�
* �����F�I�u�W�F�N�g�̐���
*******************************************************************************/

CScene3D *CScene3D::Create(void) {
	CScene3D *scene3D;
	scene3D = new CScene3D;
	scene3D -> Init( );
	return scene3D;
}