/*******************************************************************************
* �^�C�g�� GM31 X�t�@�C���`��
* �t�@�C���� sceneX.cpp
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/04/26
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "sceneX.h"
#include "player.h"
#include "camera.h"
#include "input.h"
#include "meshfield.h"
#include "meshwater.h"
#include "debugProc.h"
#include "sound.h"
#include "score.h"
#include "enemy.h"
#include "game.h"

#include "object.h"
#include "airgauge.h"


//*****************************************************************************
// �X�^�e�B�b�N�ϐ��̐���
//*****************************************************************************

//MODEL CPlayer::m_model = {D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 0};


/*******************************************************************************
* �֐����FCPlayer
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CPlayer::CPlayer(int priority, OBJTYPE objType):CSceneX(priority, objType) {

}


/*******************************************************************************
* �֐����F~CPlayer
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CPlayer::~CPlayer( ) {

}


/*******************************************************************************
* �֐����FInit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F����������
*******************************************************************************/

void CPlayer::Init(void) {
	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer = CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾
	m_pTexture = NULL;
	float field_height;

	D3DXLoadMeshFromX(PLAYER_NAME,
		D3DXMESH_SYSTEMMEM,
		device,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_numMat,
		&m_pMesh);

	// �ϐ��̏�����
	m_model.pos.x = PLAYER_POS_X;
	m_model.pos.y = PLAYER_POS_Y;
	m_model.pos.z = PLAYER_POS_Z;

	field_height = CMeshfield::GetFieldHeight(m_model.pos);
	m_model.pos.y = field_height;

	D3DXQuaternionIdentity(&m_model.rot);

	m_model.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_model.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_model.jump = false;

	m_model.onObject = false;

	m_attackFlag = false;

	m_attackCooltime = 0;

	m_newRot = 0;

	m_airLeft = 100.0f;

	m_moveReduce = D3DXVECTOR3(PLAYER_MOVE_REDUCE, PLAYER_FALL_REDUCE, PLAYER_MOVE_REDUCE);

	// �e�N�X�`���ݒ�
	D3DXCreateTextureFromFile(device, PLAYER_TEXTURE_NAME, &m_pTexture);
}


/*******************************************************************************
* �֐����FUninit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�I������
*******************************************************************************/

void CPlayer::Uninit(void) {
	if(m_pTexture != NULL) {
		m_pTexture -> Release( ); // �e�N�X�`���̊J��
		m_pTexture = NULL;
	}

	if(m_pMesh != NULL) {
		m_pMesh -> Release( );
		m_pMesh = NULL;
	}

	if(m_pBuffMat != NULL) {
		m_pBuffMat -> Release( );
		m_pBuffMat = NULL;
	}
}


/*******************************************************************************
* �֐����FUpdate
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�X�V����
*******************************************************************************/

void CPlayer::Update(void) {
	CAirgauge *get_airgauge;
	get_airgauge = CGame::GetAirgauge( );
	MODEL old_model = m_model; // ���ݏ��ێ�
	float field_height, water_height; // �n��/���ʂ̍���
	D3DXVECTOR3 axis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	D3DXQUATERNION quat;
	OBJHITCHK hitchk;
	bool drown_flag = false;

	// �G���擾
//	CEnemy *get_enemy = CGame::GetEnemy( );
//	MODEL enemy = get_enemy -> GetEnemy( );

	// ���݂̃v���C���[���ۑ�
	m_oldModel = m_model;

	// ���͏��擾
	CInput *input;
	input = CManager::GetInput( );

	// ���f���̈ړ�����
	if(!m_attackFlag) {
		if(input -> GetKeyboardPress(DIK_A) ) {
			if(input -> GetKeyboardPress(DIK_W) ) { // ����ړ�
				SetNewWay(-0.75f);
			}
			else if(input -> GetKeyboardPress(DIK_S) ) { // �����ړ�
				SetNewWay(-0.25f);
			}
			else { // ���ړ�
				SetNewWay(-0.5f);
			}
		}
		else if(input -> GetKeyboardPress(DIK_D) ) {
			if(input -> GetKeyboardPress(DIK_W) ) { // �E��ړ�
				SetNewWay(0.75f);
			}
			else if(input -> GetKeyboardPress(DIK_S) ) { // �E���ړ�
				SetNewWay(0.25f);
			}
			else { // �E�ړ�
				SetNewWay(0.5f);
			}
		} // �O�ړ�
		else if(input -> GetKeyboardPress(DIK_W) ) {
			SetNewWay(1.0f);
		} // ��ړ�
		else if(input -> GetKeyboardPress(DIK_S) ) {
			SetNewWay(0.0f);
		}

		// ���f���̉�]����
		if(input -> GetKeyboardPress(DIK_RSHIFT) ) {
			m_newRot += PLAYER_ROT_SPEED;
		}
		else if(input -> GetKeyboardPress(DIK_LSHIFT) ) {
			m_newRot -= PLAYER_ROT_SPEED;
		}

		// �W�����v����
		if(input -> GetKeyboardPress(DIK_J) && !m_model.jump) {
			m_model.move.y += PLAYER_JUMP_SPEED;
			m_model.jump = true;
			CSound::PlaySound(SOUND_LABEL_SE_JUMP);
		}

		// �ːi����
		if(input -> GetKeyboardTrigger(DIK_SPACE) && !m_model.jump && !m_attackCooltime) {
			m_attackFlag = true;
			m_attackCooltime = 60;
			m_model.move.x += sinf(D3DX_PI * (-m_newRot) ) * PLAYER_MOVE_SPEED * 3;
			m_model.move.z -= cosf(D3DX_PI * (-m_newRot) ) * PLAYER_MOVE_SPEED * 3;
			m_moveReduce = D3DXVECTOR3(0.95f, 0.0f, 0.95f);
			CSound::PlaySound(SOUND_LABEL_SE_ATTACK);
		}
	}

	// �W�����v���ł���Η������x�����Z
	if(m_model.jump) {
		m_model.move.y -= PLAYER_FALL_SPEED;
	}

	// �ړ��ʔ��f
	m_model.pos.x += m_model.move.x;
	m_model.pos.y += m_model.move.y;
	m_model.pos.z += m_model.move.z;

	// �U�����[�V�����I������
	if(m_attackFlag && D3DXVec3Length(&m_model.move) < 1.0f) {
		m_attackFlag = false;
		m_model.jump = true;
		m_moveReduce = D3DXVECTOR3(PLAYER_MOVE_REDUCE, PLAYER_FALL_REDUCE, PLAYER_MOVE_REDUCE);
	}

	// �U���N�[���^�C������
	if(m_attackCooltime > 0) m_attackCooltime--;

	// ���E�̉ʂĔ��� �������������Ă������
	if(m_model.pos.x < MESHFIELD_FIELD_X + 1.0f) {
		m_model.pos.x = MESHFIELD_FIELD_X + 1.0f;
	}
	else if(m_model.pos.x > MESHFIELD_FIELD_X + MESHFIELD_FIELD_WIDTH - 1.0f) {
		m_model.pos.x = MESHFIELD_FIELD_X + MESHFIELD_FIELD_WIDTH - 1.0f;
	}

	if(m_model.pos.z > MESHFIELD_FIELD_Z - 1.0f) {
		m_model.pos.z = MESHFIELD_FIELD_Z - 1.0f;
	}
	else if(m_model.pos.z < MESHFIELD_FIELD_Z - MESHFIELD_FIELD_HEIGHT + 1.0f) {
		m_model.pos.z = MESHFIELD_FIELD_Z - MESHFIELD_FIELD_HEIGHT + 1.0f;
	}

	// �t�B�[���h�����擾
	field_height = CMeshfield::GetFieldHeight(m_model.pos);

	// ��������
	if(!m_model.jump && !m_model.onObject && !m_attackFlag) {
		m_model.pos.y = field_height;
	}

	// �t�B�[���h�߂荞�ݏ���
	if(m_model.pos.y < field_height) {
		m_model.pos.y = field_height;
		m_model.move.y = 0.0f;
		m_model.jump = false;
	}

	// ���ʂ̍����擾
	water_height = CMeshwater::GetWaterHeight(m_model.pos);

	// ��������
	if(m_model.pos.y + 5 < water_height) {
		m_airLeft -= 0.3f;
		if(m_airLeft < 0) {
			m_airLeft = 0;
			CScore::AddScore(600000);
			CGame::EndGame( );
		}

	}
	else if(m_airLeft < 100) {
		m_airLeft += 0.8f;
		if(m_airLeft >= 100) {
			m_airLeft = 100;
		}
	}

	CAirgauge::SetGauge(m_airLeft);

	// �I�u�W�F�N�g�Ƃ̓����蔻��
	hitchk = CScene::HitCheckAll(m_model.pos, m_oldModel.pos);
	if(hitchk.type != 0) {
		switch(hitchk.type) {
		case 1:
//			m_model.pos.z = m_oldModel.pos.z;
//			check = D3DXVECTOR3(0.0f, 0.0f, 1.0f) * D3DXVec3Dot(&(-m_model.move), &D3DXVECTOR3(0.0f, 0.0f, 1.0f) ); �@���󂯎��΂��̌v�Z�ł�����͂��H
//			m_model.pos += D3DXVECTOR3(0.0f, 0.0f, 1.0f) * D3DXVec2Dot(&D3DXVECTOR2(-m_model.move.x, -m_model.move.z), &D3DXVECTOR2(0.0f, 1.0f) );
			m_model.pos += hitchk.nor * D3DXVec3Dot(&(-m_model.move), &hitchk.nor);
			break;
		case 2:
//			m_model.pos.x = m_oldModel.pos.x;
//			m_model.pos += D3DXVECTOR3(-1.0f, 0.0f, 0.0f) * D3DXVec2Dot(&D3DXVECTOR2(-m_model.move.x, -m_model.move.z), &D3DXVECTOR2(-1.0f, 0.0f) );
			m_model.pos.y = hitchk.nor.y;
			m_model.move.y = 0;
			m_model.jump = false;
			m_model.onObject = true;
			break;
/*		case 3:
//			m_model.pos.x = m_oldModel.pos.x;
			m_model.pos += D3DXVECTOR3(1.0f, 0.0f, 0.0f) * D3DXVec2Dot(&D3DXVECTOR2(-m_model.move.x, -m_model.move.z), &D3DXVECTOR2(1.0f, 0.0f) );
			break;
		case 4:
//			m_model.pos.z = m_oldModel.pos.z;
			m_model.pos += D3DXVECTOR3(0.0f, 0.0f, -1.0f) * D3DXVec2Dot(&D3DXVECTOR2(-m_model.move.x, -m_model.move.z), &D3DXVECTOR2(0.0f, -1.0f) );
			break;*/
		default:
			break;
		}
	}
	else if(m_model.onObject) {
		m_model.onObject = false;
		m_model.jump = true;
	}

	// �ړ��ʌ���
	m_model.move.x *= m_moveReduce.x;
	m_model.move.y *= m_moveReduce.y;
	m_model.move.z *= m_moveReduce.z;

	// newRot����
	if(m_newRot > 1.0f) {
		m_newRot -= 2.0f;
	}
	else if(m_newRot < -1.0f) {
		m_newRot += 2.0f;
	}

	// ���f���̕����␳
	if(abs(m_newRot - m_model.rot.y) <= 1.0f) {
		m_model.rot.y += (m_newRot - m_model.rot.y) * 0.1f;
	}
	if(abs(m_newRot - m_model.rot.y) > 1.0f) {
		m_model.rot.y += (fmod(m_newRot + 2.0f, 2.0f) - fmod(m_model.rot.y + 2.0f, 2.0f) ) * 0.1f;
	}

/*	D3DXQuaternionRotationAxis(&quat, &axis, 0.0f);
	D3DXQuaternionMultiply(&m_model.rot, &m_model.rot, &quat);*/

	// ��]�p�̗�O����
	if(m_model.rot.y > 1.0f) {
		m_model.rot.y -= 2.0f;
	}
	else if(m_model.rot.y < -1.0f) {
		m_model.rot.y += 2.0f;
	}

	PrintDebugProc("����(%f)\n", m_model.pos.y);
}

/*******************************************************************************
* �֐����FSetNewWay
* �����Fset : �ݒ肷��l
* �߂�l�F�Ȃ�
* �����F�ړ��ʂ̐ݒ�
*******************************************************************************/

/*void CPlayer::SetNewWay(float set) {
	// �J�������擾
	CCamera *get_camera;
	get_camera = CManager::GetCamera( );
	CAMERA camera = get_camera -> GetCameraData( );

	// �i�s�����̐ݒ�
	m_newRot = -camera.rot.y - set;

	// ���፷�ɂ��ړ��ʌ����v�Z
	// ���݈ʒu�̃t�B�[���h�̖@�����擾
	D3DXVECTOR3 field_norm = CMeshfield::GetFieldNorm(m_model.pos);
	// �����Ȗ@���ƃt�B�[���h�̖@���������p�x�Ƃɑ΂��Acos�Ƃ����߂�B�ǂ�������K������Ă���̂� ����/(����A * ����B) �̊��Z�ȉ��͏ȗ�
	float norm_cosangle = D3DXVec3Dot(&field_norm, &D3DXVECTOR3(0.0f, 1.0f, 0.0f) );

	PrintDebugProc("cos�ƌv�Z�O(%f)\n", norm_cosangle);


	// �t�B�[���h�̌X���Ɛi�s��������A�����ʂ̊��������߂�
	D3DXVECTOR3 field_xynorm;
	// �t�B�[���h�@����XZ�������������o�����K��
	D3DXVec3Normalize(&field_xynorm, &D3DXVECTOR3(field_norm.x, 0.0f, field_norm.z) );

	// �ړ��ʂ��v�Z���Đ��K��
	D3DXVECTOR3 player_move;
	D3DXVec3Normalize(&player_move, &D3DXVECTOR3(-sinf(D3DX_PI * (camera.rot.y + set) ), 0.0f, cosf(D3DX_PI * (camera.rot.y + set) ) ) );
	
	PrintDebugProc("�v���C���[�ړ���(%f, %f, %f)\n", player_move.x, player_move.y, player_move.z);

	// �v���C���[�̈ړ��ʂƃt�B�[���h�@����xy�����̓��ρB���ɐ��K������Ă���̂ŏ�L���l�㔼�ȗ�
	float xynorm_cosangle = D3DXVec3Dot(&field_xynorm, &player_move);

	// ���߂����������Ō����ʂ𒲐�
	norm_cosangle = 1 - ( (1 - norm_cosangle) * abs(xynorm_cosangle) );

	PrintDebugProc("cos�ƌv�Z��(%f)\n", norm_cosangle);

	// �ړ��ʉ��Z
	m_model.move.x += sinf(D3DX_PI * (camera.rot.y + set) ) * PLAYER_MOVE_SPEED * norm_cosangle;
	m_model.move.z -= cosf(D3DX_PI * (camera.rot.y + set) ) * PLAYER_MOVE_SPEED * norm_cosangle;
}*/


/*******************************************************************************
* �֐����FDraw
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�`�揈��
*******************************************************************************/

void CPlayer::Draw(void) {
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾
	D3DXMATRIX mtxScl, mtxRot, mtxTrans; // �v�Z�p
	D3DXMATERIAL *pMat;
	D3DMATERIAL9 matDef; // �}�e���A�����ۑ��p

	// ���[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&m_mtxWorld);
	// ���[���h�}�g���b�N�X�̍쐬
	// �X�P�[���𔽉f
	D3DXMatrixScaling(&mtxScl, PLAYER_SCALE_X, PLAYER_SCALE_Y, PLAYER_SCALE_Z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�𔽉f
//	D3DXMatrixRotationQuaternion(&mtxRot, &m_model.rot); // ������ y, x, z �̏��Ԃɒ��ӁI�I�I ������
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_model.rot.y * D3DX_PI, m_model.rot.x * D3DX_PI, m_model.rot.z * D3DX_PI);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// �ړ��𔽉f
	D3DXMatrixTranslation(&mtxTrans, m_model.pos.x, m_model.pos.y, m_model.pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	device -> SetTransform(D3DTS_WORLD, &m_mtxWorld);

	// ���݃}�e���A�����擾
	device -> GetMaterial(&matDef);

	// �}�e���A���ݒ�
	pMat = (D3DXMATERIAL*)m_pBuffMat -> GetBufferPointer( );
	for(int nCntMat = 0; nCntMat < (int)m_numMat; nCntMat++) {
		device -> SetMaterial(&pMat[nCntMat].MatD3D);
		device -> SetTexture(0, m_pTexture);
		m_pMesh -> DrawSubset(nCntMat);
	}

	// �}�e���A���̐ݒ��߂�
	device -> SetMaterial(&matDef);
}


/*******************************************************************************
* �֐����FCreate
* �����FsceneX : �A�h���X
* �߂�l�F�Ȃ�
* �����F�I�u�W�F�N�g�̐���
*******************************************************************************/

CPlayer *CPlayer::Create(void) {
	CPlayer *sceneX;
	sceneX = new CPlayer;
	sceneX -> Init( );
	return sceneX;
}
