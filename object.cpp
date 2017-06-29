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
#include "object.h"
#include "meshfield.h"
#include "player.h"


typedef struct{
	float depth;
	int num;
}DEPTHCHK;


/*******************************************************************************
* �֐����FCObject
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CObject::CObject(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* �֐����F~CObject
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CObject::~CObject( ) {

}


/*******************************************************************************
* �֐����FInit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F����������
*******************************************************************************/

void CObject::Init(void) {

}

void CObject::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth) {
	int i, j; // for���[�v�p
	VERTEX_3D *pVtx;
	WORD *pIdx;

	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer = CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾

	m_pTexture = NULL;
	m_playerOnFlag = false;
	m_alphaFlag = false;

	// �e�N�X�`���ݒ�
	D3DXCreateTextureFromFile(device, "data/TEXTURE/field001.jpg", &m_pTexture);

	// ���_�o�b�t�@�̐���
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * OBJECT_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * OBJECT_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxTopBuff, NULL);

	// �ǂ̏����ݒ�
	m_object[0].pos = D3DXVECTOR3(pos.x, pos.y + height * 0.5f, pos.z);
	m_object[0].rot.x = 0.5f * D3DX_PI;
	m_object[0].rot.y = 0.0f;
	m_object[0].rot.z = 0.0f;

	m_object[1].pos = D3DXVECTOR3(pos.x, pos.y, pos.z + depth * 0.5f);
	m_object[1].rot.x = 0.0f;
	m_object[1].rot.y = D3DX_PI;
	m_object[1].rot.z = 0.0f;

	m_object[2].pos = D3DXVECTOR3(pos.x - width * 0.5f, pos.y, pos.z);
	m_object[2].rot.x = 0.0f;
	m_object[2].rot.y = 0.5f * D3DX_PI;
	m_object[2].rot.z = 0.0f;

	m_object[3].pos = D3DXVECTOR3(pos.x + width * 0.5f, pos.y, pos.z);
	m_object[3].rot.x = 0.0f;
	m_object[3].rot.y = -0.5f * D3DX_PI;
	m_object[3].rot.z = 0.0f;

	m_object[4].pos = D3DXVECTOR3(pos.x, pos.y, pos.z - depth * 0.5f);
	m_object[4].rot.x = 0.0f;
	m_object[4].rot.y = 0.0f;
	m_object[4].rot.z = 0.0f;

	for(i = 0; i < 5; i++) {
		m_object[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		m_object[i].move = D3DXVECTOR3(0, 0, 0);
	}

	// ���_���ւ̃|�C���^���擾
	m_pVtxBuff -> Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0; i < OBJECT_VERTEX_NUM; i++) {
		// ���W�ݒ�
//		pVtx[i].pos = D3DXVECTOR3( (pos.x - width * 0.5f) + (width / (float)OBJECT_X_BLOCK) * (i % (OBJECT_X_BLOCK + 1) ),
//									(pos.y - height * 0.5f) - (height / (float)OBJECT_Y_BLOCK) * (i / (OBJECT_X_BLOCK + 1) ), 0);
		pVtx[i].pos = D3DXVECTOR3( (-width * 0.5f) + (width / (float)OBJECT_X_BLOCK) * (i % (OBJECT_X_BLOCK + 1) ),
									(height * 0.5f) - (height / (float)OBJECT_Y_BLOCK) * (i / (OBJECT_X_BLOCK + 1) ), 0);

		// ���_�F�ݒ�
		pVtx[i].col = D3DCOLOR_RGBA(130, 90, 20, 255);

		// �@���ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// �e�N�X�`��UV���W�ݒ�
		pVtx[i].tex = D3DXVECTOR2( (1 / (float)OBJECT_TEXTURE_X_BLOCK) * (i % (OBJECT_X_BLOCK + 1) ), (1 / (float)OBJECT_TEXTURE_Y_BLOCK) * (i / (OBJECT_X_BLOCK + 1) ) );
	}

	m_pVtxBuff -> Unlock( );

	m_pVtxTopBuff -> Lock(0, 0, (void**)&pVtx, 0);

	for(i = 0; i < OBJECT_VERTEX_NUM; i++) {
		pVtx[i].pos = D3DXVECTOR3( (-width * 0.5f) + (width / (float)OBJECT_X_BLOCK) * (i % (OBJECT_X_BLOCK + 1) ),
									(depth * 0.5f) - (depth / (float)OBJECT_Y_BLOCK) * (i / (OBJECT_X_BLOCK + 1) ), 0);

		// ���_�F�ݒ�
		pVtx[i].col = D3DCOLOR_RGBA(64, 255, 64, 255);

		// �@���ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);

		// �e�N�X�`��UV���W�ݒ�
		pVtx[i].tex = D3DXVECTOR2( (1 / (float)OBJECT_TEXTURE_X_BLOCK) * (i % (OBJECT_X_BLOCK + 1) ), (1 / (float)OBJECT_TEXTURE_Y_BLOCK) * (i / (OBJECT_X_BLOCK + 1) ) );
	}

	m_pVtxTopBuff -> Unlock( );

	// �C���f�b�N�X�o�b�t�@�m��
	device -> CreateIndexBuffer(sizeof(WORD) * OBJECT_INDEX_NUM,
								 D3DUSAGE_WRITEONLY,
								 D3DFMT_INDEX16,
								 D3DPOOL_MANAGED,
								 &m_pIdxBuff,
								 NULL);

	m_pIdxBuff -> Lock(0, 0, (void**) &pIdx, 0);

	for(i = 0; i < OBJECT_Y_BLOCK; i++) {
		if(i != 0) {
			pIdx[i * (OBJECT_X_BLOCK + 2) * 2 - 1] = (i + 1) * (OBJECT_X_BLOCK + 1);
		}

		for(j = 0; j < (OBJECT_X_BLOCK + 1) * 2; j++) {
			pIdx[i * (OBJECT_X_BLOCK + 2) * 2 + j] = (i + 1 - j % 2) * (OBJECT_X_BLOCK + 1) + (j / 2);
		}

		if(i < OBJECT_Y_BLOCK - 1) {
			pIdx[i * (OBJECT_X_BLOCK + 2) * 2 + j] = (i + 1 - (j - 1) % 2) * (OBJECT_X_BLOCK + 1) + ( (j - 1) / 2);
		}
	}

	m_pIdxBuff -> Unlock( );
}


/*******************************************************************************
* �֐����FUninit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�I������
*******************************************************************************/

void CObject::Uninit(void) {
	if(m_pTexture != NULL) {
		m_pTexture -> Release( ); // �e�N�X�`���̊J��
		m_pTexture = NULL;
	}

	if(m_pVtxBuff != NULL) {
		m_pVtxBuff -> Release( );
		m_pVtxBuff = NULL;
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

void CObject::Update(void) {

}


/*******************************************************************************
* �֐����FDraw
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�`�揈��
*******************************************************************************/

void CObject::Draw(void) {
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾
	CCamera *getCamera;
	getCamera = CManager::GetCamera( );
	CAMERA camera = getCamera -> GetCameraData( ); // �J�������擾
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // �v�Z�p
	D3DXVECTOR3 cameraFrontWay, cameraToWall;

	// ���ߐݒ肪ON�Ȃ瓧�߂��ꂽ���ʂ�`�悵�Ă���\�ʂ�`�悷��i
	if(m_alphaFlag) {
		// �A���t�@�u�����h
		device -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		// ���ʕ`��
		device -> SetRenderState(D3DRS_CULLMODE, D3DCULL_CW);

		for(int i = 0; i < 5; i++) {
			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&m_mtxWorld);
			// ���[���h�}�g���b�N�X�̍쐬
			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, m_object[i].scl.x, m_object[i].scl.y, m_object[i].scl.z);
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, m_object[i].rot.y, m_object[i].rot.x, m_object[i].rot.z); // ������ y, x, z �̏��Ԃɒ��ӁI�I�I ������
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTrans, m_object[i].pos.x, m_object[i].pos.y, m_object[i].pos.z);
			D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

			// �C���f�b�N�X�o�b�t�@���o�C���h
			device -> SetIndices(m_pIdxBuff);

			// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
			if(i == 0) {
				device -> SetStreamSource(0, m_pVtxTopBuff, 0, sizeof(VERTEX_3D) );
			}
			else {
				device -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D) );
			}

			// ���_�t�H�[�}�b�g�̐ݒ�
			device -> SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			device -> SetTexture(0, m_pTexture);

			// �|���S���̕`��
			device -> DrawIndexedPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				0,
				OBJECT_VERTEX_NUM,
				0,
				OBJECT_TRIANGLE_NUM);
		}

		// �\�ʕ`��ɖ߂�
		device -> SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

		m_alphaFlag = false;
	}

	// ���ʂ̕`��
	for(int i = 0; i < 5; i++) {

/*		// ���ߏ����m�F�p�̑��ʐF�ւ�
		// 1�ԁ@2�΁@3�@4��
		m_pVtxBuff -> Lock(0, 0, (void**)&pVtx, 0);

		for(int j = 0; j < OBJECT_VERTEX_NUM; j++) {
			switch(getDepth[i].num){
			case 1:
				pVtx[j].col = D3DCOLOR_RGBA(255, 20, 20, 255);
				break;
			case 2:
				pVtx[j].col = D3DCOLOR_RGBA(20, 255, 20, 255);
				break;
			case 3:
				pVtx[j].col = D3DCOLOR_RGBA(20, 20, 255, 255);
				break;
			case 4:
				pVtx[j].col = D3DCOLOR_RGBA(255, 255, 255, 255);
				break;
			default:
				break;
			}
		}

		m_pVtxBuff -> Unlock( );*/

		// ���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&m_mtxWorld);
		// ���[���h�}�g���b�N�X�̍쐬
		// �X�P�[���𔽉f
		D3DXMatrixScaling(&mtxScl, m_object[i].scl.x, m_object[i].scl.y, m_object[i].scl.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

		// ��]�𔽉f
		D3DXMatrixRotationYawPitchRoll(&mtxRot, m_object[i].rot.y, m_object[i].rot.x, m_object[i].rot.z); // ������ y, x, z �̏��Ԃɒ��ӁI�I�I ������
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

		// �ړ��𔽉f
		D3DXMatrixTranslation(&mtxTrans, m_object[i].pos.x, m_object[i].pos.y, m_object[i].pos.z);
		D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

		// ���[���h�}�g���b�N�X�̐ݒ�
		device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

		// �C���f�b�N�X�o�b�t�@���o�C���h
		device -> SetIndices(m_pIdxBuff);

		// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
		if(i == 0) {
			device -> SetStreamSource(0, m_pVtxTopBuff, 0, sizeof(VERTEX_3D) );
		}
		else {
			device -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D) );
		}

		// ���_�t�H�[�}�b�g�̐ݒ�
		device -> SetFVF(FVF_VERTEX_3D);

		// �e�N�X�`���̐ݒ�
		device -> SetTexture(0, m_pTexture);

		device -> DrawIndexedPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			0,
			OBJECT_VERTEX_NUM,
			0,
			OBJECT_TRIANGLE_NUM);
	}

	// �u�����h�ݒ��߂��i���߂��ĂȂ����͖��ʂȏ����j
	device -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
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

CObject *CObject::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth) {
	CObject *object;
	object = new CObject;
	object -> Init(pos, rot, height, width, depth);
	return object;
}


/*******************************************************************************
* �֐����FHitCheck
* �����Fpos : ���W
*		rot : ��]��
*		height : ����
*		width : ��
*		depth : ���s��
* �߂�l�Fobject : �V�������������N���X�̃A�h���X
* �����F�I�u�W�F�N�g�̐���
*******************************************************************************/

OBJHITCHK CObject::HitCheck(D3DXVECTOR3 pos, D3DXVECTOR3 oldPos) {
	OBJHITCHK hitchk;

	hitchk.type = 0;
	hitchk.nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

	if(m_object[1].pos.z > pos.z && m_object[2].pos.x < pos.x && m_object[3].pos.x > pos.x && m_object[4].pos.z < pos.z && m_object[0].pos.y > pos.y) {
		if(m_object[0].pos.y <= oldPos.y) {
			hitchk.type = 2;
			hitchk.nor.y = m_object[0].pos.y; // ��ʂƏՓ�
		}
		else if(m_object[1].pos.z <= oldPos.z) {
			hitchk.type = 1;
			hitchk.nor = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
		}
		else if(m_object[2].pos.x >= oldPos.x) {
			hitchk.type = 1;
			hitchk.nor = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
		}
		else if(m_object[3].pos.x <= oldPos.x) {
			hitchk.type = 1;
			hitchk.nor = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		}
		else if(m_object[4].pos.z >= oldPos.z) {
			hitchk.type = 1;
			hitchk.nor = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
		}

		return hitchk;
	}

	return hitchk;
}


/*******************************************************************************
* �֐����FSetAlphaFlag
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�I�u�W�F�N�g���߂�ONOFF�؂�ւ�
*******************************************************************************/

void CObject::SetAlphaFlag(bool onoff) {
	m_alphaFlag = onoff;
}