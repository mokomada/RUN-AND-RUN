/*******************************************************************************
* �^�C�g�� �t�B�[���h�`�揈��
* �t�@�C���� meshfield.cpp
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/5/18
*******************************************************************************/

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "meshfield.h"
#include "manager.h"
#include "sceneX.h"
#include "player.h"
#include "debugProc.h"


LPDIRECT3DVERTEXBUFFER9 CMeshfield::m_pVtxBuffMeshfield = NULL;


/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/



/*******************************************************************************
* �֐����FCMeshfield
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CMeshfield::CMeshfield(int priority, OBJTYPE objType):CScene(priority, objType) {

}


/*******************************************************************************
* �֐����F~CMeshfield
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CMeshfield::~CMeshfield( ) {

}


/*******************************************************************************
* �֐����FInitMeshfield
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�t�B�[���h�ݒ�̏�����
*******************************************************************************/

void CMeshfield::Init(void) {
	// �ϐ��錾
	int i, j, cntx, cntz; // for���[�v�p
	VERTEX_3D *pVtx;
	WORD *pIdx;
	D3DXVECTOR3 n0, v1, v2; // �@���v�Z�p

	// �ϐ��̎擾
	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer = CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾

	float heightMap[MESHFIELD_X_BLOCK + 1][MESHFIELD_Z_BLOCK + 1] = {
//		  1    2    3    4    5    6    7    8    9    10   11   12   13   14   15   16   17   18   19   20   21   22   23   24   25   26   27   28   29   30   31   32   33   34   35   36   37   38   39   40   41
		{300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300}, // 1
		{300, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 300}, // 2
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 3
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 4
		{300, 350, 360, 360, 350, 350, 350, 350, 360, 350, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 5
		{300, 350, 360, 360, 350, 300, 300, 300, 350, 340, 340, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 6
		{300, 350, 360, 360, 350, 300, 300, 300, 350, 340, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 7
		{300, 350, 360, 360, 350, 300, 300, 300, 340, 330, 340, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 8
		{300, 350, 360, 360, 360, 350, 350, 350, 350, 340, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 9
		{300, 350, 360, 370, 370, 360, 360, 360, 360, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 10
		{300, 360, 370, 380, 380, 370, 360, 370, 370, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 11
		{300, 350, 360, 370, 370, 360, 360, 360, 370, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 12
		{300, 350, 360, 360, 360, 350, 350, 360, 360, 360, 360, 360, 370, 368, 371, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 13
		{300, 350, 360, 350, 350, 340, 340, 350, 360, 360, 360, 360, 372, 380, 382, 373, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 14
		{300, 350, 360, 350, 340, 330, 340, 350, 360, 360, 360, 360, 367, 377, 380, 370, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 15
		{300, 350, 360, 350, 340, 330, 340, 350, 360, 360, 360, 360, 360, 373, 369, 371, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 16
		{300, 350, 360, 360, 350, 340, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 17
		{300, 350, 360, 360, 360, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 18
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 19
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 20
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 360, 360, 360, 350, 300}, // 21
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 360, 350, 300}, // 22
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 330, 330, 330, 330, 330, 330, 330, 330, 360, 360, 340, 360, 360, 350, 300}, // 23
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 370, 370, 370, 370, 370, 370, 330, 330, 360, 340, 360, 360, 350, 300}, // 24
		{300, 350, 352, 345, 345, 343, 345, 342, 346, 345, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 320, 320, 320, 320, 370, 370, 330, 360, 340, 360, 360, 350, 300}, // 25
		{300, 350, 350, 345, 330, 330, 328, 331, 327, 330, 345, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 380, 380, 380, 320, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 26
		{300, 347, 354, 345, 330, 322, 320, 318, 320, 320, 330, 343, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 380, 420, 380, 380, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 27
		{300, 343, 350, 342, 332, 320, 321, 320, 315, 320, 330, 345, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 380, 420, 420, 380, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 28
		{300, 346, 343, 345, 334, 321, 323, 325, 320, 321, 332, 340, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 380, 420, 420, 380, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 29
		{300, 345, 348, 348, 331, 320, 320, 320, 318, 320, 330, 345, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 380, 420, 420, 380, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 30
		{300, 348, 350, 345, 328, 317, 315, 316, 320, 322, 333, 343, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 380, 420, 420, 380, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 31
		{300, 350, 360, 342, 330, 320, 318, 320, 320, 320, 330, 345, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 320, 320, 380, 380, 380, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 32
		{300, 350, 360, 360, 345, 330, 328, 330, 330, 330, 330, 348, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 370, 370, 320, 320, 320, 320, 320, 370, 330, 360, 340, 360, 360, 350, 300}, // 33
		{300, 350, 360, 360, 360, 347, 345, 348, 345, 341, 344, 345, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 330, 330, 370, 370, 370, 370, 370, 370, 370, 330, 360, 340, 360, 360, 350, 300}, // 34
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 360, 330, 330, 330, 330, 330, 330, 330, 330, 330, 360, 340, 360, 360, 350, 300}, // 35
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 360, 360, 350, 300}, // 36
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 340, 360, 360, 350, 300}, // 37
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 38
		{300, 350, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 360, 350, 300}, // 39
		{300, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 350, 300}, // 40
		{300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300, 300}, // 41
	}; // ���������l�ݒ�p

	// �����o�ϐ��̏�����
	m_pTextureMeshfield = NULL;
	m_pVtxBuffMeshfield = NULL;
	m_pIdxBuffMeshfield = NULL;



	// �����ݒ�
	srand( (unsigned) time(NULL) );

	// ���������l�ݒ�

	for(cntz = 0; cntz < MESHFIELD_Z_BLOCK + 1; cntz++) {
		for(cntx = 0; cntx < MESHFIELD_X_BLOCK + 1; cntx++) {
			m_HeightMap[cntx][cntz] = 300.0f + rand( ) % 80;
			if(cntx != 0) {
				if(m_HeightMap[cntx][cntz] > m_HeightMap[cntx - 1][cntz] + MESHFIELD_DIFFERENCE_MAX) {
					m_HeightMap[cntx][cntz] = m_HeightMap[cntx - 1][cntz] + MESHFIELD_DIFFERENCE_MAX;
				}
				else if(m_HeightMap[cntx][cntz] < m_HeightMap[cntx - 1][cntz] - MESHFIELD_DIFFERENCE_MAX) {
					m_HeightMap[cntx][cntz] = m_HeightMap[cntx - 1][cntz] - MESHFIELD_DIFFERENCE_MAX;
				}
			}
			if(cntz != 0) {
				if(m_HeightMap[cntx][cntz] > m_HeightMap[cntx][cntz - 1] + MESHFIELD_DIFFERENCE_MAX) {
					m_HeightMap[cntx][cntz] = m_HeightMap[cntx][cntz - 1] + MESHFIELD_DIFFERENCE_MAX;
				}
				else if(m_HeightMap[cntx][cntz] < m_HeightMap[cntx][cntz - 1] - MESHFIELD_DIFFERENCE_MAX) {
					m_HeightMap[cntx][cntz] = m_HeightMap[cntx][cntz - 1] - MESHFIELD_DIFFERENCE_MAX;
				}
			}
			m_HeightMap[cntx][cntz] = heightMap[cntx][cntz];
		}
	}

	// �e�N�X�`���ݒ�
	D3DXCreateTextureFromFile(device, MESHFIELD_TEXTURE_NAME, &m_pTextureMeshfield);

	// ���_�o�b�t�@�̐���
	device -> CreateVertexBuffer(sizeof(VERTEX_3D) * MESHFIELD_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffMeshfield, NULL);

	// ���_���ւ̃|�C���^���擾
	m_pVtxBuffMeshfield -> Lock(0, 0, (void**)&pVtx, 0);

	// �p�x�����ݒ�
	m_posMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rotMeshfield = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_sclMeshfield = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	for(i = 0; i < MESHFIELD_VERTEX_NUM; i++) {
		// ���W�ݒ�
		pVtx[i].pos = D3DXVECTOR3( 
			(float) (MESHFIELD_FIELD_X + (MESHFIELD_FIELD_WIDTH / (float)MESHFIELD_X_BLOCK) * (i % (MESHFIELD_X_BLOCK + 1) ) ), 
			m_HeightMap[i / (MESHFIELD_X_BLOCK + 1)][i % (MESHFIELD_Z_BLOCK + 1)], 
			(float) (MESHFIELD_FIELD_Z - (MESHFIELD_FIELD_HEIGHT / (float)MESHFIELD_Z_BLOCK) * (i / (MESHFIELD_X_BLOCK + 1) ) ) );

		// ���_�F�ݒ�
		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		// �@���ݒ�
		pVtx[i].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �e�N�X�`��UV���W�ݒ�
		pVtx[i].tex = D3DXVECTOR2( (1 / (float)MESHFIELD_TEXTURE_X_BLOCK) * (i % (MESHFIELD_X_BLOCK + 1) ), (1 / (float)MESHFIELD_TEXTURE_Z_BLOCK) * (i / (MESHFIELD_X_BLOCK + 1) ) );
	}

	for(i = 0; i < MESHFIELD_Z_BLOCK + 1; i++) {
		for(j = 0; j < MESHFIELD_X_BLOCK + 1; j++) {
			// ������
			n0 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

			// �ŏ��̍s�̏���
			if(i == 0) {
				// �ŏ��̗�̏���
				if(j == 0) {
					// ���_�ɐڂ���e�|���S���̖@�������߂�
					// 1�ڂ̃|���S��
					v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					// �O�ς����߂�
					D3DXVec3Cross(&v1, &v1, &v2);
					// ���߂��@���𐳋K�����đ������킹��
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					// 2�ڂ̃|���S�� v2�͒l���c���Ă���̂Ŏg����
					v1 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					// �O�ς����߂�@�ϐ����g���܂킵�Ă�̂ŊO�ς����߂�ۂ̃x�N�g���̏��Ԃɒ���
					D3DXVec3Cross(&v2, &v2, &v1);
					// ���߂��@���𐳋K�����đ������킹��
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					// �������킹���@���𐳋K��
					D3DXVec3Normalize(&n0, &n0);
					// ����ꂽ���_�̖@������
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
				// �Ō�̗�̏���
				else if(j == MESHFIELD_X_BLOCK) {
					v1 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
				// ����ȊO�̗�̏���
				else{
					v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
			}
			// �Ō�̍s�̏���
			else if(i == MESHFIELD_Z_BLOCK) {
				// �ŏ��̗�̏���
				if(j == 0) {
					v1 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
				// �Ō�̗�̏���
				else if(j == MESHFIELD_X_BLOCK) {
					v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
				// ����ȊO�̗�̏���
				else{
					v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
			}
			// ����ȊO�̍s�̏���
			else{
				// �ŏ��̗�̏���
				if(j == 0) {
					v1 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
				// �Ō�̗�̏���
				else if(j == MESHFIELD_X_BLOCK) {
					v1 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
				// ����ȊO�̗�̏���
				else{
					v1 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					v2 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2, &v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					v2 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j - 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v1, &v1, &v2);
					D3DXVec3Normalize(&v1, &v1);
					n0 += v1;

					v1 = pVtx[(i - 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
					D3DXVec3Cross(&v2, &v2,&v1);
					D3DXVec3Normalize(&v2, &v2);
					n0 += v2;

					D3DXVec3Normalize(&n0, &n0);
					pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].nor = n0;
				}
			}
		}
	}

	// �����ɂ���ĐF�ς�
	for(i = 0; i < MESHFIELD_Z_BLOCK + 1; i++) {
		for(j = 0; j < MESHFIELD_X_BLOCK + 1; j++) {
			if(pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.y > 340) {
				pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].col = D3DCOLOR_RGBA(64, 255, 64, 255);
			}
			else if(pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.y > 290) {
				pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].col = D3DCOLOR_RGBA(130, 90, 20, 255);
			}
			else{
				pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].col = D3DCOLOR_RGBA(64, 64, 255, 255);
			}
		}
	}

	m_pVtxBuffMeshfield -> Unlock( );

	// �C���f�b�N�X�o�b�t�@�m��
	device -> CreateIndexBuffer(sizeof(WORD) * MESHFIELD_INDEX_NUM,
								 D3DUSAGE_WRITEONLY,
								 D3DFMT_INDEX16,
								 D3DPOOL_MANAGED,
								 &m_pIdxBuffMeshfield,
								 NULL);

	m_pIdxBuffMeshfield -> Lock(0, 0, (void**) &pIdx, 0);

	for(i = 0; i < MESHFIELD_Z_BLOCK; i++) {
		if(i != 0) {
			pIdx[i * (MESHFIELD_X_BLOCK + 2) * 2 - 1] = (i + 1) * (MESHFIELD_X_BLOCK + 1);
		}

		for(j = 0; j < (MESHFIELD_X_BLOCK + 1) * 2; j++) {
			pIdx[i * (MESHFIELD_X_BLOCK + 2) * 2 + j] = (i + 1 - j % 2) * (MESHFIELD_X_BLOCK + 1) + (j / 2);
		}

		j--;
		if(i < MESHFIELD_Z_BLOCK - 1) {
			pIdx[i * (MESHFIELD_X_BLOCK + 2) * 2 + j + 1] = (i + 1 - j % 2) * (MESHFIELD_X_BLOCK + 1) + (j / 2);
		}
	}

	m_pIdxBuffMeshfield -> Unlock( );
}


/*******************************************************************************
* �֐����FUninitMeshfield
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�t�B�[���h�̏I������
*******************************************************************************/

void CMeshfield::Uninit(void) {
	if(m_pTextureMeshfield != NULL) {
		m_pTextureMeshfield -> Release( ); // �e�N�X�`���̊J��
		m_pTextureMeshfield = NULL;
	}

	if(m_pVtxBuffMeshfield != NULL) {
		m_pVtxBuffMeshfield -> Release( );
		m_pVtxBuffMeshfield = NULL;
	}

	if(m_pIdxBuffMeshfield != NULL) {
		m_pIdxBuffMeshfield -> Release( );
		m_pIdxBuffMeshfield = NULL;
	}
}


/*******************************************************************************
* �֐����FUpdataMeshfield
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�t�B�[���h�̏��X�V
*******************************************************************************/

void CMeshfield::Update(void) {
/*	MODEL model;
	model = CPlayer::GetPlayer( );

	D3DXVECTOR2 model_pos = D3DXVECTOR2(0.0f, 0.0f);

	int i, j;
	VERTEX_3D *pVtx;
	D3DXVECTOR3 v1, v2;
	float n0, n1, n2;
	float about_x, about_z;
	int start_x, end_x, start_z, end_z;

	// �v���C���[�̍��W������o���A���̑O��̐������擾���ăt�B�[���h�Ƃ̓����蔻��ɗp����
	about_x = (MESHFIELD_FIELD_WIDTH * 0.5f + model.pos.x) / MESHFIELD_FIELD_WIDTH * MESHFIELD_X_BLOCK;
	about_z = (MESHFIELD_FIELD_HEIGHT * 0.5f - model.pos.z) / MESHFIELD_FIELD_HEIGHT * MESHFIELD_Z_BLOCK;

	start_x = (int)about_x - 1;
	end_x = (int)about_x + 1;

	start_z = (int)about_z - 1;
	end_z = (int) about_z + 1;

	if(start_x < 0) {
		start_x = 0;
	}
	if(end_x > MESHFIELD_X_BLOCK) {
		end_x = MESHFIELD_X_BLOCK;
	}

	if(start_z < 0) {
		start_z = 0;
	}
	if(end_z > MESHFIELD_Z_BLOCK) {
		end_z = MESHFIELD_Z_BLOCK;
	}
	
	n0 = n1 = n2 = 0;
	v1 = v2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	// ���_���ւ̃|�C���^���擾
	m_pVtxBuffMeshfield -> Lock(0, 0, (void**)&pVtx, 0);

	for(i = start_z; i < end_z; i++) {
		for(j = start_x; j < end_x; j++) {
			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = model.pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = model.pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = model.pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				model_pos = D3DXVECTOR2( (float)j, (float)i);
				break;
			}

			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = model.pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = model.pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = model.pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				model_pos = D3DXVECTOR2( (float)j, (float)i);
				break;
			}
		}
		if(model_pos.x != 0 && model_pos.y !=0) {
			break;
		}
	}

	m_pVtxBuffMeshfield -> Unlock( );

	PrintDebugProc("���ݒn���W(%f, %f)\n", model_pos.x, model_pos.y);*/
}


/*******************************************************************************
* �֐����FDrawMeshfield
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�t�B�[���h�̕`�揈��
*******************************************************************************/

void CMeshfield::Draw(void) {
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // �v�Z�p

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorldMeshfield);
	// ���[���h�}�g���b�N�X�̍쐬
	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&m_mtxWorldMeshfield, &m_mtxWorldMeshfield, &mtxScl);

	// ��]�𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, 0.0f, 0.0f, 0.0f); // ������ y, x, z �̏��Ԃɒ��ӁI�I�I ������
	D3DXMatrixMultiply(&m_mtxWorldMeshfield, &m_mtxWorldMeshfield, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, 0.0f, 0.0f, 0.0f);
	D3DXMatrixMultiply(&m_mtxWorldMeshfield, &m_mtxWorldMeshfield, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorldMeshfield);

	// �C���f�b�N�X�o�b�t�@���o�C���h
	device -> SetIndices(m_pIdxBuffMeshfield);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_pVtxBuffMeshfield, 0, sizeof(VERTEX_3D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_3D);

	// �e�N�X�`���̐ݒ�
	device -> SetTexture(0, m_pTextureMeshfield);

	// �|���S���̕`��
/*	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		MESHFIELD_TRIANGLE_NUM);*/

	device -> DrawIndexedPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		0,
		MESHFIELD_VERTEX_NUM,
		0,
		MESHFIELD_TRIANGLE_NUM);
}


/*******************************************************************************
* �֐����FCreate
* �����FCMeshfield : �A�h���X
* �߂�l�F�Ȃ�
* �����F�I�u�W�F�N�g�̐���
*******************************************************************************/

CMeshfield *CMeshfield::Create(void) {
	CMeshfield *meshfield;
	meshfield = new CMeshfield;
	meshfield -> Init( );
	return meshfield;
}


/*******************************************************************************
* �֐����FGetFieldHeight
* �����Fpos : ���݂̈ʒu���
* �߂�l�Fheight �F ����
* �����F���ݒn���炻�̍��W�̍������v�Z�ŋ��߂ĕԂ��֐�
*******************************************************************************/

float CMeshfield::GetFieldHeight(D3DXVECTOR3 pos) {
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

	about_x = (MESHFIELD_FIELD_WIDTH * 0.5f + pos.x) / MESHFIELD_FIELD_WIDTH * MESHFIELD_X_BLOCK;
	about_z = (MESHFIELD_FIELD_HEIGHT * 0.5f - pos.z) / MESHFIELD_FIELD_HEIGHT * MESHFIELD_Z_BLOCK;

	start_x = (int)about_x - 1;
	end_x = (int)about_x + 1;

	start_z = (int)about_z - 1;
	end_z = (int) about_z + 1;

	if(start_x < 0) {
		start_x = 0;
	}
	if(end_x > MESHFIELD_X_BLOCK) {
		end_x = MESHFIELD_X_BLOCK;
	}

	if(start_z < 0) {
		start_z = 0;
	}
	if(end_z > MESHFIELD_Z_BLOCK) {
		end_z = MESHFIELD_Z_BLOCK;
	}

	// �ϐ��̏�����
	n0 = n1 = n2 = height = 0;
	v1 = v2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	flag = false;

	// ���_���ւ̃|�C���^���擾
	m_pVtxBuffMeshfield -> Lock(0, 0, (void**)&pVtx, 0);

	// ���W�擾
	for(i = start_z; i < end_z; i++) {
		for(j = start_x; j < end_x; j++) {
			// �Ώۂ��E��|���S����ɂ��邩�ۂ��̔���
			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			// ���݂̃|���S����ɑΏۂ������ꍇ
			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				// �@�����v�Z���Đ��K��
				v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				v2 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				D3DXVec3Cross(&v1, &v1, &v2);
				D3DXVec3Normalize(&v1, &v1);

				// �����v�Z
				height = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.y
						- (v1.x * (pos.x - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.x)
						   + v1.z * (pos.z - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.z) 
						   ) / v1.y;

				// ���b�N���������ă��^�[��
				m_pVtxBuffMeshfield -> Unlock( );
				return height;
			}

			// �Ώۂ������|���S����ɂ��邩�ۂ��̔���
			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				v2 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				D3DXVec3Cross(&v1, &v1, &v2);
				D3DXVec3Normalize(&v1, &v1);

				height = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.y
						- (v1.x * (pos.x - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.x)
						   + v1.z * (pos.z - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos.z) 
						   ) / v1.y;

				m_pVtxBuffMeshfield -> Unlock( );

				return height;
			}
		}
	}
	m_pVtxBuffMeshfield -> Unlock( );

	return height;
}


/*******************************************************************************
* �֐����FGetFieldNorm
* �����Fpos : ���݂̈ʒu���
* �߂�l�Fv1 �F �@��
* �����F���ݒn���炻�̍��W�̖@�����v�Z�ŋ��߂ĕԂ��֐�
*******************************************************************************/

D3DXVECTOR3 CMeshfield::GetFieldNorm(D3DXVECTOR3 pos) {
	// �ϐ��̐錾
	D3DXVECTOR2 coord = D3DXVECTOR2(0.0f, 0.0f);

	int i, j;
	VERTEX_3D *pVtx;
	D3DXVECTOR3 v1, v2;
	float n0, n1, n2;
	bool flag;

	float about_x, about_z;
	int start_x, end_x, start_z, end_z;

	about_x = (MESHFIELD_FIELD_WIDTH * 0.5f + pos.x) / MESHFIELD_FIELD_WIDTH * MESHFIELD_X_BLOCK;
	about_z = (MESHFIELD_FIELD_HEIGHT * 0.5f - pos.z) / MESHFIELD_FIELD_HEIGHT * MESHFIELD_Z_BLOCK;

	start_x = (int)about_x - 1;
	end_x = (int)about_x + 1;

	start_z = (int)about_z - 1;
	end_z = (int) about_z + 1;

	if(start_x < 0) {
		start_x = 0;
	}
	if(end_x > MESHFIELD_X_BLOCK) {
		end_x = MESHFIELD_X_BLOCK;
	}

	if(start_z < 0) {
		start_z = 0;
	}
	if(end_z > MESHFIELD_Z_BLOCK) {
		end_z = MESHFIELD_Z_BLOCK;
	}

	// �ϐ��̏�����
	n0 = n1 = n2 = 0;
	v1 = v2 = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	flag = false;

	// ���_���ւ̃|�C���^���擾
	m_pVtxBuffMeshfield -> Lock(0, 0, (void**)&pVtx, 0);

	// ���W�擾
	for(i = start_z; i < end_z; i++) {
		for(j = start_x; j < end_x; j++) {
			// �Ώۂ��E��|���S����ɂ��邩�ۂ��̔���
			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			// ���݂̃|���S����ɑΏۂ������ꍇ
			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				// �@�����v�Z���Đ��K��
				v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				v2 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				D3DXVec3Cross(&v1, &v1, &v2);
				D3DXVec3Normalize(&v1, &v1);

				return v1;
			}

			// �Ώۂ������|���S����ɂ��邩�ۂ��̔���
			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n0 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos;
			n1 = v1.x * v2.z - v1.z * v2.x;

			v1 = pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos;
			v2 = pos - pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos;
			n2 = v1.x * v2.z - v1.z * v2.x;

			if(n0 <= 0 && n1 <= 0 && n2 <= 0) {
				v1 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j + 1].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				v2 = pVtx[ (i + 1) * (MESHFIELD_X_BLOCK + 1) + j].pos - pVtx[i * (MESHFIELD_X_BLOCK + 1) + j].pos;
				D3DXVec3Cross(&v1, &v1, &v2);
				D3DXVec3Normalize(&v1, &v1);

				return v1;
			}
		}
	}
	m_pVtxBuffMeshfield -> Unlock( );

	return v1;
}