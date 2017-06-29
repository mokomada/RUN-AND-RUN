/*******************************************************************************
* �t�@�C���� object.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/09/27
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _OBJECT_H_
#define _OBJECT_H_


/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define OBJECT_NAME "data/MODEL/dog.x" // �ǂݍ���X�t�@�C����
#define OBJECT_TEXTURE_NAME NULL // �e�N�X�`����

#define OBJECT_SCALE_X (1.0f) // �X�P�[���l X��
#define OBJECT_SCALE_Y (1.0f) // �X�P�[���l Y��
#define OBJECT_SCALE_Z (1.0f) // �X�P�[���l Z��

#define OBJECT_ROT_X (0.0f) // ��]�� X��
#define OBJECT_ROT_Y (0.0f) // ��]�� Y��
#define OBJECT_ROT_Z (0.0f) // ��]�� Z��

#define OBJECT_POS_X (0.0f) // �ʒu��� X��
#define OBJECT_POS_Y (0.0f) // �ʒu��� Y��
#define OBJECT_POS_Z (0.0f) // �ʒu��� Z��

#define OBJECT_X_BLOCK (2) // X�����̃u���b�N��
#define OBJECT_Y_BLOCK (2) // Y�����̃u���b�N��

#define OBJECT_TEXTURE_X_BLOCK (1) // ���u���b�N��1���̃e�N�X�`�����g����(X����)
#define OBJECT_TEXTURE_Y_BLOCK (1) // ���u���b�N��1���̃e�N�X�`�����g����(Y����)

#define OBJECT_VERTEX_NUM ( (OBJECT_X_BLOCK + 1) * (OBJECT_Y_BLOCK + 1) ) // �`�悷�钸�_�̌�
#define OBJECT_INDEX_NUM ( (OBJECT_X_BLOCK + 1) * OBJECT_Y_BLOCK * 2 + (OBJECT_Y_BLOCK - 1) * 2) // �C���f�b�N�X�̌�
#define OBJECT_TRIANGLE_NUM (OBJECT_X_BLOCK * OBJECT_Y_BLOCK * 2 + (OBJECT_Y_BLOCK - 1) * 4) // �`�悷��O�p�`�̌�


/*******************************************************************************
* �\���̒�`
*******************************************************************************/



/*******************************************************************************
* �I�u�W�F�N�g�N���X
*******************************************************************************/

class CObject : public CScene{
private:
	//LPD3DXMESH m_pMesh; // ���b�V�����
	//LPD3DXBUFFER m_pBuffMat; // �}�e���A�����
	//DWORD m_numMat; // �}�e���A���̐�
	D3DXMATRIX m_mtxWorld; // ���[���h�}�g���b�N�X
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxTopBuff;
	LPDIRECT3DINDEXBUFFER9 m_pIdxTopBuff;
	MODEL m_objMng;
	MODEL m_object[5];
	float m_height;
	float m_width;
	D3DXVECTOR3 m_oldPlayerPos;
	bool m_playerOnFlag;
	bool m_alphaFlag;

public:
	CObject(int priority = 1, OBJTYPE objType = OBJTYPE_SOLID); // �R���X�g���N�^
	~CObject( ); // �f�X�g���N�^

	void Init(void);
	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	OBJHITCHK HitCheck(D3DXVECTOR3 pos, D3DXVECTOR3 oldPos);
	void SetAlphaFlag(bool onoff);
	bool GetAlphaFlag(void) {return m_alphaFlag;}

	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth); // �K�v�ɉ����ď����ʒu��p�x���̈�����
	static MODEL GetObject(void);
};


#endif
/*** �C���N���[�h�K�[�h�I�� ***/