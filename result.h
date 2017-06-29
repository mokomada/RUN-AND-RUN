/*******************************************************************************
* �t�@�C���� game.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/07/19
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _RESULT_H_
#define _RESULT_H_

#include "mode.h"

/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define RESULT_VERTEX_NUM (4) // ���U���g��ʂ̕`�悷�钸�_�̌�
#define RESULT_TRIANGLE_NUM (2) // ���U���g��ʂ̕`�悷��O�p�`�̌�
#define RESULT_TEXTURE_NAME ("data/TEXTURE/bg102.jpg") // ���U���g��ʂ̃e�N�X�`����

#define RESULT_LOGO_VERTEX_NUM (4) // ���S�̕`�悷�钸�_�̌�
#define RESULT_LOGO_TRIANGLE_NUM (2) // ���S�̕`�悷��O�p�`�̌�
#define RESULT_LOGO_TEXTURE_NAME ("data/TEXTURE/result.jpg") // ���S�̃e�N�X�`����
#define RESULT_LOGO_WIDTH (SCREEN_WIDTH) // ���S�̕�
#define RESULT_LOGO_HEIGHT (SCREEN_HEIGHT) // ���S�̍���

#define RESULT_SCORE_WIDTH (40) // �X�R�A�̕�
#define RESULT_SCORE_HEIGHT (80) // �X�R�A�̍���
#define RESULT_SCORE_NUM (5) // �X�R�A�̕\����
#define RESULT_SCORE_SPACE (40) // �X�R�A�̕\���Ԋu

#define RESULT_WAIT_TIME (450) // ���U���g��ʂ̑ҋ@����
#define RESULT_STOP_MOVE_TIME (300) // �X�R�A�̈ړ����~�܂鎞��


/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
class CResult : public CMode{
private:
	LPDIRECT3DTEXTURE9 m_pTextureResultScore[RESULT_SCORE_NUM]; // �X�R�A�p�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffResultScore[RESULT_SCORE_NUM]; //���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_pTextureResultLogo; // ���S�p�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffResultLogo; // ���S�̒��_�o�b�t�@�ւ̃|�C���^
	int result_wait_time;
	static int m_result_score[RESULT_SCORE_NUM];
public:
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
};

#endif
/*** �C���N���[�h�K�[�h�I�� ***/