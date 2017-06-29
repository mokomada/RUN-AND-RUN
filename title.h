/*******************************************************************************
* ファイル名 title.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/07/19
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _TITLE_H_
#define _TITLE_H_

#include "mode.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define TITLE_VERTEX_NUM (4) // タイトル画面の描画する頂点の個数
#define TITLE_TRIANGLE_NUM (2) // タイトル画面の描画する三角形の個数
#define TITLE_TEXTURE_NAME ("data/TEXTURE/title.jpg") // タイトル画面のテクスチャ名
#define TITLE_LOGO_VERTEX_NUM (4) // ロゴの描画する頂点の個数
#define TITLE_LOGO_TRIANGLE_NUM (2) // ロゴの描画する三角形の個数
#define TITLE_LOGO_TEXTURE_NAME ("data/TEXTURE/PressEnter.png") // ロゴのテクスチャ名
#define TITLE_LOGO_WIDTH (328) // ロゴの幅
#define TITLE_LOGO_HEIGHT (58) // ロゴの高さ
#define TITLE_LOGO_POSY (600) // ロゴの表示位置（Y座標）
#define TITLE_LOGO_BLINK_TIME (30) // ロゴの点滅間隔
#define TITLE_WAIT_TIME (600) // 待機時間



/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
class CTitle : public CMode{
private:
	LPDIRECT3DTEXTURE9 m_pTextureTitle; // タイトル画面用テクスチャへのポイン
	LPDIRECT3DTEXTURE9 m_pTextureTitleLogo; // ロゴ用テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffTitle; // タイトル画面の頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffTitleLogo; // ロゴの頂点バッファへのポインタ
	int m_title_time_count; // カウンター
	D3DXCOLOR m_title_logo_alpha; // ロゴのアルファ値設定用
	float m_alpha_change; // アルファ値の変動量設定

public:
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
};

#endif
/*** インクルードガード終了 ***/