/*******************************************************************************
* �^�C�g�� �`���[�g���A���摜�\��
* �t�@�C���� tutorial.cpp
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/07/19
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "mode.h"
#include "fade.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "title.h"
#include "scene.h"
#include "input.h"
#include "sound.h"


/*******************************************************************************
* �֐����FInitTutorial
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�Q�[����ʂ̏�����
*******************************************************************************/

void CTutorial::Init(void) {
	// �ϐ��錾
	VERTEX_2D *pVtx;

	// �ϐ��̏�����
	m_timeCount = 0;
	m_alphaChange = 255.0f / (float)TUTORIAL_PRESSENTER_BLINK_TIME;
	m_PEAlpha = D3DXCOLOR(255, 255, 255, 255);

	// �ϐ��̎擾
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	m_pTexture = NULL;
	m_pVtxBuff = NULL;

	// �e�N�X�`���ݒ�
	D3DXCreateTextureFromFile(device, TUTORIAL_TEXTURE_NAME, &m_pTexture);

	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * TUTORIAL_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// ���_���ւ̃|�C���^���擾
	m_pVtxBuff -> Lock(0, 0, (void**)&pVtx, 0);

	// ���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	for(int i = 0; i < 4; i++) {
		pVtx[i].rhw = 1.0f;

		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// �e�N�X�`��UV���W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff -> Unlock( );

	// �e�N�X�`���ݒ�
	D3DXCreateTextureFromFile(device, TUTORIAL_PRESSENTER_TEXTURE_NAME, &m_pPETexture);

	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * TUTORIAL_PRESSENTER_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pPEVtxBuff, NULL);

	// ���_���ւ̃|�C���^���擾
	m_pPEVtxBuff -> Lock(0, 0, (void**)&pVtx, 0);

	// ���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3( (SCREEN_WIDTH - TUTORIAL_PRESSENTER_WIDTH) * 0.5f, TUTORIAL_PRESSENTER_POSY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3( (SCREEN_WIDTH - TUTORIAL_PRESSENTER_WIDTH) * 0.5f + TUTORIAL_PRESSENTER_WIDTH, TUTORIAL_PRESSENTER_POSY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3( (SCREEN_WIDTH - TUTORIAL_PRESSENTER_WIDTH) * 0.5f, TUTORIAL_PRESSENTER_POSY + TUTORIAL_PRESSENTER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3( (SCREEN_WIDTH - TUTORIAL_PRESSENTER_WIDTH) * 0.5f + TUTORIAL_PRESSENTER_WIDTH, TUTORIAL_PRESSENTER_POSY + TUTORIAL_PRESSENTER_HEIGHT, 0.0f);

	for(int i = 0; i < 4; i++) {
		pVtx[i].rhw = 1.0f;

		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// �e�N�X�`��UV���W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pPEVtxBuff -> Unlock( );
}


/*******************************************************************************
* �֐����FUninitTutorial
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�Q�[����ʂ̏I������
*******************************************************************************/

void CTutorial::Uninit(void) {
	if(m_pTexture != NULL) {
		m_pTexture -> Release( );
		m_pTexture = NULL;
	}

	if(m_pVtxBuff != NULL) {
		m_pVtxBuff -> Release( );
		m_pVtxBuff = NULL;
	}

		if(m_pPETexture != NULL) {
		m_pPETexture -> Release( );
		m_pPETexture = NULL;
	}

	if(m_pPEVtxBuff != NULL) {
		m_pPEVtxBuff -> Release( );
		m_pPEVtxBuff = NULL;
	}
}


/*******************************************************************************
* �֐����FUpdateTutorial
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�Q�[����ʂ̍X�V
*******************************************************************************/

void CTutorial::Update(void) {
	// �ϐ��̐錾
	VERTEX_2D *pVtx;

	// �ϐ��̎擾
	CInput *input;
	input = CManager::GetInput( );

	CFade *getFade;
	getFade = CManager::GetFade( );
	FADE fade = getFade -> GetFade( );

	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	if(fade == FADE_NONE) {
		// �_�ŏ���
		if(m_timeCount % (TUTORIAL_PRESSENTER_BLINK_TIME * 4) >= (TUTORIAL_PRESSENTER_BLINK_TIME * 3) ) {
			m_PEAlpha.a += m_alphaChange;;
			if(m_PEAlpha.a > 255) m_PEAlpha.a = 255;
		}
		else if(m_timeCount % (TUTORIAL_PRESSENTER_BLINK_TIME * 4) >= (TUTORIAL_PRESSENTER_BLINK_TIME * 2) ) {
			m_PEAlpha.a -= m_alphaChange;
			if(m_PEAlpha.a < 0) m_PEAlpha.a = 0;
		}

		m_timeCount++;

		// �G���^�[or�X�y�[�X�ŏ�ʑJ�ځB1�T�ڂ̂�3�b�ҋ@�i�A�łŃX�L�b�v���Ȃ��p�j�B2�T�ڈȍ~�̓t�F�[�h���I������u�Ԃ�����͎�t
		if( (input -> GetKeyboardTrigger(DIK_RETURN) || input -> GetKeyboardTrigger(DIK_SPACE) ) && (!CManager::m_firstTimeFlag || (CManager::m_firstTimeFlag && m_timeCount >= 180) ) ) {
			CFade::SetFade(FADE_OUT, new CGame);
			CManager::m_firstTimeFlag = false;
		}
	}
	else if(fade == FADE_OUT) {
		if(m_timeCount % 10 >= 5) {
			m_PEAlpha.a += m_alphaChange;;
			if(m_PEAlpha.a > 255) m_PEAlpha.a = 255;
		}
		else{
			m_PEAlpha.a -= m_alphaChange;
			if(m_PEAlpha.a < 0) m_PEAlpha.a = 0;
		}

		m_timeCount++;
	}

	m_pPEVtxBuff -> Lock(0, 0, (void**)&pVtx, 0);

	for(int i = 0; i < 4; i++) {
		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, (int)m_PEAlpha.a);
	}

	m_pPEVtxBuff -> Unlock( );
}


/*******************************************************************************
* �֐����FDrawTutorial
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�Q�[����ʂ̕`��
*******************************************************************************/

void CTutorial::Draw(void) {
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D) );

	// �e�N�X�`���̐ݒ�
	device -> SetTexture(0, m_pTexture);

	// 2D�|���S���̕`��
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		FADE_TRIANGLE_NUM);

	// PRESS ENTER�\��
	// �u�����h�ݒ�
	device ->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);// �����̐�������Z����
	device ->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_pPEVtxBuff, 0, sizeof(VERTEX_2D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	device -> SetTexture(0, m_pPETexture);

	// 2D�|���S���̕`��
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		TITLE_TRIANGLE_NUM);

	// �u�����h�ݒ��߂�
	device ->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );// �����̐�������Z����
	device ->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT);
}
