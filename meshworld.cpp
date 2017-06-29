/*******************************************************************************
* �^�C�g�� �O�s�`�揈��
* �t�@�C���� meshworld.cpp
* �쐬�� AT-12C-245 �H�ԗY��
* �쐬�� 2015/12/10
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "meshfield.h"
#include "meshworld.h"
#include "debugProc.h"


/*******************************************************************************
* �\���̒�`
*******************************************************************************/


/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/



/*******************************************************************************
* �֐����FCMeshworld
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CMeshworld::CMeshworld(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* �֐����F~CMeshworld
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CMeshworld::~CMeshworld( ) {

}

/*******************************************************************************
* �֐����FInitMeshworld
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�O�s�ݒ�̏�����
*******************************************************************************/

void CMeshworld::Init(void) {
	// �ϐ��錾
	int i, j; // for���[�v�p
	float angle_xz, angle_y;
	VERTEX_3D *pVtx;
	WORD *pIdx;

	// �ϐ��̎擾
	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer = CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾

	// �����o�ϐ��̏�����
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pVtxBuffLid = NULL;
	m_pIdxBuff = NULL;

	// �p�x�v�Z
	angle_xz = 2 * D3DX_PI / MESHWORLD_X_BLOCK;
	angle_y = 0.5f * D3DX_PI / (MESHWORLD_Y_BLOCK + 1);

	// �e�N�X�`���ݒ�
	D3DXCreateTextureFromFile(device, MESHWORLD_TEXTURE_NAME, &m_pTexture);

	// ���_�o�b�t�@�̐���
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * MESHWORLD_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);


	// ���_���ւ̃|�C���^���擾
	m_pVtxBuff -> Lock(0, 0, (void**)&pVtx, 0);

	// �O�s�̏����ݒ�
	m_meshworld.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_meshworld.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_meshworld.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	for(i = 0; i < MESHWORLD_VERTEX_NUM; i++) {
		// ���W�ݒ�
		pVtx[i].pos = D3DXVECTOR3(
			cosf(angle_xz * (MESHWORLD_X_BLOCK - (i % (MESHWORLD_X_BLOCK + 1) ) ) ) * cosf(angle_y * (MESHWORLD_Y_BLOCK - (i / (MESHWORLD_X_BLOCK + 1) ) ) ) * MESHWORLD_DISTANCE,	// X���W
			sinf(angle_y * (MESHWORLD_Y_BLOCK - (i / (MESHWORLD_X_BLOCK + 1) ) ) ) * MESHWORLD_DISTANCE,																			// Y���W
			sinf(angle_xz * (MESHWORLD_X_BLOCK - (i % (MESHWORLD_X_BLOCK + 1) ) ) ) * cosf(angle_y * (MESHWORLD_Y_BLOCK - (i / (MESHWORLD_X_BLOCK + 1) ) ) ) * MESHWORLD_DISTANCE	// Z���W
		);

		// ���_�F�ݒ�
		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �@���ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// �e�N�X�`��UV���W�ݒ�
		if(i / (MESHWORLD_X_BLOCK + 1) >= MESHWORLD_TEXTURE_START_Y) {
			pVtx[i].tex = D3DXVECTOR2( (1 / (float)MESHWORLD_TEXTURE_X_BLOCK) * (i % (MESHWORLD_X_BLOCK + 1) ), (1 / (float)MESHWORLD_TEXTURE_Y_BLOCK) * (i / (MESHWORLD_X_BLOCK + 1) ) + MESHWORLD_TEXTURE_ADJUST);
		}
		else {
			pVtx[i].tex = D3DXVECTOR2( (1 / (float)MESHWORLD_TEXTURE_X_BLOCK) * (i % (MESHWORLD_X_BLOCK + 1) ), (1 / (float)MESHWORLD_TEXTURE_Y_BLOCK) * MESHWORLD_TEXTURE_START_Y + MESHWORLD_TEXTURE_ADJUST);
		}
	}

	m_pVtxBuff -> Unlock( );

	// �C���f�b�N�X�o�b�t�@�m��
	device -> CreateIndexBuffer(sizeof(WORD) * MESHWORLD_INDEX_NUM,
								 D3DUSAGE_WRITEONLY,
								 D3DFMT_INDEX16,
								 D3DPOOL_MANAGED,
								 &m_pIdxBuff,
								 NULL);

	m_pIdxBuff -> Lock(0, 0, (void**) &pIdx, 0);

	for(i = 0; i < MESHWORLD_Y_BLOCK; i++) {
		if(i != 0) {
			pIdx[i * (MESHWORLD_X_BLOCK + 2) * 2 - 1] = (i + 1) * (MESHWORLD_X_BLOCK + 1);
		}

		for(j = 0; j < (MESHWORLD_X_BLOCK + 1) * 2; j++) {
			pIdx[i * (MESHWORLD_X_BLOCK + 2) * 2 + j] = (i + 1 - j % 2) * (MESHWORLD_X_BLOCK + 1) + (j / 2);
		}

		if(i < MESHWORLD_Y_BLOCK - 1) {
			pIdx[i * (MESHWORLD_X_BLOCK + 2) * 2 + j] = (i + 1 - (j - 1) % 2) * (MESHWORLD_X_BLOCK + 1) + ( (j - 1) / 2);
		}
	}

	m_pIdxBuff -> Unlock( );

	// ���_�o�b�t�@�̐���(�W)
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * MESHWORLD_VERTEX_FAN_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffLid, NULL);


	// ���_���ւ̃|�C���^���擾
	m_pVtxBuffLid -> Lock(0, 0, (void**)&pVtx, 0);

		// ���_�ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, MESHWORLD_DISTANCE, 0.0f);
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		pVtx[0].tex = D3DXVECTOR2(0.0f, MESHWORLD_TEXTURE_ADJUST);

		for(i = 0; i < MESHWORLD_VERTEX_FAN_NUM - 1; i++) {
			// �ʒu�ݒ�
			pVtx[i + 1].pos = D3DXVECTOR3(
				cosf(angle_xz * i) * cosf(angle_y * MESHWORLD_Y_BLOCK) * MESHWORLD_DISTANCE, // X���W
				sinf(angle_y * MESHWORLD_Y_BLOCK) * MESHWORLD_DISTANCE,						 // Y���W
				sinf(angle_xz * i) * cosf(angle_y * MESHWORLD_Y_BLOCK) * MESHWORLD_DISTANCE  // Z���W
			);

			// ���_�F�ݒ�
			pVtx[i + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			// �@���ݒ�
			pVtx[i + 1].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

			// �e�N�X�`���ݒ�
			pVtx[i + 1].tex = D3DXVECTOR2( (1 / (float)MESHWORLD_TEXTURE_X_BLOCK) * (i % (MESHWORLD_X_BLOCK + 1) ), MESHWORLD_TEXTURE_ADJUST);
		}

	m_pVtxBuff -> Unlock( );
}


/*******************************************************************************
* �֐����FUninitMeshworld
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�O�s�̏I������
*******************************************************************************/

void CMeshworld::Uninit(void) {
	if(m_pTexture != NULL) {
		m_pTexture -> Release( ); // �e�N�X�`���̊J��
		m_pTexture = NULL;
	}

	if(m_pVtxBuff != NULL) {
		m_pVtxBuff -> Release( );
		m_pVtxBuff = NULL;
	}

	if(m_pVtxBuffLid != NULL) {
		m_pVtxBuffLid -> Release( );
		m_pVtxBuffLid = NULL;
	}

	if(m_pIdxBuff != NULL) {
		m_pIdxBuff -> Release( );
		m_pIdxBuff = NULL;
	}
}


/*******************************************************************************
* �֐����FUpdataMeshworld
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�O�s�̏��X�V
*******************************************************************************/

void CMeshworld::Update(void) {

}


/*******************************************************************************
* �֐����FDrawMeshworld
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�O�s�̕`�揈��
*******************************************************************************/

void CMeshworld::Draw(void) {
	// �ϐ��̎擾
	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer = CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // �v�Z�p

	// ���C�g�ݒ�I�t
	device -> SetRenderState(D3DRS_LIGHTING, FALSE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	// ���[���h�}�g���b�N�X�̍쐬
	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_meshworld.scl.x, m_meshworld.scl.y, m_meshworld.scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_meshworld.rot.y, m_meshworld.rot.x, m_meshworld.rot.z); // ������ y, x, z �̏��Ԃɒ��ӁI�I�I ������
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_meshworld.pos.x, m_meshworld.pos.y, m_meshworld.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// �C���f�b�N�X�o�b�t�@���o�C���h
	device -> SetIndices(m_pIdxBuff);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	device -> SetTexture(0, m_pTexture);

	device -> DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		MESHWORLD_VERTEX_NUM,
		0,
		MESHWORLD_TRIANGLE_NUM);

	// �W�̕`��
	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_pVtxBuffLid, 0, sizeof(VERTEX_3D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_3D);

	device->DrawPrimitive(
		D3DPT_TRIANGLEFAN,
		0,
		MESHWORLD_TRIANGLE_FAN_NUM);

	// ���C�g�ݒ�I��
	device -> SetRenderState(D3DRS_LIGHTING, TRUE);
}

/*******************************************************************************
* �֐����FCreate
* �����FCMeshworld : �A�h���X
* �߂�l�F�Ȃ�
* �����F�I�u�W�F�N�g�̐���
*******************************************************************************/

CMeshworld *CMeshworld::Create(void) {
	CMeshworld *meshworld;
	meshworld = new CMeshworld;
	meshworld -> Init( );
	return meshworld;
}