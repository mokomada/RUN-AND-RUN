/*******************************************************************************
* ファイル名 renderer.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/04/13
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _RENDERER_H_
#define _RENDERER_H_


/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define SCENE_NUM (3) // シーン数


/*******************************************************************************
* 構造体定義
*******************************************************************************/


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/


/*******************************************************************************
* レンダラクラス
*******************************************************************************/

class CRenderer {
private:
	LPDIRECT3D9 m_D3D; // Direct3Dオブジェクトへのポインタ
	LPDIRECT3DDEVICE9 m_D3DDevice; // Diret3Dデバイスへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; //頂点バッファへのポインタ
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
/*** インクルードガード終了 ***/