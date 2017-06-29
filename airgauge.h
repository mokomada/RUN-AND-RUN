/*******************************************************************************
* �t�@�C���� airgauge.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2017/01/26
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _AIRGAUGE_H_
#define _AIRGAUGE_H_


/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define AIRGAUGE_VERTEX_NUM (4) /* �`�悷�钸�_�̌� */
#define AIRGAUGE_TRIANGLE_NUM (2) /* �`�悷��O�p�`�̌� */

#define AIRGAUGE_X (70) // �|���S���̕`��ʒu�iX���W�j
#define AIRGAUGE_Y (160) // �|���S���̕`��ʒu�iY���W�j
#define AIRGAUGE_WIDTH (30) // �|���S���̕�
#define AIRGAUGE_HEIGHT (100) // �|���S���̍���


/*******************************************************************************
* �\���̒�`
*******************************************************************************/


/*******************************************************************************
* 2D�|���S���N���X
*******************************************************************************/

class CAirgauge : public CScene{
private:
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffAirgauge; // ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_TextureAirgauge; // �e�N�X�`���ւ̃|�C���^

	static bool m_drawGaugeFlag;

public:
	CAirgauge(int priority = 2, OBJTYPE objType = OBJTYPE_NONE); // �R���X�g���N�^
	~CAirgauge( ); // �f�X�g���N�^

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CAirgauge *Create(void); // �K�v�ɉ����ď����ʒu��p�x���̈�����
	static void SetGauge(float height);
	static float GetGauge(void);
};

#endif
/*** �C���N���[�h�K�[�h�I�� ***/