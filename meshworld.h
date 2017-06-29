/*******************************************************************************
* �t�@�C���� meshworld.h
* �쐬�� AT-12C-245 �H�ԗY��
* �쐬�� 2015/12/10
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _MESHWORLD_H_
#define _MESHWORLD_H_

/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define MESHWORLD_NUM (2) // �O�s�̐�

#define MESHWORLD_X_BLOCK (32) // X�����̃u���b�N�� 1�ȏ�̐���
#define MESHWORLD_Y_BLOCK (32) // Y�����̃u���b�N�� 1�ȏ�̐���

#define MESHWORLD_TEXTURE_X_BLOCK (32) // ���u���b�N��1���̃e�N�X�`�����g����(X����)
#define MESHWORLD_TEXTURE_Y_BLOCK (16) // ���u���b�N��1���̃e�N�X�`�����g����(Y����)
#define MESHWORLD_TEXTURE_START_X (0) // ���u���b�N�ڂ���e�N�X�`����K�p���n�߂邩(X����)
#define MESHWORLD_TEXTURE_START_Y (16) // ���u���b�N�ڂ���e�N�X�`����K�p���n�߂邩(Y����)
#define MESHWORLD_TEXTURE_ADJUST (0.003f) // �e�N�X�`�������p�i��ԏォ��e�N�X�`����K�p�J�n����u���b�N�܂ŁA�e�N�X�`���̂ǂ̈ʒu���������΂����j

#define MESHWORLD_FIELD_X (-MESHWORLD_FIELD_WIDTH * 0.5f) // �O�s�̍���[��X���W
#define MESHWORLD_FIELD_Y (MESHWORLD_FIELD_HEIGHT) // �O�s�̍���[��Y���W

#define MESHWORLD_VERTEX_NUM ( (MESHWORLD_X_BLOCK + 1) * (MESHWORLD_Y_BLOCK + 1) ) // �`�悷�钸�_�̌�
#define MESHWORLD_INDEX_NUM ( (MESHWORLD_X_BLOCK + 1) * MESHWORLD_Y_BLOCK * 2 + (MESHWORLD_Y_BLOCK - 1) * 2) // �C���f�b�N�X�̌�
#define MESHWORLD_TRIANGLE_NUM (MESHWORLD_X_BLOCK * MESHWORLD_Y_BLOCK * 2 + (MESHWORLD_Y_BLOCK - 1) * 4) // �`�悷��O�p�`�̌�

#define MESHWORLD_DISTANCE (2000) // �O�s�̔��a

#define MESHWORLD_VERTEX_FAN_NUM ( (MESHWORLD_X_BLOCK + 2) ) // �`�悷�钸�_�̌�(�W�j
#define MESHWORLD_TRIANGLE_FAN_NUM (MESHWORLD_X_BLOCK) // �`�悷��O�p�`�̌�(�W)


#define MESHWORLD_TEXTURE_NAME "data/TEXTURE/sky000.jpg" // �e�N�X�`���[�Ɏg���摜�w��


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct{
	D3DXVECTOR3 pos; // �ʒu���
	D3DXVECTOR3 rot; // ��]���
	D3DXVECTOR3 scl; // �X�P�[���l�@��{�I�ɑS��1.0f�ŌŒ�
} MESHWORLD;


/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/

class CMeshworld : public CScene{
private:
	LPDIRECT3DTEXTURE9 m_pTexture; // �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; //���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffLid; //���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff; // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3 m_pos; // �ʒu���
	D3DXVECTOR3 m_rot; // ��]���
	D3DXVECTOR3 m_scl; // �X�P�[���l�@��{�I�ɑS��1.0f�ŌŒ�

	D3DXMATRIX m_mtxWorld; // ���[���h�}�g���b�N�X

	MESHWORLD m_meshworld; // �O�s���

public:
	CMeshworld(int priority = 2, OBJTYPE objType = OBJTYPE_FIELD); // �R���X�g���N�^
	~CMeshworld( ); // �f�X�g���N�^

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshworld *Create(void);
};
#endif
/*** �C���N���[�h�K�[�h�I�� ***/