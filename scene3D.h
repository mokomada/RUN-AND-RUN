/*******************************************************************************
* �t�@�C���� scene3D.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/04/20
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _SCENE3D_H_
#define _SCENE3D_H_


/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define VERTEX_NUM (4) // �`�悷�钸�_�̌�
#define TRIANGLE_NUM (2) // �`�悷��O�p�`�̌�

#define FIELD_X (-100) // �|���S���̕`��ʒu�iX���W�j
#define FIELD_Z (-100) // �|���S���̕`��ʒu�iZ���W�j
#define FIELD_WIDTH (200) // �|���S���̕�
#define FIELD_HEIGHT (200) // �|���S���̉��s��

//#define FIELD_TEXTURE_NAME ("data/TEXTURE/field003.jpg") // �e�N�X�`����
#define FIELD_TEXTURE_NAME (NULL) // �e�N�X�`����

#define SCENE3D_PRIORITY_NUM (1) // �D��l

/*******************************************************************************
* �\���̒�`
*******************************************************************************/


/*******************************************************************************
* 3D�|���S���N���X
*******************************************************************************/

class CScene3D : public CScene{
private:
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer; // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_TexturePolygon; // �e�N�X�`���ւ̃|�C���^

	D3DXMATRIX m_mtxWorld; // ���[���h�}�g���N�X


public:
	CScene3D(int priority = 1, OBJTYPE objType = OBJTYPE_NONE); // �R���X�g���N�^
	~CScene3D( ); // �f�X�g���N�^

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScene3D *Create(void); // �K�v�ɉ����ď����ʒu��p�x���̈�����
};


#endif
/*** �C���N���[�h�K�[�h�I�� ***/