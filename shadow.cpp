/*******************************************************************************
* �^�C�g�� �e�`�揈��
* �t�@�C���� shadow.cpp
* �쐬�� AT-12C-245 �H�ԗY��
* �쐬�� 2015/11/12
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "scene.h"
#include "shadow.h"
#include "renderer.h"


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct {
	D3DXVECTOR3 pos; // �ʒu���
	D3DXVECTOR3 *obj_pos; // �e���t���Ώۂ̍��W���ւ̃|�C���^
	D3DXVECTOR3 rot; // ��]���
	D3DXVECTOR3 scl; // �X�P�[���l ��{�I�ɑS��1.0f�ŌŒ�
	float size; // �e�̃T�C�Y
	bool use; // �g�p������
}SHADOW;


/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/

LPDIRECT3DTEXTURE9 g_pTextureShadow = NULL; // �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffShadow = NULL; //���_�o�b�t�@�ւ̃|�C���^

SHADOW g_shadow[SHADOW_MAX]; // �e�̏��

D3DXMATRIX g_mtxWorldShadow; // ���[���h�}�g���b�N�X


/*******************************************************************************
* �֐����FInitShadow
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�e�ݒ�̏�����
*******************************************************************************/

void InitShadow(void) {
	// �ϐ��錾
	int i; // for���[�v�p
	VERTEX_3D *pVtx;

	// �O���[�o���ϐ��̏�����

	// �ϐ��̎擾
	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer= CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 pDevice = renderer -> GetDevice( );

	// �e�N�X�`���ݒ�
	D3DXCreateTextureFromFile(pDevice, SHADOW_TEXTURE_NAME, &g_pTextureShadow);

	// ���_�o�b�t�@�̐���
	pDevice -> CreateVertexBuffer(sizeof(VERTEX_3D) * SHADOW_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &g_pVtxBuffShadow, NULL);
	

	// ���_���ւ̃|�C���^���擾
	g_pVtxBuffShadow -> Lock(0, 0, (void**)&pVtx, 0);

	// �p�x�����ݒ�
	for(i = 0; i < SHADOW_MAX; i++) {
		g_shadow[i].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shadow[i].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_shadow[i].scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		g_shadow[i].obj_pos = NULL;
		g_shadow[i].use = false;
	}

	// ���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(-20, 0.0f, 20);
	pVtx[1].pos = D3DXVECTOR3(20, 0.0f, 20);
	pVtx[2].pos = D3DXVECTOR3(-20, 0.0f, -20);
	pVtx[3].pos = D3DXVECTOR3(20, 0.0f, -20);

	for(i = 0; i < SHADOW_VERTEX_NUM; i++) {
		pVtx[i].col = D3DCOLOR_RGBA(SHADOW_DEFAULT_COLOR, SHADOW_DEFAULT_COLOR, SHADOW_DEFAULT_COLOR, 255);
	}

	// �@���ݒ�
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);


	// �e�N�X�`��UV���W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	g_pVtxBuffShadow -> Unlock( );
}


/*******************************************************************************
* �֐����FUninitShadow
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�e�̏I������
*******************************************************************************/

void UninitShadow(void) {
	if(g_pTextureShadow != NULL) {
		g_pTextureShadow -> Release( ); // �e�N�X�`���̊J��
		g_pTextureShadow = NULL;
	}

	if(g_pVtxBuffShadow != NULL) {
		g_pVtxBuffShadow -> Release( );
		g_pVtxBuffShadow = NULL;
	}
}


/*******************************************************************************
* �֐����FUpdataShadow
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�e�̏��X�V
*******************************************************************************/

void UpdateShadow(void) {
	D3DXVECTOR3 obj_pos; // �ʒu���擾�p
	int i; // for�����[�v�p

	for(i = 0; i < SHADOW_MAX; i++) {
		if(g_shadow[i].use) {
			obj_pos = *g_shadow[i].obj_pos;
			g_shadow[i].pos.x = obj_pos.x;
			g_shadow[i].pos.z = obj_pos.z;
		}
	}
}


/*******************************************************************************
* �֐����FDrawShadow
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�e�̕`�揈��
*******************************************************************************/

void DrawShadow(void) {
	int i, j; // for�����[�v�p

	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer= CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 pDevice = renderer -> GetDevice( );
	VERTEX_3D *pVtx;
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // �v�Z�p
	D3DXVECTOR3 obj_pos; // �ʒu���擾�p

	// �A���t�@�u�����h�����Z�����ɐݒ�
	pDevice -> SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_REVSUBTRACT);
	pDevice -> SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �A���t�@�e�X�g�ݒ�
	pDevice -> SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice -> SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice -> SetRenderState(D3DRS_ALPHAREF, 0);


	for(i = 0; i < SHADOW_MAX; i++) {
		if(g_shadow[i].use) {
			// �e�̃T�C�Y�ݒ�
			obj_pos = *g_shadow[i].obj_pos; // �ʒu���擾

			g_pVtxBuffShadow -> Lock(0, 0, (void**)&pVtx, 0);

			pVtx[0].pos = D3DXVECTOR3(-g_shadow[i].size - g_shadow[i].size * (obj_pos.y / 100), 0.1f, g_shadow[i].size + g_shadow[i].size * (obj_pos.y / 100) );
			pVtx[1].pos = D3DXVECTOR3(g_shadow[i].size + g_shadow[i].size * (obj_pos.y / 100), 0.1f, g_shadow[i].size + g_shadow[i].size * (obj_pos.y / 100) );
			pVtx[2].pos = D3DXVECTOR3(-g_shadow[i].size - g_shadow[i].size * (obj_pos.y / 100), 0.1f, -g_shadow[i].size - g_shadow[i].size * (obj_pos.y / 100));
			pVtx[3].pos = D3DXVECTOR3(g_shadow[i].size + g_shadow[i].size * (obj_pos.y / 100), 0.1f, -g_shadow[i].size - g_shadow[i].size * (obj_pos.y / 100));

			for(j = 0; j < SHADOW_VERTEX_NUM; j++) {
				pVtx[j].col = D3DCOLOR_RGBA(SHADOW_DEFAULT_COLOR - int(obj_pos.y), SHADOW_DEFAULT_COLOR - int(obj_pos.y), SHADOW_DEFAULT_COLOR - int(obj_pos.y), 255);
				if(SHADOW_DEFAULT_COLOR - int(obj_pos.y) < 0) {
					pVtx[j].col = D3DCOLOR_RGBA(0, 0, 0, 255);
				}
			}

			g_pVtxBuffShadow -> Unlock( );

			// ���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&g_mtxWorldShadow);
			// ���[���h�}�g���b�N�X�̍쐬
			// �X�P�[���𔽉f
			D3DXMatrixScaling(&mtxScl, g_shadow[i].scl.x, g_shadow[i].scl.y, g_shadow[i].scl.z);
			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxScl);

			// ��]�𔽉f
			D3DXMatrixRotationYawPitchRoll(&mtxRot, g_shadow[i].rot.y, g_shadow[i].rot.x, g_shadow[i].rot.z); // ������ y, x, z �̏��Ԃɒ��ӁI�I�I ������
			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxRot);

			// �ړ��𔽉f
			D3DXMatrixTranslation(&mtxTrans, g_shadow[i].pos.x, g_shadow[i].pos.y, g_shadow[i].pos.z);
			D3DXMatrixMultiply(&g_mtxWorldShadow, &g_mtxWorldShadow, &mtxTrans);

			// ���[���h�}�g���b�N�X�̐ݒ�
			pDevice -> SetTransform(D3DTS_WORLD, &g_mtxWorldShadow);

			// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
			pDevice -> SetStreamSource(0, g_pVtxBuffShadow, 0, sizeof(VERTEX_3D) );

			// ���_�t�H�[�}�b�g�̐ݒ�
			pDevice -> SetFVF(FVF_VERTEX_3D);

			// �e�N�X�`���̐ݒ�
			pDevice -> SetTexture(0, g_pTextureShadow);

			// �|���S���̕`��
			pDevice -> DrawPrimitive(
				D3DPT_TRIANGLESTRIP,
				0,
				SHADOW_TRIANGLE_NUM);
		}
	}
	// �����_�[�X�e�[�g�ݒ�����ɖ߂�
	pDevice -> SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice -> SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	pDevice -> SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice -> SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice -> SetRenderState(D3DRS_ALPHAREF, 0);
}


/*******************************************************************************
* �֐����FSetShadow
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�e���̂ɑ΂��e��ݒ肷��
*******************************************************************************/

int SetShadow(D3DXVECTOR3 *new_pos, float width, float height) {
	int i; // for�����[�v�p

	for(i = 0; i < SHADOW_MAX; i++) {
		if(g_shadow[i].use == false) {
			g_shadow[i].pos = *new_pos;
			g_shadow[i].pos.y = 0.0f;
			g_shadow[i].obj_pos = new_pos;
			g_shadow[i].size = (width + height) * 0.25f;
			g_shadow[i].use = true;
			break;
		}
	}
	return i;
}



/*******************************************************************************
* �֐����FDeleteShadow
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�e����������
*******************************************************************************/

void DeleteShadow(int num) {
	g_shadow[num].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_shadow[num].obj_pos = NULL;
	g_shadow[num].use = false;
}