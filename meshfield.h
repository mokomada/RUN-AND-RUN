/*******************************************************************************
* �t�@�C���� meshfield.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/5/18
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define MESHFIELD_X_BLOCK (40) // X�����̃u���b�N��
#define MESHFIELD_Z_BLOCK (40) // Z�����̃u���b�N��

#define MESHFIELD_TEXTURE_X_BLOCK (1) // ���u���b�N��1���̃e�N�X�`�����g����(X����)
#define MESHFIELD_TEXTURE_Z_BLOCK (1) // ���u���b�N��1���̃e�N�X�`�����g����(Z����)

#define MESHFIELD_FIELD_WIDTH (500) // �t�B�[���h�̕�
#define MESHFIELD_FIELD_HEIGHT (500) // �t�B�[���h�̍���

#define MESHFIELD_FIELD_X (-250) // �t�B�[���h�̍���[��X���W
#define MESHFIELD_FIELD_Z (250) // �t�B�[���h�̍���[��Z���W

#define MESHFIELD_VERTEX_NUM ( (MESHFIELD_X_BLOCK + 1) * (MESHFIELD_Z_BLOCK + 1) ) // �`�悷�钸�_�̌�
#define MESHFIELD_INDEX_NUM ( (MESHFIELD_X_BLOCK + 1) * MESHFIELD_Z_BLOCK * 2 + (MESHFIELD_Z_BLOCK - 1) * 2) // �C���f�b�N�X�̌�
#define MESHFIELD_TRIANGLE_NUM (MESHFIELD_X_BLOCK * MESHFIELD_Z_BLOCK * 2 + (MESHFIELD_Z_BLOCK - 1) * 4) // �`�悷��O�p�`�̌�

#define MESHFIELD_TEXTURE_NAME "data/TEXTURE/field001.jpg" // �e�N�X�`���[�Ɏg���摜�w��
//#define MESHFIELD_TEXTURE_NAME NULL // �e�N�X�`���[�ז��Ȏ��p

#define MESHFIELD_DIFFERENCE_MAX (3.0f) // �ׂ̒��_�Ƃ̍����̍��̍ő�l


/*******************************************************************************
* ���b�V���t�B�[���h�N���X
*******************************************************************************/

class CMeshfield : public CScene{
private:
	LPDIRECT3DTEXTURE9 m_pTextureMeshfield; // �e�N�X�`���ւ̃|�C���^
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMeshfield; //���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffMeshfield; // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3 m_posMeshfield; // �ʒu���
	D3DXVECTOR3 m_rotMeshfield; // ��]���
	D3DXVECTOR3 m_sclMeshfield; // �X�P�[���l�@��{�I�ɑS��1.0f�ŌŒ�

	D3DXMATRIX m_mtxWorldMeshfield; // ���[���h�}�g���b�N�X

	float m_HeightMap[MESHFIELD_X_BLOCK + 1][MESHFIELD_Z_BLOCK + 1]; // �e���W�̍���

public:
	CMeshfield(int priority = 1, OBJTYPE objType = OBJTYPE_FIELD); // �R���X�g���N�^
	~CMeshfield( ); // �f�X�g���N�^

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshfield *Create(void);
	static float GetFieldHeight(D3DXVECTOR3 pos);
	static D3DXVECTOR3 GetFieldNorm(D3DXVECTOR3 pos);
};

#endif
/*** �C���N���[�h�K�[�h�I�� ***/