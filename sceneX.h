/*******************************************************************************
* �t�@�C���� sceneX.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/04/26
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _SCENEX_H_
#define _SCENEX_H_


/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define MODEL_MOVE_SPEED (0.5f) // ���f���̈ړ����x
#define MODEL_ROT_SPEED (0.01f) // ���f���̉�]���x
#define MODEL_JUMP_SPEED (1.5f) // ���f���̃W�����v��
#define MODEL_FALL_SPEED (0.1f) // ���f���̗������x


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct
{
	D3DXVECTOR3 pos; // �ʒu���
	D3DXVECTOR3 move; // �ړ��ʏ��
	D3DXQUATERNION rot; // ��]���
	D3DXVECTOR3 scl; // �X�P�[���l�@��{�I�ɑS��1.0f�ŌŒ�
	int shadow_num; // ���g�̉e�̔ԍ�
	bool jump; // �W�����v�����ۂ�
	bool onObject;
} MODEL;


/*******************************************************************************
* 3D�|���S���N���X
*******************************************************************************/

class CSceneX : public CScene{
protected:
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPD3DXMESH m_pMesh; // ���b�V�����
	LPD3DXBUFFER m_pBuffMat; // �}�e���A�����
	DWORD m_numMat; // �}�e���A���̐�
	D3DXMATRIX m_mtxWorld; // ���[���h�}�g���b�N�X

	MODEL m_model; // ���f�����
	MODEL m_oldModel; // �O�t���[���̃��f�����
	float m_newRot; // �i�s�����ۑ��p

	void SetNewWay(float set);

public:
	CSceneX(int priority = 1, OBJTYPE objType = OBJTYPE_NONE); // �R���X�g���N�^
	~CSceneX( ); // �f�X�g���N�^

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CSceneX *Create(void); // �K�v�ɉ����ď����ʒu��p�x���̈�����

	MODEL GetModel(void) {return m_model;} // ���f������Ԃ�
};


#endif
/*** �C���N���[�h�K�[�h�I�� ***/