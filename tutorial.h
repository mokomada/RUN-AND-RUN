/*******************************************************************************
* �t�@�C���� tutorial.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/07/19
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "mode.h"

/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define TUTORIAL_VERTEX_NUM (4) // �^�C�g����ʂ̕`�悷�钸�_�̌�
#define TUTORIAL_TRIANGLE_NUM (2) // �^�C�g����ʂ̕`�悷��O�p�`�̌�
#define TUTORIAL_TEXTURE_NAME ("data/TEXTURE/tutorial.jpg") // �^�C�g����ʂ̃e�N�X�`����
#define TUTORIAL_WAIT_TIME (600) // �ҋ@����
#define TUTORIAL_PRESSENTER_VERTEX_NUM (4) // ���S�̕`�悷�钸�_�̌�
#define TUTORIAL_PRESSENTER_TRIANGLE_NUM (2) // ���S�̕`�悷��O�p�`�̌�
#define TUTORIAL_PRESSENTER_TEXTURE_NAME ("data/TEXTURE/PressEnter.png") // ���S�̃e�N�X�`����
#define TUTORIAL_PRESSENTER_WIDTH (328) // ���S�̕�
#define TUTORIAL_PRESSENTER_HEIGHT (58) // ���S�̍���
#define TUTORIAL_PRESSENTER_POSY (600) // ���S�̕\���ʒu�iY���W�j
#define TUTORIAL_PRESSENTER_BLINK_TIME (30) // ���S�̓_�ŊԊu


/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/

class CScene; // �O���錾
class CCamera; // �O���錾
class CLight; // �O���錾
class CScore; // �O���錾
class CPlayer; // �O���錾
class CEnemy; // �O���錾


/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/
class CTutorial : public CMode{
private:
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pPETexture;
	LPDIRECT3DVERTEXBUFFER9 m_pPEVtxBuff;
	CCamera *m_camera; // �J����
	CLight *m_light; // ���C�g
	CPlayer *m_player; // �v���C���[���

	int m_timeCount;
	D3DXCOLOR m_PEAlpha; // PRESS ENTER�̃A���t�@�l�ݒ�p
	float m_alphaChange; // �A���t�@�l�̕ϓ��ʐݒ�

public:
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
};

#endif
/*** �C���N���[�h�K�[�h�I�� ***/