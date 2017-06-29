/*******************************************************************************
* �t�@�C���� score.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/06/24
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _SCORE_H_
#define _SCORE_H_

/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define NUMBER_NUM (3) // �\������X�R�A�̌���
#define SCORE_VERTEX_NUM (4 * NUMBER_NUM) // �`�悷�钸�_�̌�
#define SCORE_TRIANGLE_NUM (4 * NUMBER_NUM - 2) // �`�悷��O�p�`�̌�
#define SCORE_TEXTURE_NAME ("data/TEXTURE/number000.png") // �e�N�X�`����

#define BOX_WIDTH (35) // 1�̐����̕�
#define BOX_HEIGHT (70) // 1�̐����̍���
#define TEX_LENGTH (0.1) // �e�N�X�`���̐����̊Ԋu

#define SCORE_MIN (0) // �X�R�A�̍ŏ��l
#define SCORE_MAX (999 * 60) // �X�R�A�̍ő�l


/*******************************************************************************
* �X�R�A�N���X
*******************************************************************************/

class CScore {
private:
	LPDIRECT3DTEXTURE9 m_pTextureScore; // �X�R�A�p�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffScore; //���_�o�b�t�@�ւ̃|�C���^
	static int m_nScore; /* �X�R�A�̒l */

public:
	CScore(int priority = 2, OBJTYPE objType = OBJTYPE_NONE); // �R���X�g���N�^
	~CScore( ); // �f�X�g���N�^

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void AddScore(int score);
	static int GetScore(void);
	static CScore *Create(void);
};

#endif
/*** �C���N���[�h�K�[�h�I�� ***/