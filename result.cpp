/*******************************************************************************
* �^�C�g�� ���U���g��ʕ`�揈��
* �t�@�C���� result.cpp
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/07/19
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "mode.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "score.h"
#include "scene.h"
#include "game.h"
#include "title.h"


int CResult::m_result_score[RESULT_SCORE_NUM] = {100, 200, 300, 400, 500};

/*******************************************************************************
* �O���[�o���ϐ�
*******************************************************************************/


/*******************************************************************************
* �֐����FInitResult
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F���U���g��ʐݒ�̏�����
*******************************************************************************/

void CResult::Init(void) {
	/* �ϐ��錾 */
	int i, j; // for�����[�v�p
	int nNumDigit; // �X�R�A�p�ϐ�
	int current_score;
	VERTEX_2D *pVtx;

	/* �X�R�A�����ݒ� */
	nNumDigit = 1;
	for(i = 0; i < NUMBER_NUM; i++) {
		nNumDigit *= 10;
	}

	/* �O���[�o���ϐ��̏����� */
	result_wait_time = RESULT_WAIT_TIME;

	/* �ϐ��̎擾 */
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	for(int n = 0; n < RESULT_SCORE_NUM; n++) {
		m_pTextureResultScore[n] = NULL;
		m_pVtxBuffResultScore[n] = NULL;
	}

	current_score = CScore::GetScore( );
	CScore::AddScore(-current_score); // �X�R�A���c���Ă���ƕ�����X�V����Ă��܂��\��������̂ŁA�X�R�A�̒��g��0��

	// �����L���O�X�V
	for(i = 0; i < RESULT_SCORE_NUM; i++) {
		if(current_score < m_result_score[i]) {
			for(j = RESULT_SCORE_NUM - 1; j > i; j--) {
				m_result_score[j] = m_result_score[j - 1];
			}
			m_result_score[i] = current_score;
			break;
		}
	}

	for(i = 0; i < RESULT_SCORE_NUM; i++) {
		// �X�R�A�e�N�X�`���ݒ�
		D3DXCreateTextureFromFile(device, SCORE_TEXTURE_NAME, &m_pTextureResultScore[i]);

		// ���_�o�b�t�@�̐���
		device -> CreateVertexBuffer(sizeof(VERTEX_2D) * SCORE_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffResultScore[i], NULL);

		// �X�R�A�����ݒ�
		nNumDigit = 1;
		for(j = 0; j < NUMBER_NUM; j++) {
			nNumDigit *= 10;
		}

		// ���_���ւ̃|�C���^���擾
		m_pVtxBuffResultScore[i] -> Lock(0, 0, (void**)&pVtx, 0);

		// ���W�ݒ�
		for(j = 0; j < NUMBER_NUM; j++) {
			pVtx[(j * 4) + 0].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) * 0.5f + j * BOX_WIDTH, (float) (BOX_HEIGHT + RESULT_SCORE_SPACE) * (i + 2) - SCREEN_HEIGHT, 0.0f);
			pVtx[(j * 4) + 1].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) * 0.5f + (j + 1) * BOX_WIDTH, (float) (BOX_HEIGHT + RESULT_SCORE_SPACE) * (i + 2) - SCREEN_HEIGHT, 0.0f);
			pVtx[(j * 4) + 2].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) * 0.5f + j * BOX_WIDTH, (float) BOX_HEIGHT + (BOX_HEIGHT + RESULT_SCORE_SPACE) * (i + 2) - SCREEN_HEIGHT, 0.0f);
			pVtx[(j * 4) + 3].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) * 0.5f + (j + 1) * BOX_WIDTH, (float) BOX_HEIGHT + (BOX_HEIGHT + RESULT_SCORE_SPACE) * (i + 2) - SCREEN_HEIGHT, 0.0f);
		}

		for(j = 0; j < SCORE_VERTEX_NUM; j++) {
			pVtx[j].rhw = 1.0f;

			pVtx[j].col = D3DCOLOR_RGBA(255, 255, 128, 255);
		}

		// �e�N�X�`��UV���W�ݒ�
		for(j = 0; j < NUMBER_NUM; j++) {
			pVtx[(j * 4) + 0].tex = D3DXVECTOR2( (float) ( ( (m_result_score[i] % nNumDigit) / (nNumDigit / 10) ) * TEX_LENGTH), 0.0f);
			pVtx[(j * 4) + 1].tex = D3DXVECTOR2( (float) ( ( (m_result_score[i] % nNumDigit) / (nNumDigit / 10) + 1) * TEX_LENGTH), 0.0f);
			pVtx[(j * 4) + 2].tex = D3DXVECTOR2( (float) ( ( (m_result_score[i] % nNumDigit) / (nNumDigit / 10) ) * TEX_LENGTH), 1.0f);
			pVtx[(j * 4) + 3].tex = D3DXVECTOR2( (float) ( ( (m_result_score[i] % nNumDigit) / (nNumDigit / 10) + 1) * TEX_LENGTH), 1.0f);
			nNumDigit /= 10;
		}

		m_pVtxBuffResultScore[i] -> Unlock( );
	}

// ���S�e�N�X�`���ݒ�
	D3DXCreateTextureFromFile(device, RESULT_LOGO_TEXTURE_NAME, &m_pTextureResultLogo);

	// ���_�o�b�t�@�̐���
	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * RESULT_LOGO_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffResultLogo, NULL);

	m_pVtxBuffResultLogo -> Lock(0, 0, (void**)&pVtx, 0);

	// ���W�ݒ�
	pVtx[0].pos = D3DXVECTOR3( (SCREEN_WIDTH - RESULT_LOGO_WIDTH) * 0.5f, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3( (SCREEN_WIDTH - RESULT_LOGO_WIDTH) * 0.5f + RESULT_LOGO_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3( (SCREEN_WIDTH - RESULT_LOGO_WIDTH) * 0.5f, RESULT_LOGO_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3( (SCREEN_WIDTH - RESULT_LOGO_WIDTH) * 0.5f + RESULT_LOGO_WIDTH, RESULT_LOGO_HEIGHT, 0.0f);


	for(i = 0; i < RESULT_VERTEX_NUM; i++) {
		pVtx[i].rhw = 1.0f;

		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
	
	// �e�N�X�`��UV���W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuffResultLogo -> Unlock( );
}


/*******************************************************************************
* �֐����FUninitResult
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F���U���g��ʐݒ�̏I������
*******************************************************************************/

void CResult::Uninit(void) {
	/* �ϐ��̐錾 */
	int i;

	for(i = 0; i < RESULT_SCORE_NUM; i++) {
		if(m_pTextureResultScore[i] != NULL) {
			m_pTextureResultScore[i] -> Release( );
			m_pTextureResultScore[i] = NULL;
		}

		if(m_pVtxBuffResultScore[i] != NULL) {
			m_pVtxBuffResultScore[i] -> Release( );
			m_pVtxBuffResultScore[i] = NULL;
		}
	}
	
	if(m_pTextureResultLogo != NULL) {
		m_pTextureResultLogo -> Release( );
		m_pTextureResultLogo = NULL;
	}

	if(m_pVtxBuffResultLogo != NULL) {
		m_pVtxBuffResultLogo -> Release( );
		m_pVtxBuffResultLogo = NULL;
	}
}


/*******************************************************************************
* �֐����FUpdataResult
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F���U���g��ʂ̏��X�V
*******************************************************************************/

void CResult::Update(void) {
	/* �ϐ��̐錾 */
	int i, j, k;
	VERTEX_2D *pVtx;

	/* �ϐ��̎擾 */
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	CFade *getFade;
	getFade = CManager::GetFade( );
	FADE fade = getFade -> GetFade( );

	CInput *input;
	input = CManager::GetInput( );

	if(fade == FADE_NONE) {
		if(input -> GetKeyboardTrigger(DIK_RETURN) || input -> GetKeyboardTrigger(DIK_SPACE) || result_wait_time == 0) {
			if(result_wait_time > RESULT_STOP_MOVE_TIME) {
				while(result_wait_time > RESULT_STOP_MOVE_TIME) {
					for(i = 0; i < RESULT_SCORE_NUM; i++) {
						// ���_���ւ̃|�C���^���擾
						m_pVtxBuffResultScore[i] -> Lock(0, 0, (void**)&pVtx, 0);
						// ���W�ݒ�
						for(j = 0; j < NUMBER_NUM; j++) {
							for(k = 0; k < 4; k++) {
							pVtx[(j * 4) + k].pos.y += SCREEN_HEIGHT / (RESULT_WAIT_TIME - RESULT_STOP_MOVE_TIME);
							}
						}
						m_pVtxBuffResultScore[i] -> Unlock( );
					}
					result_wait_time--;
				}
			}
			else {
				CFade::SetFade(FADE_OUT, new CTitle);
			}
		}

		if(result_wait_time > RESULT_STOP_MOVE_TIME) {
			for(i = 0; i < RESULT_SCORE_NUM; i++) {
				// ���_���ւ̃|�C���^���擾
				m_pVtxBuffResultScore[i] -> Lock(0, 0, (void**)&pVtx, 0);
				// ���W�ݒ�
				for(j = 0; j < NUMBER_NUM; j++) {
					for(k = 0; k < 4; k++) {
					pVtx[(j * 4) + k].pos.y += SCREEN_HEIGHT / (RESULT_WAIT_TIME - RESULT_STOP_MOVE_TIME);
					}
				}
				m_pVtxBuffResultScore[i] -> Unlock( );
			}
		}
		result_wait_time--;
	}
}


/*******************************************************************************
* �֐����FDrawResult
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F���U���g��ʂ̕`�揈��
*******************************************************************************/

void CResult::Draw(void) {
	/* �ϐ��̐錾 */
	int i;
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	// ���S�̕`��
	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	device -> SetStreamSource(0, m_pVtxBuffResultLogo, 0, sizeof(VERTEX_2D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	device -> SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	device -> SetTexture(0, m_pTextureResultLogo);

	// ���C�g�ݒ�I�t
	device -> SetRenderState(D3DRS_LIGHTING, FALSE);

	// 2D�|���S���̕`��
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		TITLE_TRIANGLE_NUM);

	// ���C�g�ݒ�I��
	device -> SetRenderState(D3DRS_LIGHTING, TRUE);

	for(i = 0; i < RESULT_SCORE_NUM; i++) {
		device -> SetStreamSource(0, m_pVtxBuffResultScore[i], 0, sizeof(VERTEX_2D) );

		device -> SetTexture(0, m_pTextureResultScore[i]);

		device -> DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			SCORE_TRIANGLE_NUM);
	}
}
