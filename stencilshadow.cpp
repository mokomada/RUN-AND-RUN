/*******************************************************************************
* �^�C�g�� GM31 �I�u�W�F�N�g�Ǘ�
* �t�@�C���� object.cpp
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/09/27
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
#include "stencilshadow.h"
#include "meshfield.h"
#include "player.h"
#include "game.h"


/*******************************************************************************
* �֐����FCStencilShadow
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CStencilShadow::CStencilShadow(int priority, OBJTYPE objType) {

}


/*******************************************************************************
* �֐����F~CStencilShadow
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CStencilShadow::~CStencilShadow( ) {

}


/*******************************************************************************
* �֐����FInit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F����������
*******************************************************************************/

void CStencilShadow::Init(void) {

}

void CStencilShadow::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth) {
	// �ϐ��錾
	int i, j; // for���[�v�p
	float angle_xz;
	VERTEX_2D *pVtx2D;
	VERTEX_3D *pVtx;
	WORD *pIdx;

	// �ϐ��̎擾
	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer = CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾
	CPlayer *get_player = CGame::GetPlayer( );
	MODEL player = get_player -> GetModel( );

	// �����i�[
	m_height = height;
	m_width = width;

	// �����o�ϐ��̏�����
	m_pTexture = NULL;
	m_pVtxBuff = NULL;
	m_pVtxBuffLid = NULL;
	m_pIdxBuff = NULL;

	// �~������
	D3DXCreateCylinder(device, m_width, m_width, m_height, 12, 1, &m_pMesh, NULL);

	// �p�x�v�Z
	angle_xz = 2 * D3DX_PI / STENCILSHADOW_X_BLOCK;

	// �e�N�X�`���ݒ�
	D3DXCreateTextureFromFile(device, STENCILSHADOW_TEXTURE_NAME, &m_pTexture);

	// ���_�o�b�t�@�̐���
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * STENCILSHADOW_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);


	// ���_���ւ̃|�C���^���擾
	m_pVtxBuff -> Lock(0, 0, (void**)&pVtx, 0);

	// �O�s�̏����ݒ�
	m_stencilshadow.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_stencilshadow.rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_stencilshadow.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	for(i = 0; i < STENCILSHADOW_VERTEX_NUM; i++) {
		// ���W�ݒ�
		pVtx[i].pos = D3DXVECTOR3(
			cosf(angle_xz * (i % (STENCILSHADOW_X_BLOCK + 1) ) ) * m_width,	// X���W
			(STENCILSHADOW_Y_BLOCK - (i / (STENCILSHADOW_X_BLOCK + 1) ) ) * m_height / STENCILSHADOW_Y_BLOCK - m_height * 0.5f, // Y���W
			sinf(angle_xz * (i % (STENCILSHADOW_X_BLOCK + 1) ) ) * m_width	// Z���W
		);

		// ���_�F�ݒ�
		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �@���ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

		// UV���W�ݒ�
		pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);
	}

	m_pVtxBuff -> Unlock( );

	// �C���f�b�N�X�o�b�t�@�m��
	device -> CreateIndexBuffer(sizeof(WORD) * STENCILSHADOW_INDEX_NUM,
								 D3DUSAGE_WRITEONLY,
								 D3DFMT_INDEX16,
								 D3DPOOL_MANAGED,
								 &m_pIdxBuff,
								 NULL);

	m_pIdxBuff -> Lock(0, 0, (void**) &pIdx, 0);

	for(i = 0; i < STENCILSHADOW_Y_BLOCK; i++) {
		if(i != 0) {
			pIdx[i * (STENCILSHADOW_X_BLOCK + 2) * 2 - 1] = (i + 1) * (STENCILSHADOW_X_BLOCK + 1);
		}

		for(j = 0; j < (STENCILSHADOW_X_BLOCK + 1) * 2; j++) {
			pIdx[i * (STENCILSHADOW_X_BLOCK + 2) * 2 + j] = (i + 1 - j % 2) * (STENCILSHADOW_X_BLOCK + 1) + (j / 2);
		}

		if(i < STENCILSHADOW_Y_BLOCK - 1) {
			pIdx[i * (STENCILSHADOW_X_BLOCK + 2) * 2 + j] = (i + 1 - (j - 1) % 2) * (STENCILSHADOW_X_BLOCK + 1) + ( (j - 1) / 2);
		}
	}

	m_pIdxBuff -> Unlock( );

	// ���_�o�b�t�@�̐���(�W)
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * STENCILSHADOW_VERTEX_FAN_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffLid, NULL);


	// ���_���ւ̃|�C���^���擾
	m_pVtxBuffLid -> Lock(0, 0, (void**)&pVtx, 0);

		// ���_�ݒ�
		pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[0].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);

		for(i = 0; i < STENCILSHADOW_VERTEX_FAN_NUM - 1; i++) {
			// �ʒu�ݒ�
			pVtx[i + 1].pos = D3DXVECTOR3(
				cosf(-angle_xz * i) * m_width, // X���W
				0.0f,						 // Y���W
				sinf(-angle_xz * i) * m_width  // Z���W
			);

			// ���_�F�ݒ�
			pVtx[i + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			// �@���ݒ�
			pVtx[i + 1].nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);

			// �e�N�X�`���ݒ�
			pVtx[i + 1].tex = D3DXVECTOR2(0.0f, 0.0f);
		}

	m_pVtxBuff -> Unlock( );


	/* ���_�o�b�t�@�̐��� */
	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * STENCILSHADOW_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffScreen, NULL);

	/* ���_���ւ̃|�C���^���擾 */
	m_pVtxBuffScreen -> Lock(0, 0, (void**)&pVtx2D, 0);

	/* ���W�ݒ� */
	pVtx2D[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx2D[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx2D[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx2D[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);


	for(i = 0; i < STENCILSHADOW_VERTEX_NUM; i++) {
		pVtx2D[i].rhw = 1.0f;

		pVtx2D[i].col = D3DCOLOR_RGBA(0, 0, 0, 128);
	}

	/* �e�N�X�`��UV���W�ݒ� */
	pVtx2D[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2D[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2D[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx2D[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuffScreen -> Unlock( );
}


/*******************************************************************************
* �֐����FUninit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�I������
*******************************************************************************/

void CStencilShadow::Uninit(void) {
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
* �֐����FUpdate
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�X�V����
*******************************************************************************/

void CStencilShadow::Update(void) {
	CPlayer *get_player = CGame::GetPlayer( );
	MODEL player = get_player -> GetModel( );

	m_stencilshadow.pos = player.pos;
	m_stencilshadow.pos.y += (1.0f - m_height * 0.5f);

	m_oldPlayerPos = player.pos;
}


/*******************************************************************************
* �֐����FDraw
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�`�揈��
*******************************************************************************/

void CStencilShadow::Draw(void) {
	// �ϐ��̎擾
	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer = CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // �v�Z�p

	// ���C�g�ݒ�I�t
	device -> SetRenderState(D3DRS_LIGHTING, FALSE);

	// �~���i�\�j�`��
	device -> SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	device -> SetRenderState(D3DRS_STENCILENABLE, TRUE);
	device -> SetRenderState(D3DRS_COLORWRITEENABLE, 0);
	device -> SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	device -> SetRenderState(D3DRS_STENCILREF, 1);
	device -> SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_INCR);
	device -> SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	// ���[���h�}�g���b�N�X�̍쐬
	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_stencilshadow.scl.x, m_stencilshadow.scl.y, m_stencilshadow.scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_stencilshadow.rot.y, m_stencilshadow.rot.x + D3DX_PI * 0.5f, m_stencilshadow.rot.z); // ������ y, x, z �̏��Ԃɒ��ӁI�I�I ������
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_stencilshadow.pos.x, m_stencilshadow.pos.y, m_stencilshadow.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	m_pMesh -> DrawSubset(0);

	// �C���f�b�N�X�o�b�t�@���o�C���h
/*	device -> SetIndices(m_pIdxBuff);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	device -> SetTexture(0, NULL);

	device -> DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		STENCILSHADOW_VERTEX_NUM,
		0,
		STENCILSHADOW_TRIANGLE_NUM);

	// �W�̕`��
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	// ���[���h�}�g���b�N�X�̍쐬
	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_stencilshadow.scl.x, m_stencilshadow.scl.y, m_stencilshadow.scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_stencilshadow.rot.y, m_stencilshadow.rot.x, m_stencilshadow.rot.z); // ������ y, x, z �̏��Ԃɒ��ӁI�I�I ������
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_stencilshadow.pos.x, m_stencilshadow.pos.y + m_height * 0.5f, m_stencilshadow.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_pVtxBuffLid, 0, sizeof(VERTEX_3D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_3D);

	device->DrawPrimitive(
		D3DPT_TRIANGLEFAN,
		0,
		STENCILSHADOW_TRIANGLE_FAN_NUM);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	// ���[���h�}�g���b�N�X�̍쐬
	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_stencilshadow.scl.x, m_stencilshadow.scl.y, m_stencilshadow.scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_stencilshadow.rot.y, m_stencilshadow.rot.x + D3DX_PI, m_stencilshadow.rot.z); // ������ y, x, z �̏��Ԃɒ��ӁI�I�I ������
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_stencilshadow.pos.x, m_stencilshadow.pos.y - m_height * 0.5f, m_stencilshadow.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_pVtxBuffLid, 0, sizeof(VERTEX_3D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_3D);

	device->DrawPrimitive(
		D3DPT_TRIANGLEFAN,
		0,
		STENCILSHADOW_TRIANGLE_FAN_NUM);*/

	// �~���i���j�`��
	device -> SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_DECR);
	device -> SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

	m_pMesh -> DrawSubset(0);

	// ���[���h�}�g���b�N�X�̏�����
/*	D3DXMatrixIdentity(&m_mtxWorld);
	// ���[���h�}�g���b�N�X�̍쐬
	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_stencilshadow.scl.x, m_stencilshadow.scl.y, m_stencilshadow.scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_stencilshadow.rot.y, m_stencilshadow.rot.x + D3DX_PI * 0.5f, m_stencilshadow.rot.z); // ������ y, x, z �̏��Ԃɒ��ӁI�I�I ������
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_stencilshadow.pos.x, m_stencilshadow.pos.y, m_stencilshadow.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);*/

/*	// �C���f�b�N�X�o�b�t�@���o�C���h
	device -> SetIndices(m_pIdxBuff);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	device -> SetTexture(0, NULL);

	device -> DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		STENCILSHADOW_VERTEX_NUM,
		0,
		STENCILSHADOW_TRIANGLE_NUM);

	// �W�̕`��
	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	// ���[���h�}�g���b�N�X�̍쐬
	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_stencilshadow.scl.x, m_stencilshadow.scl.y, m_stencilshadow.scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_stencilshadow.rot.y, m_stencilshadow.rot.x, m_stencilshadow.rot.z); // ������ y, x, z �̏��Ԃɒ��ӁI�I�I ������
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_stencilshadow.pos.x, m_stencilshadow.pos.y + m_height * 0.5f, m_stencilshadow.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_pVtxBuffLid, 0, sizeof(VERTEX_3D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_3D);

	device->DrawPrimitive(
		D3DPT_TRIANGLEFAN,
		0,
		STENCILSHADOW_TRIANGLE_FAN_NUM);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	// ���[���h�}�g���b�N�X�̍쐬
	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, m_stencilshadow.scl.x, m_stencilshadow.scl.y, m_stencilshadow.scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_stencilshadow.rot.y, m_stencilshadow.rot.x + D3DX_PI, m_stencilshadow.rot.z); // ������ y, x, z �̏��Ԃɒ��ӁI�I�I ������
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_stencilshadow.pos.x, m_stencilshadow.pos.y - m_height * 0.5f, m_stencilshadow.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_pVtxBuffLid, 0, sizeof(VERTEX_3D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_3D);

	device->DrawPrimitive(
		D3DPT_TRIANGLEFAN,
		0,
		STENCILSHADOW_TRIANGLE_FAN_NUM);*/

	// �S���2D�|���S���`��
	device -> SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	device -> SetRenderState(D3DRS_COLORWRITEENABLE, 0xf);
	device -> SetRenderState(D3DRS_ZENABLE, FALSE);
	device -> SetRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_pVtxBuffScreen, 0, sizeof(VERTEX_2D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	device -> SetTexture(0, NULL);

	// 2D�|���S���̕`��
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

	device -> SetRenderState(D3DRS_ZENABLE, TRUE);
	device -> SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	device -> SetRenderState(D3DRS_STENCILENABLE, FALSE);

	// ���C�g�ݒ�I��
	device -> SetRenderState(D3DRS_LIGHTING, TRUE);
}


/*******************************************************************************
* �֐����FCreate
* �����Fpos : ���W
*		rot : ��]��
*		height : ����
*		width : ��
*		depth : ���s��
* �߂�l�Fobject : �V�������������N���X�̃A�h���X
* �����F�I�u�W�F�N�g�̐���
*******************************************************************************/

CStencilShadow *CStencilShadow::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth) {
	CStencilShadow *object;
	object = new CStencilShadow;
	object -> Init(pos, rot, height, width, depth);
	return object;
}