/*******************************************************************************
* �^�C�g�� �t�B�[���h�`�揈��
* �t�@�C���� meshwater.cpp
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/5/18
*******************************************************************************/

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "meshwater.h"
#include "manager.h"
#include "sceneX.h"
#include "player.h"
#include "debugProc.h"


LPDIRECT3DVERTEXBUFFER9 CMeshwater::m_pVtxBuffMeshwater = NULL;


/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/

float g_HeightWater[MESHWATER_X_BLOCK + 1][MESHWATER_Z_BLOCK + 1];


/*******************************************************************************
* �֐����FCMeshwater
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CMeshwater::CMeshwater(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* �֐����F~CMeshwater
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CMeshwater::~CMeshwater( ) {

}


/*******************************************************************************
* �֐����FInitMeshwater
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�t�B�[���h�ݒ�̏�����
*******************************************************************************/

void CMeshwater::Init(void) {
	// �ϐ��錾
	int i, j; // for���[�v�p
	VERTEX_3D *pVtx;
	WORD *pIdx;
	D3DXVECTOR3 n0, v1, v2; // �@���v�Z�p

	// �ϐ��̎擾
	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer = CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾

	// �����o�ϐ��̏�����
	m_pTextureMeshwater = NULL;
	m_pVtxBuffMeshwater = NULL;
	m_pIdxBuffMeshwater = NULL;
	m_time = 0;

	// �����ݒ�
	srand( (unsigned) time(NULL) );

	for(int cntz = 0; cntz < MESHWATER_Z_BLOCK + 1; cntz++) {
		for(int cntx = 0; cntx < MESHWATER_X_BLOCK + 1; cntx++) {
			g_HeightWater[cntx][cntz] = MESHWATER_MIN_HEIGHT + ( (rand( ) % (10 * (MESHWATER_MAX_HEIGHT - MESHWATER_MIN_HEIGHT) ) ) / 10.0f);
		}
	}

	// �e�N�X�`���ݒ�
	D3DXCreateTextureFromFile(device, MESHWATER_TEXTURE_NAME, &m_pTextureMeshwater);

	// ���_�o�b�t�@�̐���
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * MESHWATER_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffMeshwater, NULL);

	// ���_���ւ̃|�C���^���擾
	m_pVtxBuffMeshwater -> Lock(0, 0, (void**)&pVtx, 0);

	// �p�x�����ݒ�
	m_posMeshwater = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotMeshwater = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_sclMeshwater = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	for(i = 0; i < MESHWATER_VERTEX_NUM; i++) {
		// ���W�ݒ�
		pVtx[i].pos = D3DXVECTOR3( (float) (MESHWATER_WATER_X + (MESHWATER_WATER_WIDTH / (float)MESHWATER_X_BLOCK) * (i % (MESHWATER_X_BLOCK + 1) ) ), g_HeightWater[i / (MESHWATER_X_BLOCK + 1)][i % (MESHWATER_Z_BLOCK + 1)], (float) (MESHWATER_WATER_Z - (MESHWATER_WATER_HEIGHT / (float)MESHWATER_Z_BLOCK) * (i / (MESHWATER_X_BLOCK + 1) ) ) );

		// ���_�F�ݒ�
		pVtx[i].col = D3DCOLOR_RGBA(32, 32, 192, 255);

		// �@���ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �e�N�X�`��UV���W�ݒ�
		pVtx[i].tex = D3DXVECTOR2( (1 / (float)MESHWATER_TEXTURE_X_BLOCK) * (i % (MESHWATER_X_BLOCK + 1) ), (1 / (float)MESHWATER_TEXTURE_Z_BLOCK) * (i / (MESHWATER_X_BLOCK + 1) ) );
	}

	for(i = 0; i < MESHWATER_Z_BLOCK + 1; i++) {
		for(j = 0; j < MESHWATER_X_BLOCK + 1; j++) {
			// ������
			n0 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// �ŏ��̍s�̏���
			if(i == 0) {
				// �ŏ��̗�̏���
				if(j == 0) {
					// ���_�ɐڂ���e�|���S���̖@�������߂�
					// 1�ڂ̃|���S��
					v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					// �O�ς����߂�
					D3DXVec3Cross(&v1, &v1, &v2);
					// ���߂��@���𐳋K�����đ������킹��
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					// 2�ڂ̃|���S�� v2�͒l���c���Ă���̂Ŏg����
					v1 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					// �O�ς����߂�@�ϐ����g���܂킵�Ă�̂ŊO�ς����߂�ۂ̃x�N�g���̏��Ԃɒ���
					D3DXVec3Cross(&v2, &v2, &v1);
					// ���߂��@���𐳋K�����đ������킹��
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					// �������킹���@���𐳋K��
					D3DXVec3Normalize(&n0, &n0);
					// ����ꂽ���_�̖@������
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
				// �Ō�̗�̏���
				else if(j == MESHWATER_X_BLOCK) {
					v1 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
				// ����ȊO�̗�̏���
				else{
					v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
			}
			// �Ō�̍s�̏���
			else if(i == MESHWATER_Z_BLOCK) {
				// �ŏ��̗�̏���
				if(j == 0) {
					v1 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
				// �Ō�̗�̏���
				else if(j == MESHWATER_X_BLOCK) {
					v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
				// ����ȊO�̗�̏���
				else{
					v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
			}
			// ����ȊO�̍s�̏���
			else{
				// �ŏ��̗�̏���
				if(j == 0) {
					v1 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
				// �Ō�̗�̏���
				else if(j == MESHWATER_X_BLOCK) {
					v1 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
				// ����ȊO�̗�̏���
				else{
					v1 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2,&v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHWATER_X_BLOCK + 1) + j].nor = n0;
				}
			}
		}
	}

	m_pVtxBuffMeshwater -> Unlock( );

	// �C���f�b�N�X�o�b�t�@�m��
	device -> CreateIndexBuffer(sizeof(WORD) * MESHWATER_INDEX_NUM,
								 D3DUSAGE_WRITEONLY,
								 D3DFMT_INDEX16,
								 D3DPOOL_MANAGED,
								 &m_pIdxBuffMeshwater,
								 NULL);

	m_pIdxBuffMeshwater -> Lock(0, 0, (void**) &pIdx, 0);

	for(i = 0; i < MESHWATER_Z_BLOCK; i++) {
		if(i != 0) {
			pIdx[i * (MESHWATER_X_BLOCK + 2) * 2 - 1] = (i + 1) * (MESHWATER_X_BLOCK + 1);
		}

		for(j = 0; j < (MESHWATER_X_BLOCK + 1) * 2; j++) {
			pIdx[i * (MESHWATER_X_BLOCK + 2) * 2 + j] = (i + 1 - j % 2) * (MESHWATER_X_BLOCK + 1) + (j / 2);
		}

		j--;
		if(i < MESHWATER_Z_BLOCK - 1) {
			pIdx[i * (MESHWATER_X_BLOCK + 2) * 2 + j + 1] = (i + 1 - j % 2) * (MESHWATER_X_BLOCK + 1) + (j / 2);
		}
	}

	m_pIdxBuffMeshwater -> Unlock( );
}


/*******************************************************************************
* �֐����FUninitMeshwater
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�t�B�[���h�̏I������
*******************************************************************************/

void CMeshwater::Uninit(void) {
	if(m_pTextureMeshwater != NULL) {
		m_pTextureMeshwater -> Release( ); // �e�N�X�`���̊J��
		m_pTextureMeshwater = NULL;
	}

	if(m_pVtxBuffMeshwater != NULL) {
		m_pVtxBuffMeshwater -> Release( );
		m_pVtxBuffMeshwater = NULL;
	}

	if(m_pIdxBuffMeshwater != NULL) {
		m_pIdxBuffMeshwater -> Release( );
		m_pIdxBuffMeshwater = NULL;
	}
}


/*******************************************************************************
* �֐����FUpdataMeshwater
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�t�B�[���h�̏��X�V
*******************************************************************************/

void CMeshwater::Update(void) {
	D3DXVECTOR2 model_pos = D3DXVECTOR2(0.0f, 0.0f);

	VERTEX_3D *pVtx;

	if(m_time % MESHWATER_MOVE_TIME == 0) {
		// ���_���ւ̃|�C���^���擾
		m_pVtxBuffMeshwater -> Lock(0, 0, (void**)&pVtx, 0);

		for(int cntz = 0; cntz < MESHWATER_Z_BLOCK + 1; cntz++) {	// 330.15 330   3301.5 3300
			for(int cntx = 0; cntx < MESHWATER_X_BLOCK + 1; cntx++) {
				if(pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y * 10.0f - (int)(pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y * 10.0f) <= 0.1f) {
					pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y += 0.1f;
				}
				else{
					pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y -= 0.1f;
				}

				if(pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y > MESHWATER_MAX_HEIGHT) {
					pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y = MESHWATER_MAX_HEIGHT - 0.05f;
				}
				else if(pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y < MESHWATER_MIN_HEIGHT) {
					pVtx[cntz * (MESHWATER_X_BLOCK + 1) + cntx].pos.y = MESHWATER_MIN_HEIGHT;
				}
			}
		}

		m_pVtxBuffMeshwater -> Unlock( );
	}

	m_time++;
}


/*******************************************************************************
* �֐����FDrawMeshwater
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�t�B�[���h�̕`�揈��
*******************************************************************************/

void CMeshwater::Draw(void) {
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // �v�Z�p

	// �A���t�@�u�����h�̕��@�����Z������
	device -> SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device -> SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorldMeshwater);
	// ���[���h�}�g���b�N�X�̍쐬
	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&m_mtxWorldMeshwater, &m_mtxWorldMeshwater, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f); // ������ y, x, z �̏��Ԃɒ��ӁI�I�I ������
	D3DXMatrixMultiply(&m_mtxWorldMeshwater, &m_mtxWorldMeshwater, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorldMeshwater, &m_mtxWorldMeshwater, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorldMeshwater);

	// �C���f�b�N�X�o�b�t�@���o�C���h
	device -> SetIndices(m_pIdxBuffMeshwater);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_pVtxBuffMeshwater, 0, sizeof(VERTEX_3D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	device -> SetTexture(0, m_pTextureMeshwater);

	// �|���S���̕`��
/*	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		MESHWATER_TRIANGLE_NUM);*/

	device -> DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		MESHWATER_VERTEX_NUM,
		0,
		MESHWATER_TRIANGLE_NUM);

	// �A���t�@�u�����h�ݒ��߂�
	device -> SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	device -> SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}


/*******************************************************************************
* �֐����FCreate
* �����FCMeshwater : �A�h���X
* �߂�l�F�Ȃ�
* �����F�I�u�W�F�N�g�̐���
*******************************************************************************/

CMeshwater *CMeshwater::Create(void) {
	CMeshwater *meshwater;
	meshwater = new CMeshwater;
	meshwater -> Init( );
	return meshwater;
}


/*******************************************************************************
* �֐����FGetWaterHeight
* �����Fpos : ���݂̈ʒu���
* �߂�l�Fcoord �F ���W
* �����F���ݒn���炻�̍��W�̍������v�Z�ŋ��߂ĕԂ��֐�
*******************************************************************************/

float CMeshwater::GetWaterHeight(D3DXVECTOR3 pos) {
	// �ϐ��̐錾
	D3DXVECTOR2 coord = D3DXVECTOR2(0.0f, 0.0f);

	int i, j;
	float height;
	VERTEX_3D *pVtx;
	D3DXVECTOR3 v1, v2;
	float n0, n1, n2;
	bool flag;

	float about_x, about_z;
	int start_x, end_x, start_z, end_z;

	about_x = (MESHWATER_WATER_WIDTH * 0.5f + pos.x) / MESHWATER_WATER_WIDTH * MESHWATER_X_BLOCK;
	about_z = (MESHWATER_WATER_HEIGHT * 0.5f - pos.z) / MESHWATER_WATER_HEIGHT * MESHWATER_Z_BLOCK;

	start_x = (int)about_x - 1;
	end_x = (int)about_x + 1;

	start_z = (int)about_z - 1;
	end_z = (int) about_z + 1;

	if(start_x < 0) {
		start_x = 0;
	}
	if(end_x > MESHWATER_X_BLOCK) {
		end_x = MESHWATER_X_BLOCK;
	}

	if(start_z < 0) {
		start_z = 0;
	}
	if(end_z > MESHWATER_Z_BLOCK) {
		end_z = MESHWATER_Z_BLOCK;
	}

	// �ϐ��̏�����
	n0 = n1 = n2 = height = 0;
	v1 = v2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	flag = false;

	// ���_���ւ̃|�C���^���擾
	m_pVtxBuffMeshwater -> Lock(0, 0, (void**)&pVtx, 0);

	// ���W�擾
	for(i = start_z; i < end_z; i++) {
		for(j = start_x; j < end_x; j++) {
			// �Ώۂ��E��|���S����ɂ��邩�ۂ��̔���
			v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			// ���݂̃|���S����ɑΏۂ������ꍇ
			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				// �@�����v�Z���Đ��K��
				v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
				v2 = pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
				D3DXVec3Cross(&v1, &v1, &v2);
				D3DXVec3Normalize(&v1, &v1);

				// �����v�Z
				height = pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos.y
						- (v1.x * (pos.x - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos.x)
						   + v1.z * (pos.z - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos.z) 
						   ) / v1.y;

				// ���b�N���������ă��^�[��
				m_pVtxBuffMeshwater -> Unlock( );
				return height;
			}

			// �Ώۂ������|���S����ɂ��邩�ۂ��̔���
			v1 = pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				v1 = pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
				v2 = pVtx[ (i + 1) * (MESHWATER_X_BLOCK + 1) + j].pos - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos;
				D3DXVec3Cross(&v1, &v1, &v2);
				D3DXVec3Normalize(&v1, &v1);

				height = pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos.y
						- (v1.x * (pos.x - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos.x)
						   + v1.z * (pos.z - pVtx[i * (MESHWATER_X_BLOCK + 1) + j].pos.z) 
						   ) / v1.y;

				m_pVtxBuffMeshwater -> Unlock( );

				return height;
			}
		}
	}
	m_pVtxBuffMeshwater -> Unlock( );

	return height;
}