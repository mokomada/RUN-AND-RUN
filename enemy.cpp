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
#include "enemy.h"
#include "camera.h"
#include "input.h"
#include "meshfield.h"
#include "debugProc.h"
#include "sound.h"
#include "score.h"
#include "player.h"
#include "game.h"
#include "object.h"


//*****************************************************************************
// �X�^�e�B�b�N�ϐ��̐���
//*****************************************************************************

//MODEL CEnemy::m_model = {D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 0), 0};


/*******************************************************************************
* �֐����FCEnemy
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CEnemy::CEnemy(int priority, OBJTYPE objType):CSceneX(priority, objType) {

}


/*******************************************************************************
* �֐����F~CEnemy
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CEnemy::~CEnemy( ) {

}


/*******************************************************************************
* �֐����FInit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F����������
*******************************************************************************/

void CEnemy::Init(void) {
	CRenderer *renderer; // �f�o�C�X�擾�p�ϐ�
	renderer = CManager::GetRenderer( ); // �����_���̃A�h���X�擾
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( ); // �f�o�C�X�擾
	m_pTexture = NULL;

	D3DXLoadMeshFromX(ENEMY_NAME,
		D3DXMESH_SYSTEMMEM,
		device,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_numMat,
		&m_pMesh);

	// �ϐ��̏�����
	m_model.pos.x = ENEMY_POS_X + rand( ) % 40 - ENEMY_START_DISTANCE;
	if(m_model.pos.x < 0) {
		m_model.pos.x -= ENEMY_START_DISTANCE;
	}
	else{
		m_model.pos.x += ENEMY_START_DISTANCE;
	}

	m_model.pos.y = ENEMY_POS_Y;

	m_model.pos.z = ENEMY_POS_Z + rand( ) % 40 - ENEMY_START_DISTANCE;
	if(m_model.pos.z < 0) {
		m_model.pos.z -= ENEMY_START_DISTANCE;
	}
	else{
		m_model.pos.z += ENEMY_START_DISTANCE;
	}

	D3DXQuaternionIdentity(&m_model.rot);

	m_model.move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_model.scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);

	m_model.jump = false;

	m_newRot = 0;

	m_moveChange = 0;

	m_respawn = 0;

	m_moveFlag = false;

	m_jumpFlag = false;

	m_rnd = 0;

	m_use = true;

	// �e�N�X�`���ݒ�
	D3DXCreateTextureFromFile(device, ENEMY_TEXTURE_NAME, &m_pTexture);
}


/*******************************************************************************
* �֐����FUninit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�I������
*******************************************************************************/

void CEnemy::Uninit(void) {
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

void CEnemy::Update(void) {
	if(m_use) {
		CCamera *get_camera;
		get_camera = CManager::GetCamera( ); // �J�������擾
		CAMERA camera = get_camera -> GetCameraData( );
		MODEL old_model = m_model; // ���ݏ��ێ�
		float field_height; // �n�ʂ̍���
		D3DXVECTOR3 axis = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		D3DXQUATERNION quat;
		float PEDistance;
		OBJHITCHK hitchk;

		CPlayer *get_player = CGame::GetPlayer( );
		MODEL player = get_player -> GetModel( );

		// ���݂̓G���ۑ�
		m_oldModel = m_model;

		// �v���C���[�Ƃ̋���
		PEDistance = sqrt( (m_model.pos.x - player.pos.x) * (m_model.pos.x - player.pos.x) + (m_model.pos.z - player.pos.z) * (m_model.pos.z - player.pos.z) );

		// ��苗���ȉ��Ȃ�ړ��J�n�A��苗���ȏ�ňړ��I��
		if(PEDistance < ENEMY_SEARCH_START && m_moveFlag == false) {
			m_moveFlag = true;
		}
		else if(PEDistance >= ENEMY_SEARCH_STOP && m_moveFlag == true) {
			m_moveFlag = false;
		}

		// �ړ����Ȃ炭�����K�o�K�o�ȗ��������_���ړ����s��
		if(m_moveFlag) {
			m_moveChange--;

			if(m_moveChange <= 0) {
				m_rnd = rand( ) % 8;
				m_moveChange = ENEMY_MOVE_CHANGE;
			}

		// ���f���̈ړ�����
			if(m_rnd < 3) {
				if(m_rnd == 1) { // ����ړ�
				SetNewWay(-0.75f);
				}
				else if(m_rnd == 2) { // �����ړ�
					SetNewWay(-0.25f);
				}
				else { // ���ړ�
					SetNewWay(-0.5f);
				}
			}
			// ��ړ�
			else if(m_rnd == 3) {
				SetNewWay(1.0f);
			}
			else if(m_rnd < 7) {
				if(m_rnd == 6) { // �E��ړ�
					SetNewWay(0.75f);
				}
				else if(m_rnd == 5) { // �E���ړ�
					SetNewWay(0.25f);
				}
				else { // �E�ړ�
					SetNewWay(0.5f);
				}
			}
			// ���ړ�
			else if(m_rnd == 7) {
				SetNewWay(0.0f);
			}
			else if(m_rnd == 8) {
				m_model.move.x = 0;
				m_model.move.z = 0;
			}
			// �v���C���[�̕����Ɉړ����Ă����ꍇ�A����߂Ĉړ��������t�����ɂ���i�����_���l��4�𑫂��ƈړ��������t�ɂȂ�悤�ړ�������ݒ肵�Ă���j
			if(m_moveChange == ENEMY_MOVE_CHANGE && PEDistance > sqrt( ( (m_model.pos.x + m_model.move.x) - player.pos.x) * ( (m_model.pos.x + m_model.move.x) - player.pos.x) + ( (m_model.pos.z + m_model.move.z) - player.pos.z) * ( (m_model.pos.z + m_model.move.z) - player.pos.z) ) ) {
				m_rnd = (m_rnd + 4) % 8;
				m_model.move.x = 0.0f;
				m_model.move.z = 0.0f;
				m_newRot += 1.0f;
			}
		}

		if(m_jumpFlag && m_model.jump == false) {
			m_model.move.y += ENEMY_JUMP_SPEED;
			m_model.jump = true;
			m_jumpFlag = false;
			CSound::PlaySound(SOUND_LABEL_SE_JUMP);
		}


		// �W�����v���ł���Η������x�����Z
		if(m_model.jump) {
			m_model.move.y -= ENEMY_FALL_SPEED;
		}

		// �ړ��ʔ��f
		m_model.pos.x += m_model.move.x;
		m_model.pos.y += m_model.move.y;
		m_model.pos.z += m_model.move.z;

		// �ړ��ʌ���
		m_model.move.x *= ENEMY_MOVE_REDUCE;
		m_model.move.y *= ENEMY_FALL_REDUCE;
		m_model.move.z *= ENEMY_MOVE_REDUCE;

		// ���E�̉ʂĔ��� �������������Ă������
		if(m_model.pos.x < MESHFIELD_FIELD_X + 25.0f) {
			m_model.pos.x = MESHFIELD_FIELD_X + 25.0f;
		}
		else if(m_model.pos.x > MESHFIELD_FIELD_X + MESHFIELD_FIELD_WIDTH - 25.0f) {
			m_model.pos.x = MESHFIELD_FIELD_X + MESHFIELD_FIELD_WIDTH - 25.0f;
		}

		if(m_model.pos.z > MESHFIELD_FIELD_Z - 25.0f) {
			m_model.pos.z = MESHFIELD_FIELD_Z - 25.0f;
		}
		else if(m_model.pos.z < MESHFIELD_FIELD_Z - MESHFIELD_FIELD_HEIGHT + 25.0f) {
			m_model.pos.z = MESHFIELD_FIELD_Z - MESHFIELD_FIELD_HEIGHT + 25.0f;
		}

		// �t�B�[���h�����擾
		field_height = CMeshfield::GetFieldHeight(m_model.pos);

		// ��������
		if(m_model.jump == false && !m_model.onObject) {
			m_model.pos.y = field_height;
		}

		// �t�B�[���h�߂荞�ݏ���
		if(m_model.pos.y < field_height ){
			m_model.pos.y = field_height;
			m_model.move.y = 0.0f;
			m_model.jump = false;
		}

		// �I�u�W�F�N�g�Ƃ̓����蔻��
		hitchk = CScene::HitCheckAll(m_model.pos, m_oldModel.pos);
		if(hitchk.type != 0) {
			if(hitchk.type == 1) {
				m_model.pos.x = m_oldModel.pos.x;
				m_model.pos.z = m_oldModel.pos.z;
				if(!m_model.jump) m_jumpFlag = true;
			}
			else{
				m_model.pos.y = hitchk.nor.y;
				m_model.move.y = 0;
				m_model.jump = false;
				m_model.onObject = true;
			}
		}
		else if(m_model.onObject) {
			m_model.onObject = false;
			m_model.jump = true;
		}

		// m_newRot����
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

		if(m_respawn) {
			m_respawn--;
			if(m_respawn == 0) {
				CSound::PlaySound(SOUND_LABEL_SE_ATTACK);
			}
		}

		// �ڐG����
		if( sqrt( (m_model.pos.x - player.pos.x) * (m_model.pos.x - player.pos.x) + (m_model.pos.z - player.pos.z) * (m_model.pos.z - player.pos.z) ) < 2.0f && m_respawn == 0) {
			CSound::PlaySound(SOUND_LABEL_SE_HIT);
//			CScore::AddScore(10);
			m_respawn += ENEMY_RESPAWN_TIME;
			m_use = false;
		}
	}
}


/*******************************************************************************
* �֐����FSetNewWay
* �����Fset : �ݒ肷��l
* �߂�l�F�Ȃ�
* �����F�ړ��ʂ̐ݒ�
*******************************************************************************/

/*void CEnemy::SetNewWay(float set) {
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

void CEnemy::Draw(void) {
	if(m_use && m_respawn == 0) {
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
		D3DXMatrixScaling(&mtxScl, ENEMY_SCALE_X, ENEMY_SCALE_Y, ENEMY_SCALE_Z);
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
}


/*******************************************************************************
* �֐����FCreate
* �����FsceneX : �A�h���X
* �߂�l�F�Ȃ�
* �����F�I�u�W�F�N�g�̐���
*******************************************************************************/

CEnemy *CEnemy::Create(void) {
	CEnemy *sceneX;
	sceneX = new CEnemy;
	sceneX -> Init( );
	return sceneX;
}
