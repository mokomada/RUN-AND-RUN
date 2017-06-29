/*******************************************************************************
* �t�@�C���� title.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/07/19
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _TITLE_H_
#define _TITLE_H_

#include "mode.h"

/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define TITLE_VERTEX_NUM (4) // �^�C�g����ʂ̕`�悷�钸�_�̌�
#define TITLE_TRIANGLE_NUM (2) // �^�C�g����ʂ̕`�悷��O�p�`�̌�
#define TITLE_TEXTURE_NAME ("data/TEXTURE/title.jpg") // �^�C�g����ʂ̃e�N�X�`����
#define TITLE_LOGO_VERTEX_NUM (4) // ���S�̕`�悷�钸�_�̌�
#define TITLE_LOGO_TRIANGLE_NUM (2) // ���S�̕`�悷��O�p�`�̌�
#define TITLE_LOGO_TEXTURE_NAME ("data/TEXTURE/PressEnter.png") // ���S�̃e�N�X�`����
#define TITLE_LOGO_WIDTH (328) // ���S�̕�
#define TITLE_LOGO_HEIGHT (58) // ���S�̍���
#define TITLE_LOGO_POSY (600) // ���S�̕\���ʒu�iY���W�j
#define TITLE_LOGO_BLINK_TIME (30) // ���S�̓_�ŊԊu
#define TITLE_WAIT_TIME (600) // �ҋ@����



/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
class CTitle : public CMode{
private:
	LPDIRECT3DTEXTURE9 m_pTextureTitle; // �^�C�g����ʗp�e�N�X�`���ւ̃|�C��
	LPDIRECT3DTEXTURE9 m_pTextureTitleLogo; // ���S�p�e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffTitle; // �^�C�g����ʂ̒��_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffTitleLogo; // ���S�̒��_�o�b�t�@�ւ̃|�C���^
	int m_title_time_count; // �J�E���^�[
	D3DXCOLOR m_title_logo_alpha; // ���S�̃A���t�@�l�ݒ�p
	float m_alpha_change; // �A���t�@�l�̕ϓ��ʐݒ�

public:
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
};

#endif
/*** �C���N���[�h�K�[�h�I�� ***/