/*******************************************************************************
* タイトル GM31 2Dポリゴン描画
* ファイル名 renderer.cpp
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/04/13
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
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
* 関数名：Init
* 引数：hInstance, hWnd, bWindow
* 戻り値：なし
* 説明：初期化処理
*******************************************************************************/

HRESULT CRenderer::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow) {
	// メンバ変数の初期化
	m_D3D = NULL; // Direct3Dオブジェクトへのポインタ
	m_D3DDevice = NULL; // Diret3Dデバイスへのポインタ
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

	// サンプラーステートの設定
	m_D3DDevice -> SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR); //テクスチャ縮小時の補完設定
	m_D3DDevice -> SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR); //テクスチャ拡大時の補完設定

	// フィードバックブラー設定
	D3DXCreateTexture(m_D3DDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_BlurTexture1);
	m_BlurTexture1 -> GetSurfaceLevel(0, &m_BlurSurface1);

	D3DXCreateTexture(m_D3DDevice, SCREEN_WIDTH, SCREEN_HEIGHT, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_BlurTexture2);
	m_BlurTexture2 -> GetSurfaceLevel(0, &m_BlurSurface2);

	m_D3DDevice -> GetRenderTarget(0, &m_BackBufferSurface);

	// 全画面2Dポリゴン
	VERTEX_2D *pVtx2D;
	// 頂点バッファの生成 
	m_D3DDevice -> CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// 頂点情報へのポインタを取得
	m_pVtxBuff -> Lock(0, 0, (void**)&pVtx2D, 0);

	// 座標設定
	pVtx2D[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx2D[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx2D[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx2D[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	for(int i = 0; i < 4; i++) {
		pVtx2D[i].rhw = 1.0f;

		pVtx2D[i].col = D3DCOLOR_RGBA(255, 255, 255, 250);
	}

	// テクスチャUV座標設定
	pVtx2D[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx2D[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx2D[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx2D[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff -> Unlock( );

	return S_OK;
}


/*******************************************************************************
* 関数名：Uninit
* 引数：なし
* 戻り値：なし
* 説明：終了処理
*******************************************************************************/

void CRenderer::Uninit(void) {
	if(m_D3DDevice != NULL) {
		m_D3DDevice -> Release( ); //デバイスの開放
		m_D3DDevice = NULL;
	}

	if(m_D3D != NULL) {
		m_D3D -> Release( ); //オブジェクトの開放
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
* 関数名：Update
* 引数：なし
* 戻り値：なし
* 説明：更新処理
*******************************************************************************/

void CRenderer::Update(void) {

}


/*******************************************************************************
* 関数名：StartDraw
* 引数：なし
* 戻り値：なし
* 説明：描画処理
*******************************************************************************/

void CRenderer::StartDraw(void) {
	// フィードバックブラー設定 1行だけ
//	m_D3DDevice -> SetRenderTarget(0, m_BlurSurface1);

	m_D3DDevice -> Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	m_D3DDevice -> BeginScene( );
}


/*******************************************************************************
* 関数名：EndDraw
* 引数：なし
* 戻り値：なし
* 説明：描画処理
*******************************************************************************/

void CRenderer::EndDraw(void) {
	m_D3DDevice -> EndScene( );

	// フィードバックブラー設定　ここから
/*
	// 全画面2Dポリゴン設定
	VERTEX_2D *pVtx2D;

	// 頂点情報へのポインタを取得
	m_pVtxBuff -> Lock(0, 0, (void**)&pVtx2D, 0);

	// 座標設定
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

	// 頂点バッファをデータストリームにバインド
	m_D3DDevice -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D) );

	// 頂点フォーマットの設定
	m_D3DDevice -> SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	m_D3DDevice -> SetTexture(0, m_BlurTexture2);

	// 2Dポリゴンの描画
	m_D3DDevice -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		2);

	m_D3DDevice -> EndScene( );

	m_D3DDevice -> SetRenderTarget(0, m_BackBufferSurface);
	m_D3DDevice -> Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_RGBA(0, 0, 0, 0), 1.0f, 0);

	// 全画面2Dポリゴン設定
	m_pVtxBuff -> Lock(0, 0, (void**)&pVtx2D, 0);

	// 座標設定
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

	// 頂点バッファをデータストリームにバインド
	m_D3DDevice -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D) );

	// 頂点フォーマットの設定
	m_D3DDevice -> SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	m_D3DDevice -> SetTexture(0, m_BlurTexture1);

	// 2Dポリゴンの描画
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
	// ここまで
	
	m_D3DDevice -> Present(NULL, NULL, NULL, NULL);
}

/*******************************************************************************
* 関数名：GetDevice
* 引数：なし
* 戻り値：m_D3DDevice
* 説明：m_D3DDeviceのアドレスを返す
*******************************************************************************/

LPDIRECT3DDEVICE9 CRenderer::GetDevice(void) {
	return m_D3DDevice;
}