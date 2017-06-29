/*******************************************************************************
* �t�@�C���� scene.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/04/19
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _SCENE_H_
#define _SCENE_H_


/*******************************************************************************
* �}�N����`
*******************************************************************************/

// 2D�|���S���̒��_�t�H�[�}�b�g
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 3D�|���S���̒��_�t�H�[�}�b�g
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define SCENE_PRIORITY_NUM (3) // �D��x�̐�


/*******************************************************************************
* �\���̒�`
*******************************************************************************/

typedef struct{
	D3DXVECTOR3 pos; // ���_���W
	float rhw; // ���W�����p�W��
	D3DCOLOR col; // ���_�J���[
	D3DXVECTOR2 tex; // �e�N�X�`�����W
} VERTEX_2D;

typedef struct{
	D3DXVECTOR3 pos; // ���_���W
	D3DXVECTOR3 nor; // �@��
	D3DCOLOR col; // ���_�J���[
	D3DXVECTOR2 tex; // �e�N�X�`�����W
} VERTEX_3D;

// �I�u�W�F�N�g�^�C�v
enum OBJTYPE{
	OBJTYPE_NONE,
	OBJTYPE_PLAYER,
	OBJTYPE_ENEMY,
	OBJTYPE_SOLID, // �ђʂ��Ȃ�����
	OBJTYPE_FIELD,
	OBJTYPE_LIQUID // �t�́B�ʂ蔲����
};

// �I�u�W�F�N�g�����蔻��p
typedef struct{
	int type;
	D3DXVECTOR3 nor;
}OBJHITCHK;


/*******************************************************************************
* ���N���X
*******************************************************************************/

class CScene {
protected:
	static CScene *m_top[3];
	static CScene *m_cur[3];
	CScene *m_prev;
	CScene *m_next;
	OBJTYPE m_objType;

	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_rotation;
	D3DXVECTOR3 m_scale;

public:
	CScene(int priority = 1, OBJTYPE objType = OBJTYPE_NONE); // �R���X�g���N�^
	~CScene( ); // �f�X�g���N�^

	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void UpdateAll(void);
	static void DrawAll(void);
	static void UninitAll(void);
	void Release(int priority);

	static OBJHITCHK HitCheckAll(D3DXVECTOR3 pos, D3DXVECTOR3 oldPos);
	static bool ChkEnemyAll(void);

	static CScene *GetList(int priority);
	CScene *GetNext(void);
	OBJTYPE GetObjType(void);

	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void);
	void SetRotation(D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotation(void);
};


#endif
/*** �C���N���[�h�K�[�h�I�� ***/