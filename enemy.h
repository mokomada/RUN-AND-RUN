/*******************************************************************************
* �t�@�C���� sceneX.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/04/26
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _ENEMY_H_
#define _ENEMY_H_


/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define ENEMY_NAME "data/MODEL/dogE.x" // �ǂݍ���X�t�@�C����
#define ENEMY_TEXTURE_NAME NULL // �e�N�X�`����

#define ENEMY_MOVE_SPEED (0.5f) // ���f���̈ړ����x
#define ENEMY_ROT_SPEED (0.03f) // ���f���̉�]���x
#define ENEMY_JUMP_SPEED (1.5f) // ���f���̃W�����v��
#define ENEMY_FALL_SPEED (0.1f) // ���f���̗������x

#define ENEMY_SCALE_X (1.0f) // �X�P�[���l X��
#define ENEMY_SCALE_Y (1.0f) // �X�P�[���l Y��
#define ENEMY_SCALE_Z (1.0f) // �X�P�[���l Z��

#define ENEMY_ROT_X (0.0f) // ��]�� X��
#define ENEMY_ROT_Y (0.0f) // ��]�� Y��
#define ENEMY_ROT_Z (0.0f) // ��]�� Z��

#define ENEMY_POS_X (0.0f) // �ʒu��� X��
#define ENEMY_POS_Y (0.0f) // �ʒu��� Y��
#define ENEMY_POS_Z (0.0f) // �ʒu��� Z��

#define ENEMY_MOVE_MAX (3.0f) // �ő�ړ���
#define ENEMY_MOVE_REDUCE (0.0f) // �ړ��ʌ�����
#define ENEMY_FALL_REDUCE (1.0f) // �������x������

#define ENEMY_VIEW_DISTANCE (0.0f) // ���f���̒��S�_���璍���_�܂ł̋���

#define ENEMY_MOVE_CHANGE (30) // �ړ�������ύX����܂ł̎���
#define ENEMY_RESPAWN_TIME (300) // ���X�|�[���܂ł̎���
#define ENEMY_START_DISTANCE (20.0f) // �����ʒu�̃v���C���[�Ƃ̍Œ዗��

#define ENEMY_SEARCH_START (70) // ���G�J�n�����i���̋����ȉ��܂ŋ߂Â��Ɠ����o���j
#define ENEMY_SEARCH_STOP (200) // ���G�I�������i���̋����ȏ�܂ŗ����Ɠ�����̂���߂�j

#define ENEMY_MAX (3) // �G�l�~�[�̍ő吔

/*******************************************************************************
* �\���̒�`
*******************************************************************************/


/*******************************************************************************
* 3D�|���S���N���X
*******************************************************************************/

class CEnemy : public CSceneX{
private:
	int m_moveChange;
	int m_respawn;
	bool m_moveFlag;
	bool m_jumpFlag;
	int m_rnd;
	bool m_use;

public:
	CEnemy(int priority = 1, OBJTYPE objType = OBJTYPE_ENEMY); // �R���X�g���N�^
	~CEnemy( ); // �f�X�g���N�^

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEnemy *Create(void); // �K�v�ɉ����ď����ʒu��p�x���̈�����
	bool ChkEnemy(void) {return m_use;}
};


#endif
/*** �C���N���[�h�K�[�h�I�� ***/