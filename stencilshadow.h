/*******************************************************************************
* �t�@�C���� object.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/09/27
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _STENCILSHADOW_H_
#define _STENCILSHADOW_H_


/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define STENCILSHADOW_NAME "data/MODEL/dog.x" // �ǂݍ���X�t�@�C����
#define STENCILSHADOW_TEXTURE_NAME NULL // �e�N�X�`����

#define STENCILSHADOW_SCALE_X (1.0f) // �X�P�[���l X��
#define STENCILSHADOW_SCALE_Y (1.0f) // �X�P�[���l Y��
#define STENCILSHADOW_SCALE_Z (1.0f) // �X�P�[���l Z��

#define STENCILSHADOW_ROT_X (0.0f) // ��]�� X��
#define STENCILSHADOW_ROT_Y (0.0f) // ��]�� Y��
#define STENCILSHADOW_ROT_Z (0.0f) // ��]�� Z��

#define STENCILSHADOW_POS_X (0.0f) // �ʒu��� X��
#define STENCILSHADOW_POS_Y (0.0f) // �ʒu��� Y��
#define STENCILSHADOW_POS_Z (0.0f) // �ʒu��� Z��

#define STENCILSHADOW_X_BLOCK (20) // X�����̃u���b�N��
#define STENCILSHADOW_Y_BLOCK (4) // Y�����̃u���b�N��

#define STENCILSHADOW_TEXTURE_X_BLOCK (1) // ���u���b�N��1���̃e�N�X�`�����g����(X����)
#define STENCILSHADOW_TEXTURE_Y_BLOCK (1) // ���u���b�N��1���̃e�N�X�`�����g����(Y����)

#define STENCILSHADOW_VERTEX_NUM ( (STENCILSHADOW_X_BLOCK + 1) * (STENCILSHADOW_Y_BLOCK + 1) ) // �`�悷�钸�_�̌�
#define STENCILSHADOW_INDEX_NUM ( (STENCILSHADOW_X_BLOCK + 1) * STENCILSHADOW_Y_BLOCK * 2 + (STENCILSHADOW_Y_BLOCK - 1) * 2) // �C���f�b�N�X�̌�
#define STENCILSHADOW_TRIANGLE_NUM (STENCILSHADOW_X_BLOCK * STENCILSHADOW_Y_BLOCK * 2 + (STENCILSHADOW_Y_BLOCK - 1) * 4) // �`�悷��O�p�`�̌�

#define STENCILSHADOW_VERTEX_FAN_NUM ( (STENCILSHADOW_X_BLOCK + 2) ) // �`�悷�钸�_�̌�(�W�j
#define STENCILSHADOW_TRIANGLE_FAN_NUM (STENCILSHADOW_X_BLOCK) // �`�悷��O�p�`�̌�(�W)

#define STENCILSHADOW_DISTANCE (4) // ���a

/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct{
	D3DXVECTOR3 pos; // �ʒu���
	D3DXVECTOR3 rot; // ��]���
	D3DXVECTOR3 scl; // �X�P�[���l�@��{�I�ɑS��1.0f�ŌŒ�
} STENCILSHADOW;

/*******************************************************************************
* �I�u�W�F�N�g�N���X
*******************************************************************************/

class CStencilShadow : public CScene{
private:
	LPD3DXMESH m_pMesh; // ���b�V�����
	//LPD3DXBUFFER m_pBuffMat; // �}�e���A�����
	//DWORD m_numMat; // �}�e���A���̐�
	D3DXMATRIX m_mtxWorld; // ���[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffLid;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffScreen;
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
	MODEL m_objMng;
	MODEL m_object[5];
	float m_height;
	float m_width;
	D3DXVECTOR3 m_oldPlayerPos;

	STENCILSHADOW m_stencilshadow; // �O�s���

public:
	CStencilShadow(int priority = 1, OBJTYPE objType = OBJTYPE_SOLID); // �R���X�g���N�^
	~CStencilShadow( ); // �f�X�g���N�^

	void Init(void);
	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CStencilShadow *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth); // �K�v�ɉ����ď����ʒu��p�x���̈�����
	static MODEL GetObject(void);
};


#endif
/*** �C���N���[�h�K�[�h�I�� ***/