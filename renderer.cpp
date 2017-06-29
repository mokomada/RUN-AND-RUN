/*******************************************************************************
* �^�C�g�� GM31 2D�|���S���`��
* �t�@�C���� renderer.cpp
* �쐬�� AT-13A-284 �H�ԗY��
* �쐬�� 2016/04/13
*******************************************************************************/

/*******************************************************************************
* �C���N���[�h�t�@�C��
*******************************************************************************/

#include "main.h"
#include "renderer.h"
#include "scene.h"
#include "scene2D.h"
#include "scene3D.h"
#include "sceneX.h"
#include "camera.h"
#include "light.h"
#include "input.h"


/*******************************************************************************
* �֐����FInit
* �����FhInstance, hWnd, bWindow
* �߂�l�F�Ȃ�
* �����F����������
*******************************************************************************/

HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow) {
	// �����o�ϐ��̏�����
	m_D3D = NULL; // Direct3D�I�u�W�F�N�g�ւ̃|�C���^
	m_D3DDevice = NULL; // Diret3D�f�o�C�X�ւ̃|�C���^
	m_BlurTexture1 = m_BlurTexture2 = NULL;
	m_BlurSurface1 = m_BlurSurface2 = NULL;
	m_BackBufferSurface = NULL;

	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	m_D3D = Direct3DCreate9(D3D_SDK_VERSION);

	if(m_D3D == NULL) {
		return E_FAIL;
	}

	if(FAILED(m_D3D -> GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm) ) ) {
		return E_FAIL;
	}

	ZeroMemory(&d3dpp, sizeof(d3dpp) );
	
	d3dpp.BackBufferWidth = SCREEN_WIDTH;
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	if(FAILED(m_D3D -> CreateDevice(
		D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL,
		hWnd,
		D3DCREATE_HARDWARE_VERTEXPROCESSING,
		&d3dpp,
		&m_D3DDevice) ) )
	{
		if(FAILED(m_D3D -> CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&m_D3DDevice) ) )
		{
			if(FAILED(m_D3D -> CreateDevice(
				D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&m_D3DDevice) ) )
			{
				return E_FAIL;
			}
		}
	}

	m_D3DDevice -> SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_D3DDevice -> SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_D3DDevice -> SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_D3DDevice -> SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	// �T���v���[�X�e�[�g�̐ݒ�
	m_D3DDevice -> SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); //�e�N�X�`���k�����̕⊮�ݒ�
	m_D3DDevice -> SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); //�e�N�X�`���g�厞�̕⊮�ݒ�

	// �t�B�[�h�o�b�N�u���[�ݒ�
	D3DXCreateTexture(m_D3DDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_BlurTexture1);
	m_BlurTexture1 -> GetSurfaceLevel(0, &m_BlurSurface1);

	D3DXCreateTexture(m_D3DDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_BlurTexture2);
	m_BlurTexture2 -> GetSurfaceLevel(0, &m_BlurSurface2);

	m_D3DDevice -> GetRenderTarget(0, &m_BackBufferSurface);

	// �S���2D�|���S��
	VERTEX_2D *pVtx2D;
	// ���_�o�b�t�@�̐��� 
	m_D3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// ���_���ւ̃|�C���^���擾
	m_pVtxBuff -> Lock(0, 0, (void**)&pVtx2D, 0);

	// ���W�ݒ�
	pVtx2D[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx2D[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx2D[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx2D[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	for(int i = 0; i < 4; i++) {
		pVtx2D[i].rhw = 1.0f;

		pVtx2D[i].col = D3DCOLOR_RGBA(255, 255, 255, 250);
	}

	// �e�N�X�`��UV���W�ݒ�
	pVtx2D[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2D[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2D[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx2D[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff -> Unlock( );

	return S_OK;
}


/*******************************************************************************
* �֐����FUninit
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�I������
*******************************************************************************/

void CRenderer::Uninit(void) {
	if(m_D3DDevice != NULL) {
		m_D3DDevice -> Release( ); //�f�o�C�X�̊J��
		m_D3DDevice = NULL;
	}

	if(m_D3D != NULL) {
		m_D3D -> Release( ); //�I�u�W�F�N�g�̊J��
		m_D3D = NULL;
	}

	if(m_BlurTexture1 != NULL) {
		m_BlurTexture1 -> Release( );
		m_BlurTexture1 = NULL;
	}

	if(m_BlurSurface1 != NULL) {
		m_BlurSurface1 -> Release( );
		m_BlurSurface1 = NULL;
	}

	if(m_BlurTexture2 != NULL) {
		m_BlurTexture2 -> Release( );
		m_BlurTexture2 = NULL;
	}

	if(m_BlurSurface2 != NULL) {
		m_BlurSurface2 -> Release( );
		m_BlurSurface2 = NULL;
	}

	if(m_BackBufferSurface != NULL) {
		m_BackBufferSurface -> Release( );
		m_BackBufferSurface = NULL;
	}
}


/*******************************************************************************
* �֐����FUpdate
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�X�V����
*******************************************************************************/

void CRenderer::Update(void) {

}


/*******************************************************************************
* �֐����FStartDraw
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�`�揈��
*******************************************************************************/

void CRenderer::StartDraw(void) {
	// �t�B�[�h�o�b�N�u���[�ݒ� 1�s����
//	m_D3DDevice -> SetRenderTarget(0, m_BlurSurface1);

	m_D3DDevice -> Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	m_D3DDevice -> BeginScene( );
}


/*******************************************************************************
* �֐����FEndDraw
* �����F�Ȃ�
* �߂�l�F�Ȃ�
* �����F�`�揈��
*******************************************************************************/

void CRenderer::EndDraw(void) {
	m_D3DDevice -> EndScene( );

	// �t�B�[�h�o�b�N�u���[�ݒ�@��������
/*
	// �S���2D�|���S���ݒ�
	VERTEX_2D *pVtx2D;

	// ���_���ւ̃|�C���^���擾
	m_pVtxBuff -> Lock(0, 0, (void**)&pVtx2D, 0);

	// ���W�ݒ�
	pVtx2D[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx2D[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx2D[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx2D[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	for(int i = 0; i < 4; i++) {
		pVtx2D[i].col = D3DCOLOR_RGBA(255, 255, 255, 250);
	}

	m_pVtxBuff -> Unlock( );

	m_D3DDevice -> BeginScene( );
//	m_D3DDevice -> SetTexture(0, m_BlurTexture2);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	m_D3DDevice -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	m_D3DDevice -> SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	m_D3DDevice -> SetTexture(0, m_BlurTexture2);

	// 2D�|���S���̕`��
	m_D3DDevice -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

	m_D3DDevice -> EndScene( );

	m_D3DDevice -> SetRenderTarget(0, m_BackBufferSurface);
	m_D3DDevice -> Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// �S���2D�|���S���ݒ�
	m_pVtxBuff -> Lock(0, 0, (void**)&pVtx2D, 0);

	// ���W�ݒ�
	pVtx2D[0].pos = D3DXVECTOR3(-SCREEN_WIDTH * 0.1f, -SCREEN_HEIGHT * 0.1f, 0.0f);
	pVtx2D[1].pos = D3DXVECTOR3(SCREEN_WIDTH * 1.1f, -SCREEN_HEIGHT * 0.1f, 0.0f);
	pVtx2D[2].pos = D3DXVECTOR3(-SCREEN_WIDTH * 0.1f, SCREEN_HEIGHT * 1.1f, 0.0f);
	pVtx2D[3].pos = D3DXVECTOR3(SCREEN_WIDTH * 1.1f, SCREEN_HEIGHT * 1.1f, 0.0f);

	for(int i = 0; i < 4; i++) {
		pVtx2D[i].col = D3DCOLOR_RGBA(255, 255, 255, 250);
	}

	m_pVtxBuff -> Unlock( );

	m_D3DDevice -> BeginScene( );
//	m_D3DDevice -> SetTexture(0, m_BlurTexture1);

	// ���_�o�b�t�@���f�[�^�X�g���[���Ƀo�C���h
	m_D3DDevice -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D) );

	// ���_�t�H�[�}�b�g�̐ݒ�
	m_D3DDevice -> SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	m_D3DDevice -> SetTexture(0, m_BlurTexture1);

	// 2D�|���S���̕`��
	m_D3DDevice -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

	m_D3DDevice -> EndScene( );

	LPDIRECT3DTEXTURE9 texture;
	texture = m_BlurTexture1;
	m_BlurTexture1 = m_BlurTexture2;
	m_BlurTexture2 = texture;

	LPDIRECT3DSURFACE9 surface;
	surface = m_BlurSurface1;
	m_BlurSurface1 = m_BlurSurface2;
	m_BlurSurface2 = surface;*/
	// �����܂�
	
	m_D3DDevice -> Present(NULL, NULL, NULL, NULL);
}

/*******************************************************************************
* �֐����FGetDevice
* �����F�Ȃ�
* �߂�l�Fm_D3DDevice
* �����Fm_D3DDevice�̃A�h���X��Ԃ�
*******************************************************************************/

LPDIRECT3DDEVICE9 CRenderer::GetDevice(void) {
	return m_D3DDevice;
}