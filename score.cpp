/*******************************************************************************
* �^�C�g�� �X�R�A�`�揈��
* �t�@�C���� score.cpp
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/06/24
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "score.h"
#include "input.h"


/*******************************************************************************
* �X�^�e�B�b�N�ϐ��̐���
*******************************************************************************/

int CScore::m_nScore = 0;


/*******************************************************************************
* �֐����FCScore
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CScore::CScore(int priority, OBJTYPE objType){

}


/*******************************************************************************
* �֐����F~CScore
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CScore::~CScore( ) {

}


/*******************************************************************************
* �֐����FInitScore
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�X�R�A�̏�����
*******************************************************************************/

void CScore::Init(void) {
	/* �ϐ��錾 */
	int nCountVer, nCountBox, nCountDigit, nNumDigit; // for�����[�v�p
	VERTEX_2D *pVtx;

	/* �����o�ϐ��̏����� */
	m_pTextureScore = NULL; // �X�R�A�p�e�N�X�`���ւ̃|�C���^
	m_pVtxBuffScore = NULL; //���_�o�b�t�@�ւ̃|�C���^

	/* �����ݒ� */
	nNumDigit = 1;
	for(nCountDigit = 0; nCountDigit < NUMBER_NUM; nCountDigit++) {
		nNumDigit *= 10;
	}

	/* �ϐ��̎擾 */
	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer = CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾

	/* �X�R�A�e�N�X�`���ݒ� */
	D3DXCreateTextureFromFile(device, SCORE_TEXTURE_NAME, &m_pTextureScore);

	/* ���_�o�b�t�@�̐��� */
	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * SCORE_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffScore, NULL);

	/* ���_���ւ̃|�C���^���擾 */
	m_pVtxBuffScore -> Lock(0, 0, (void**)&pVtx, 0);

	/* ���W�ݒ� */
	for(nCountBox = 0; nCountBox < NUMBER_NUM; nCountBox++) {
		pVtx[(nCountBox * 4) + 0].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) + nCountBox * BOX_WIDTH, 0.0f, 0.0f);
		pVtx[(nCountBox * 4) + 1].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) +  (nCountBox + 1) * BOX_WIDTH, 0.0f, 0.0f);
		pVtx[(nCountBox * 4) + 2].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) + nCountBox * BOX_WIDTH, BOX_HEIGHT, 0.0f);
		pVtx[(nCountBox * 4) + 3].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) +  (nCountBox + 1) * BOX_WIDTH, BOX_HEIGHT, 0.0f);
	}

	for(nCountVer = 0; nCountVer < SCORE_VERTEX_NUM; nCountVer++) {
		pVtx[nCountVer].rhw = 1.0f;

		pVtx[nCountVer].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	/* �e�N�X�`��UV���W�ݒ� */
	for(nCountBox = 0; nCountBox < NUMBER_NUM; nCountBox++) {
		pVtx[(nCountBox * 4) + 0].tex = D3DXVECTOR2( (float) ( ( (m_nScore % nNumDigit) / (nNumDigit / 10) ) * TEX_LENGTH), 0.0f);
		pVtx[(nCountBox * 4) + 1].tex = D3DXVECTOR2( (float) ( ( (m_nScore % nNumDigit) / (nNumDigit / 10) + 1) * TEX_LENGTH), 0.0f);
		pVtx[(nCountBox * 4) + 2].tex = D3DXVECTOR2( (float) ( ( (m_nScore % nNumDigit) / (nNumDigit / 10) ) * TEX_LENGTH), 1.0f);
		pVtx[(nCountBox * 4) + 3].tex = D3DXVECTOR2( (float) ( ( (m_nScore % nNumDigit) / (nNumDigit / 10) + 1) * TEX_LENGTH), 1.0f);
		nNumDigit /= 10;
	}

	m_pVtxBuffScore -> Unlock( );

	m_nScore = 0;
}


/*******************************************************************************
* �֐����FUninitScore
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�X�R�A�̏I������
*******************************************************************************/

void CScore::Uninit(void) {
	if(m_pTextureScore != NULL) {
		m_pTextureScore -> Release( ); // �e�N�X�`���̊J��
		m_pTextureScore = NULL;
	}

	if(m_pVtxBuffScore != NULL) {
		m_pVtxBuffScore -> Release( );
		m_pVtxBuffScore = NULL;
	}
}


/*******************************************************************************
* �֐����FUpdateScore
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�X�R�A�̏��X�V
*******************************************************************************/

void CScore::Update(void) {
	/* �ϐ��錾 */
	int score, nCountBox, nCountDigit, nNumDigit; // for�����[�v�p
	VERTEX_2D *pVtx;

	/* �ړ����X�R�A���� 
	if(GetKeyboardPress(DIK_A) ) {
		if(GetKeyboardPress(DIK_W) ) { // ����ړ�
			m_nScore += 100;
		}
		else if(GetKeyboardPress(DIK_S) ) { // �����ړ�
			m_nScore += 100;
		}
		else { // ���ړ�
			m_nScore += 1;
		}
	}
	else if(GetKeyboardPress(DIK_D) ) {
		if(GetKeyboardPress(DIK_W) ) { // �E��ړ�
			m_nScore += 100;
		}
		else if(GetKeyboardPress(DIK_S) ) { // �E���ړ�
			m_nScore += 100;
		}
		else { // �E�ړ�
			m_nScore += 1;
		}
	}
	else if(GetKeyboardPress(DIK_W) ) { // ��ړ�
			m_nScore += 1;
	}
	else if(GetKeyboardPress(DIK_S) ) { // ���ړ�
			m_nScore += 1;
	}*/

	/* ��]���X�R�A���� 
	if(GetKeyboardRepeat(DIK_LEFT) ) { // ����]
		m_nScore += 10000;
	}

	if(GetKeyboardRepeat(DIK_RIGHT) ) { // �E��]
		m_nScore -= 10000;
	}*/

	/* �g�k���̃X�R�A���� 
	if(GetKeyboardTrigger(DIK_UP) ) { // �g��
		m_nScore += 1000000;
	}
	
	if(GetKeyboardTrigger(DIK_DOWN) ) { // �k��
		m_nScore -= 1000000;
	}*/

	m_nScore++;

	score = m_nScore / 60;

	/* ���/�������� */
	if(m_nScore < SCORE_MIN) {
		m_nScore = SCORE_MIN;
	}
	if(m_nScore > SCORE_MAX) {
		m_nScore = SCORE_MAX;
	}

	/* �����ݒ� */
	nNumDigit = 1;
	for(nCountDigit = 0; nCountDigit < NUMBER_NUM; nCountDigit++) {
		nNumDigit *= 10;
	}

	m_pVtxBuffScore -> Lock(0, 0, (void**)&pVtx, 0);
	/* ���l�̍X�V */
	for(nCountBox = 0; nCountBox < NUMBER_NUM; nCountBox++) {
		pVtx[(nCountBox * 4) + 0].tex = D3DXVECTOR2( (float) ( ( (score % nNumDigit) / (nNumDigit / 10) ) * TEX_LENGTH), 0.0f);
		pVtx[(nCountBox * 4) + 1].tex = D3DXVECTOR2( (float) ( ( (score % nNumDigit) / (nNumDigit / 10) + 1) * TEX_LENGTH), 0.0f);
		pVtx[(nCountBox * 4) + 2].tex = D3DXVECTOR2( (float) ( ( (score % nNumDigit) / (nNumDigit / 10) ) * TEX_LENGTH), 1.0f);
		pVtx[(nCountBox * 4) + 3].tex = D3DXVECTOR2( (float) ( ( (score % nNumDigit) / (nNumDigit / 10) + 1) * TEX_LENGTH), 1.0f);
		nNumDigit /= 10;
	}

	m_pVtxBuffScore -> Unlock( );
}


/*******************************************************************************
* �֐����FDrawScore
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�X�R�A�̕`�揈��
*******************************************************************************/

void CScore::Draw(void) {
	// �ϐ��̎擾
	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer = CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾

	/* ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h */
	device -> SetStreamSource(0, m_pVtxBuffScore, 0, sizeof(VERTEX_2D) );

	/* ���_�t�H�[�}�b�g�̐ݒ� */
	device -> SetFVF(FVF_VERTEX_2D);

	/* �e�N�X�`���̐ݒ� */
	device -> SetTexture(0, m_pTextureScore);

	/* �|���S���̕`�� */
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		SCORE_TRIANGLE_NUM);
}


/*******************************************************************************
* �֐����FAddScore
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�X�R�A�̕`�揈��
*******************************************************************************/

void CScore::AddScore(int score) {
	m_nScore += score;
}


/*******************************************************************************
* �֐����FGetScore
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�X�R�A�̊l��
*******************************************************************************/

int CScore::GetScore(void) {
	return (m_nScore / 60);
}


/*******************************************************************************
* �֐����FCreate
* �����FCScore : �A�h���X
* �߂�l�F�Ȃ�
* �����F�I�u�W�F�N�g�̐���
*******************************************************************************/

CScore *CScore::Create(void) {
	CScore *score;
	score = new CScore;
	score -> Init( );
	return score;
}