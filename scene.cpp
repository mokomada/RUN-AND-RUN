/*******************************************************************************
* �^�C�g�� GM31 ���N���X
* �t�@�C���� scene.cpp
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/04/19
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "sceneX.h"
#include "object.h"
#include "enemy.h"

/*******************************************************************************
* static�����o�ϐ��̏�����
*******************************************************************************/

CScene *CScene::m_top[3] = {NULL};
CScene *CScene::m_cur[3] = {NULL};


/*******************************************************************************
* �֐����FCScene
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X����������
*******************************************************************************/

CScene::CScene(int priority, OBJTYPE objType) {
	if(m_top[priority] == NULL) {
		m_top[priority] = this;
		m_cur[priority] = this;
		m_prev = NULL;
		m_next = NULL;
	}
	else {
		m_cur[priority] -> m_next = this;
		m_prev = m_cur[priority];
		m_cur[priority] = this;
		m_next = NULL;
	}

	m_objType = objType;
}


/*******************************************************************************
* �֐����F~CScene
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�N���X�j��������
*******************************************************************************/

CScene::~CScene( ) {

}

/*******************************************************************************
* �֐����FSetPosition
* �����Fpos : ���W���
* �߂�l�F�Ȃ�
* �����F���W���Z�b�g����
*******************************************************************************/

void CScene::SetPosition(D3DXVECTOR3 pos) {
	m_position = pos;
}


/*******************************************************************************
* �֐����FGetPosition
* �����F�Ȃ�
* �߂�l�Fm_position : ���W���
* �����F���W����Ԃ�
*******************************************************************************/

D3DXVECTOR3 CScene::GetPosition(void) {
	return m_position;
}


/*******************************************************************************
* �֐����FSetRotation
* �����Frot : ��]���
* �߂�l�F�Ȃ�
* �����F��]�p���Z�b�g����
*******************************************************************************/

void CScene::SetRotation(D3DXVECTOR3 rot) {
	m_rotation = rot;
}


/*******************************************************************************
* �֐����FGetRotation
* �����F�Ȃ�
* �߂�l�Fm_rotation : ��]���
* �����F��]����Ԃ�
*******************************************************************************/

D3DXVECTOR3 CScene::GetRotation(void) {
	return m_rotation;
}


/*******************************************************************************
* �֐����FUpdateAll
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F���X�g�̑S�Ă�Update�����s����
*******************************************************************************/

void CScene::UpdateAll(void) {
	for(int i = 0; i < SCENE_PRIORITY_NUM; i++) {
		CScene *scene = m_top[i];
		CScene *next;

		while(scene != NULL) {
			next = scene -> m_next;
			scene -> Update( );
			scene = next;
		}
	}
}


/*******************************************************************************
* �֐����FDrawAll
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F���X�g�̑S�Ă�Draw�����s����
*******************************************************************************/

void CScene::DrawAll(void) {
	CScene *keepScene[64];
	int cnt = 0;
	for(int i = 0; i < SCENE_PRIORITY_NUM; i++) {
		CScene *scene = m_top[i];

		while(scene != NULL) {
			if(scene -> m_objType == OBJTYPE_SOLID && ( (CObject*)(scene) )->GetAlphaFlag( ) ) {
				keepScene[cnt] = scene;
				cnt++;
			}
			else {
				scene -> Draw( );
			}
			scene = scene -> m_next;
		}
	}

	for(int i = 0; i < cnt; i++) {
		keepScene[i] -> Draw( );
	}
}


/*******************************************************************************
* �֐����FUninitAll
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F���X�g�̑S�Ă�Uninit�����s����
*******************************************************************************/

void CScene::UninitAll(void) {
	CScene *next;
	for(int i = 0; i < SCENE_PRIORITY_NUM; i++) {
		CScene *scene = m_top[i];

		while(scene != NULL) {
			next = scene -> m_next;
			scene -> Uninit( );
			delete scene;
			scene = next;
		}
		m_top[i] = NULL;
		m_cur[i] = NULL;
	}
}


/*******************************************************************************
* �֐����FRelease
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�I�u�W�F�N�g�����X�g����폜����
*******************************************************************************/

void CScene::Release(int priority) {
	if(m_prev != NULL) {
		m_prev -> m_next = m_next;
	}
	else {
		m_top[priority] = m_next;
	}

	if(m_next != NULL) {
		m_next -> m_prev = m_prev;
	}
	else {
		m_cur[priority] = m_prev;
	}

	delete this;
}


/*******************************************************************************
* �֐����FGetList
* �����Fpriority : ���肵�������X�g�̗D��l
* �߂�l�Fm_top[priority] : �Y���̃��X�g�̐擪�A�h���X
* �����F�w�肳�ꂽ�D��l�̃��X�g�̐擪�A�h���X��Ԃ�
*******************************************************************************/

CScene *CScene::GetList(int priority) {
	return m_top[priority];
}


/*******************************************************************************
* �֐����FGetNext
* �����F�Ȃ�
* �߂�l�Fm_next : ���̃��X�g�̏ꏊ
* �����F���X�g�̎��̏ꏊ�ւ̃A�h���X���擾����
*******************************************************************************/

CScene *CScene::GetNext(void) {
	return m_next;
}


/*******************************************************************************
* �֐����FGetObjType
* �����F�Ȃ�
* �߂�l�Fm_objType : �I�u�W�F�N�g�^�C�v
* �����F�����̃I�u�W�F�N�g�^�C�v��Ԃ�
*******************************************************************************/

OBJTYPE CScene::GetObjType(void) {
	return m_objType;
}


/*******************************************************************************
* �֐����FHitCheckAll
* �����Fpos : �����蔻����s���Ώۂ̌��ݒn
* �߂�l�Fint : �����蔻��̌���
* �����F�e�I�u�W�F�N�g�Ƃ̓����蔻����s���A���̌��ʂ�Ԃ�
*******************************************************************************/

OBJHITCHK CScene::HitCheckAll(D3DXVECTOR3 pos, D3DXVECTOR3 oldPos) {
	OBJHITCHK chk;
	CObject *object;
	for(int i = 0; i < SCENE_PRIORITY_NUM; i++) {
		CScene *scene = m_top[i];

		while(scene != NULL) {
			if(scene -> m_objType == OBJTYPE_SOLID) {
				object = (CObject*)(scene);
				chk = object -> HitCheck(pos, oldPos);
				if(chk.type != 0) {
					if( (int)oldPos.z == -12345) object -> SetAlphaFlag(true);
					return chk;
				}
			}
			scene = scene -> m_next;
		}
	}
	return chk;
}


/*******************************************************************************
* �֐����FChkEnemyAll
* �����F�Ȃ�
* �߂�l�Fint : �����蔻��̌���
* �����F�e�I�u�W�F�N�g�Ƃ̓����蔻����s���A���̌��ʂ�Ԃ�
*******************************************************************************/

bool CScene::ChkEnemyAll(void) {
	bool chk;
	CEnemy *enemy;

	for(int i = 0; i < ENEMY_MAX; i++) {
		CScene *scene = m_top[i];

		while(scene != NULL) {
			if(scene -> m_objType == OBJTYPE_ENEMY) {
				enemy = (CEnemy*)(scene);
				chk = enemy ->ChkEnemy( );
				if(chk) return true;
			}
			scene = scene -> m_next;
		}
	}
	return false;
}