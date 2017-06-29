/*******************************************************************************
* �t�@�C���� shadow.h
* �쐬�� AT-12C-245 �H�ԗY��
* �쐬�� 2015/11/12
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _SHADOW_H_
#define _SHADOW_H_

/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define SHADOW_VERTEX_NUM (4) // �`�悷�钸�_�̌�
#define SHADOW_TRIANGLE_NUM (2) // �`�悷��O�p�`�̌�

#define SHADOW_TEXTURE_NAME "data/TEXTURE/shadow000.jpg" // �e�N�X�`���[�Ɏg���摜�w��

#define SHADOW_DEFAULT_COLOR (128) // �e�̔Z���̃f�t�H���g�l

#define SHADOW_MAX (100) // ��x�ɕ\���ł���e�̍ő吔

/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/

void InitShadow(void);
void UninitShadow(void);
void UpdateShadow(void);
void DrawShadow(void);
int SetShadow(D3DXVECTOR3 *new_pos, float width, float height);
void DeleteShadow(int num);

#endif
/*** �C���N���[�h�K�[�h�I�� ***/