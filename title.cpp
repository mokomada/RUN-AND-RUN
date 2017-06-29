/*******************************************************************************
* �^�C�g�� �^�C�g����ʕ`�揈��
* �t�@�C���� title.cpp
* �쐬�� AT-12C-245 �H�ԗY��
* �쐬�� 2015/06/24
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "mode.h"
#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "input.h"
#include "scene.h"
#include "sound.h"


/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/


/*******************************************************************************
* �֐����FInitTitle
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�^�C�g����ʐݒ�̏�����
*******************************************************************************/

void CTitle::Init(void) {
	// �ϐ��錾
	int i; // for�����[�v�p
	VERTEX_2D *pVtx;

	// �ϐ��̏�����
	m_title_time_count = 0;
	m_alpha_change = 255.0f / (float)TITLE_LOGO_BLINK_TIME;
	m_title_logo_alpha = D3DXCOLOR(255, 255, 255, 255);
	m_pTextureTitle = NULL; // �^�C�g����ʗp�e�N�X�`���ւ̃|�C���^
	m_pTextureTitleLogo = NULL; // ���S�p�e�N�X�`���ւ̃|�C���^
	m_pVtxBuffTitle = NULL; // �^�C�g����ʂ̒��_�o�b�t�@�ւ̃|�C���^
	m_pVtxBuffTitleLogo = NULL; // ���S�̒��_�o�b�t�@�ւ̃|�C���^

	// �ϐ��̎擾
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	// �^�C�g����ʃe�N�X�`���ݒ�
	D3DXCreateTextureFromFile(device, TITLE_TEXTURE_NAME, &m_pTextureTitle);

	// ���_�o�b�t�@�̐���
	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * TITLE_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffTitle, NULL);

	// ���_���ւ̃|�C���^���擾
	m_pVtxBuffTitle -> Lock(0, 0, (void**)&pVtx, 0);

	// ���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);


	for(i = 0; i < TITLE_VERTEX_NUM; i++) {
		pVtx[i].rhw = 1.0f;

		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// �e�N�X�`��UV���W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuffTitle -> Unlock( );

	// ���S�e�N�X�`���ݒ�
	D3DXCreateTextureFromFile(device, TITLE_LOGO_TEXTURE_NAME, &m_pTextureTitleLogo);

	// ���_�o�b�t�@�̐���
	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * TITLE_LOGO_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffTitleLogo, NULL);

	m_pVtxBuffTitleLogo -> Lock(0, 0, (void**)&pVtx, 0);

	// ���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3( (SCREEN_WIDTH - TITLE_LOGO_WIDTH) * 0.5f, TITLE_LOGO_POSY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3( (SCREEN_WIDTH - TITLE_LOGO_WIDTH) * 0.5f + TITLE_LOGO_WIDTH, TITLE_LOGO_POSY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3( (SCREEN_WIDTH - TITLE_LOGO_WIDTH) * 0.5f, TITLE_LOGO_POSY + TITLE_LOGO_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3( (SCREEN_WIDTH - TITLE_LOGO_WIDTH) * 0.5f + TITLE_LOGO_WIDTH, TITLE_LOGO_POSY + TITLE_LOGO_HEIGHT, 0.0f);


	for(i = 0; i < TITLE_VERTEX_NUM; i++) {
		pVtx[i].rhw = 1.0f;

		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// �e�N�X�`��UV���W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuffTitleLogo -> Unlock( );

	CSound::StopSound(SOUND_LABEL_BGM000);
	CSound::PlaySound(SOUND_LABEL_BGM001);
}


/*******************************************************************************
* �֐����FUninitTitle
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�^�C�g����ʐݒ�̏I������
*******************************************************************************/

void CTitle::Uninit(void) {
	if(m_pTextureTitle != NULL) {
		m_pTextureTitle -> Release( ); // �e�N�X�`���̊J��
		m_pTextureTitle = NULL;
	}

	if(m_pVtxBuffTitle != NULL) {
		m_pVtxBuffTitle -> Release( ); // ���_�o�b�t�@�̊J��
		m_pVtxBuffTitle = NULL;
	}

	if(m_pTextureTitleLogo != NULL) {
		m_pTextureTitleLogo -> Release( );
		m_pTextureTitleLogo = NULL;
	}

	if(m_pVtxBuffTitleLogo != NULL) {
		m_pVtxBuffTitleLogo -> Release( );
		m_pVtxBuffTitleLogo = NULL;
	}
}


/*******************************************************************************
* �֐����FUpdataTitle
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�^�C�g����ʂ̏��X�V
*******************************************************************************/

void CTitle::Update(void) {
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

	// �t�F�[�h�C�����łȂ����PRESS ENTER�̓_�ŏ����ƃL�[���͂ɂ���ʑJ�ڔ���
	if(fade == FADE_NONE) {
		// �_�ŏ���
		if(m_title_time_count % (TITLE_LOGO_BLINK_TIME * 4) >= (TITLE_LOGO_BLINK_TIME * 3) ) {
			m_title_logo_alpha.a += m_alpha_change;;
			if(m_title_logo_alpha.a > 255) m_title_logo_alpha.a = 255;
		}
		else if(m_title_time_count % (TITLE_LOGO_BLINK_TIME * 4) >= (TITLE_LOGO_BLINK_TIME * 2) ) {
			m_title_logo_alpha.a -= m_alpha_change;
			if(m_title_logo_alpha.a < 0) m_title_logo_alpha.a = 0;
		}

		m_title_time_count++;

		// ����̃L�[�������ꂽ��`���[�g���A���Ɉȍ~
		if(input -> GetKeyboardTrigger(DIK_RETURN) || input -> GetKeyboardTrigger(DIK_SPACE) ) {
			CFade::SetFade(FADE_OUT, new CTutorial);
			m_alpha_change = 255.0f / 5.0f;
			m_title_logo_alpha.a = 255;
			m_title_time_count = 0;
		}
	}
	else if(fade == FADE_OUT) {
		if(m_title_time_count % 10 >= 5) {
			m_title_logo_alpha.a += m_alpha_change;;
			if(m_title_logo_alpha.a > 255) m_title_logo_alpha.a = 255;
		}
		else{
			m_title_logo_alpha.a -= m_alpha_change;
			if(m_title_logo_alpha.a < 0) m_title_logo_alpha.a = 0;
		}

		m_title_time_count++;
	}

	m_pVtxBuffTitleLogo -> Lock(0, 0, (void**)&pVtx, 0);

	for(int i = 0; i < TITLE_VERTEX_NUM; i++) {
//		pVtx[i].col = D3DCOLOR_RGBA( (int)m_title_logo_alpha.a, (int)m_title_logo_alpha.a, (int)m_title_logo_alpha.a, 255);
		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, (int)m_title_logo_alpha.a);
	}

	m_pVtxBuffTitleLogo -> Unlock( );
}


/*******************************************************************************
* �֐����FDrawTitle
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�^�C�g����ʂ̕`�揈��
*******************************************************************************/

void CTitle::Draw(void) {
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	device -> SetStreamSource(0, m_pVtxBuffTitle, 0, sizeof(VERTEX_2D) );

	device -> SetTexture(0, m_pTextureTitle);

	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		TITLE_TRIANGLE_NUM);

	// ���S�i�ƌ�������PRESS ENTER�j�̕`��
	// �u�����h�ݒ�
	device ->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);// �����̐�������Z����
	device ->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	device -> SetTexture(0, m_pTextureTitleLogo);

	// 2D�|���S���̕`��
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		TITLE_TRIANGLE_NUM);

	// �u�����h�ݒ��߂�
	device ->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );// �����̐�������Z����
	device ->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT);
}
