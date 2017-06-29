/*******************************************************************************
* �t�@�C���� meshwater.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/5/18
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _MESHWATER_H_
#define _MESHWATER_H_

/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define MESHWATER_X_BLOCK (25) // X�����̃u���b�N��
#define MESHWATER_Z_BLOCK (25) // Z�����̃u���b�N��

#define MESHWATER_TEXTURE_X_BLOCK (1) // ���u���b�N��1���̃e�N�X�`�����g����(X����)
#define MESHWATER_TEXTURE_Z_BLOCK (1) // ���u���b�N��1���̃e�N�X�`�����g����(Z����)

#define MESHWATER_WATER_WIDTH (2000) // �t�B�[���h�̕�
#define MESHWATER_WATER_HEIGHT (2000) // �t�B�[���h�̍���

#define MESHWATER_MAX_HEIGHT (340) // ���ʂ̍ő�l
#define MESHWATER_MIN_HEIGHT (335) // ���ʂ̍ŏ��l

#define MESHWATER_MOVE_TIME (10) // ���ʂ̈ړ����x

#define MESHWATER_WATER_X (-1000) // �t�B�[���h�̍���[��X���W
#define MESHWATER_WATER_Z (1000) // �t�B�[���h�̍���[��Z���W

#define MESHWATER_VERTEX_NUM ( (MESHWATER_X_BLOCK + 1) * (MESHWATER_Z_BLOCK + 1) ) // �`�悷�钸�_�̌�
#define MESHWATER_INDEX_NUM ( (MESHWATER_X_BLOCK + 1) * MESHWATER_Z_BLOCK * 2 + (MESHWATER_Z_BLOCK - 1) * 2) // �C���f�b�N�X�̌�
#define MESHWATER_TRIANGLE_NUM (MESHWATER_X_BLOCK * MESHWATER_Z_BLOCK * 2 + (MESHWATER_Z_BLOCK - 1) * 4) // �`�悷��O�p�`�̌�

#define MESHWATER_TEXTURE_NAME NULL // �e�N�X�`���[�Ɏg���摜�w��
//#define MESHWATER_TEXTURE_NAME NULL // �e�N�X�`���[�ז��Ȏ��p


/*******************************************************************************
* ���b�V���t�B�[���h�N���X
*******************************************************************************/

class CMeshwater : public CScene{
private:
	LPDIRECT3DTEXTURE9 m_pTextureMeshwater; // �e�N�X�`���ւ̃|�C���^
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMeshwater; //���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffMeshwater; // �C���f�b�N�X�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3 m_posMeshwater; // �ʒu���
	D3DXVECTOR3 m_rotMeshwater; // ��]���
	D3DXVECTOR3 m_sclMeshwater; // �X�P�[���l�@��{�I�ɑS��1.0f�ŌŒ�

	D3DXMATRIX m_mtxWorldMeshwater; // ���[���h�}�g���b�N�X

	int m_time;

public:
	CMeshwater(int priority = 2, OBJTYPE objType = OBJTYPE_LIQUID); // �R���X�g���N�^
	~CMeshwater( ); // �f�X�g���N�^

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshwater *Create(void);
	static float GetWaterHeight(D3DXVECTOR3 pos);
};

#endif
/*** �C���N���[�h�K�[�h�I�� ***/