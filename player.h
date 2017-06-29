/*******************************************************************************
* �t�@�C���� sceneX.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/04/26
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _PLAYER_H_
#define _PLAYER_H_


/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define PLAYER_NAME "data/MODEL/dog.x" // �ǂݍ���X�t�@�C����
#define PLAYER_TEXTURE_NAME NULL // �e�N�X�`����

#define PLAYER_MOVE_SPEED (0.5f) // ���f���̈ړ����x
#define PLAYER_ROT_SPEED (0.01f) // ���f���̉�]���x
#define PLAYER_JUMP_SPEED (1.5f) // ���f���̃W�����v��
#define PLAYER_FALL_SPEED (0.1f) // ���f���̗������x

#define PLAYER_SCALE_X (1.0f) // �X�P�[���l X��
#define PLAYER_SCALE_Y (1.0f) // �X�P�[���l Y��
#define PLAYER_SCALE_Z (1.0f) // �X�P�[���l Z��

#define PLAYER_ROT_X (0.0f) // ��]�� X��
#define PLAYER_ROT_Y (0.0f) // ��]�� Y��
#define PLAYER_ROT_Z (0.0f) // ��]�� Z��

#define PLAYER_POS_X (30.0f) // �ʒu��� X��
#define PLAYER_POS_Y (0.0f) // �ʒu��� Y��
#define PLAYER_POS_Z (30.0f) // �ʒu��� Z��

#define PLAYER_MOVE_MAX (3.0f) // �ő�ړ���
#define PLAYER_MOVE_REDUCE (0.0f) // �ړ��ʌ�����
#define PLAYER_FALL_REDUCE (1.0f) // �������x������

#define PLAYER_VIEW_DISTANCE (0.0f) // ���f���̒��S�_���璍���_�܂ł̋���

/*******************************************************************************
* �\���̒�`
*******************************************************************************/


/*******************************************************************************
* 3D�|���S���N���X
*******************************************************************************/

class CPlayer : public CSceneX{
private:
	bool m_attackFlag;
	int m_attackCooltime;
	D3DXVECTOR3 m_moveReduce;
	float m_airLeft;

public:
	CPlayer(int priority = 1, OBJTYPE objType = OBJTYPE_PLAYER); // �R���X�g���N�^
	~CPlayer( ); // �f�X�g���N�^

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer *Create(void); // �K�v�ɉ����ď����ʒu��p�x���̈�����
};


#endif
/*** �C���N���[�h�K�[�h�I�� ***/