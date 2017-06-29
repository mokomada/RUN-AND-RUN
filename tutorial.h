/*******************************************************************************
* ファイル名 tutorial.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/07/19
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "mode.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define TUTORIAL_VERTEX_NUM (4) // タイトル画面の描画する頂点の個数
#define TUTORIAL_TRIANGLE_NUM (2) // タイトル画面の描画する三角形の個数
#define TUTORIAL_TEXTURE_NAME ("data/TEXTURE/tutorial.jpg") // タイトル画面のテクスチャ名
#define TUTORIAL_WAIT_TIME (600) // 待機時間
#define TUTORIAL_PRESSENTER_VERTEX_NUM (4) // ロゴの描画する頂点の個数
#define TUTORIAL_PRESSENTER_TRIANGLE_NUM (2) // ロゴの描画する三角形の個数
#define TUTORIAL_PRESSENTER_TEXTURE_NAME ("data/TEXTURE/PressEnter.png") // ロゴのテクスチャ名
#define TUTORIAL_PRESSENTER_WIDTH (328) // ロゴの幅
#define TUTORIAL_PRESSENTER_HEIGHT (58) // ロゴの高さ
#define TUTORIAL_PRESSENTER_POSY (600) // ロゴの表示位置（Y座標）
#define TUTORIAL_PRESSENTER_BLINK_TIME (30) // ロゴの点滅間隔


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/

class CScene; // 前方宣言
class CCamera; // 前方宣言
class CLight; // 前方宣言
class CScore; // 前方宣言
class CPlayer; // 前方宣言
class CEnemy; // 前方宣言


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
class CTutorial : public CMode{
private:
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DTEXTURE9 m_pPETexture;
	LPDIRECT3DVERTEXBUFFER9 m_pPEVtxBuff;
	CCamera *m_camera; // カメラ
	CLight *m_light; // ライト
	CPlayer *m_player; // プレイヤー情報

	int m_timeCount;
	D3DXCOLOR m_PEAlpha; // PRESS ENTERのアルファ値設定用
	float m_alphaChange; // アルファ値の変動量設定

public:
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
};

#endif
/*** インクルードガード終了 ***/