/*******************************************************************************
* �t�@�C���� renderer.h
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/04/13
*******************************************************************************/

/*** �C���N���[�h�K�[�h�J�n ***/
#ifndef _RENDERER_H_
#define _RENDERER_H_


/*******************************************************************************
* �}�N����`
*******************************************************************************/

#define SCENE_NUM (3) // �V�[����


/*******************************************************************************
* �\���̒�`
*******************************************************************************/


/*******************************************************************************
* �v���g�^�C�v�錾
*******************************************************************************/


/*******************************************************************************
* �����_���N���X
*******************************************************************************/

class CRenderer {
private:
	LPDIRECT3D9 m_D3D; // Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	LPDIRECT3DDEVICE9 m_D3DDevice; // Diret3D�f�o�C�X�ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; //���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9 m_BlurTexture1;
	LPDIRECT3DSURFACE9 m_BlurSurface1;
	LPDIRECT3DTEXTURE9 m_BlurTexture2;
	LPDIRECT3DSURFACE9 m_BlurSurface2;
	LPDIRECT3DSURFACE9 m_BackBufferSurface;

public:
//	CRenderer( );
//	~CRenderer( );

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	void Uninit(void);
	void Update(void);
	void StartDraw(void);
	void EndDraw(void);

	LPDIRECT3DDEVICE9 GetDevice(void);

	void InitPolygon(void);
	void UninitPolygon(void);
	void UpdatePolygon(void);
	void DrawPolygon(void);
};


#endif
/*** �C���N���[�h�K�[�h�I�� ***/