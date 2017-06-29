/*******************************************************************************
* ファイル名 game.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/07/19
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _RESULT_H_
#define _RESULT_H_

#include "mode.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define RESULT_VERTEX_NUM (4) // リザルト画面の描画する頂点の個数
#define RESULT_TRIANGLE_NUM (2) // リザルト画面の描画する三角形の個数
#define RESULT_TEXTURE_NAME ("data/TEXTURE/bg102.jpg") // リザルト画面のテクスチャ名

#define RESULT_LOGO_VERTEX_NUM (4) // ロゴの描画する頂点の個数
#define RESULT_LOGO_TRIANGLE_NUM (2) // ロゴの描画する三角形の個数
#define RESULT_LOGO_TEXTURE_NAME ("data/TEXTURE/result.jpg") // ロゴのテクスチャ名
#define RESULT_LOGO_WIDTH (SCREEN_WIDTH) // ロゴの幅
#define RESULT_LOGO_HEIGHT (SCREEN_HEIGHT) // ロゴの高さ

#define RESULT_SCORE_WIDTH (40) // スコアの幅
#define RESULT_SCORE_HEIGHT (80) // スコアの高さ
#define RESULT_SCORE_NUM (5) // スコアの表示数
#define RESULT_SCORE_SPACE (40) // スコアの表示間隔

#define RESULT_WAIT_TIME (450) // リザルト画面の待機時間
#define RESULT_STOP_MOVE_TIME (300) // スコアの移動が止まる時間


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
class CResult : public CMode{
private:
	LPDIRECT3DTEXTURE9 m_pTextureResultScore[RESULT_SCORE_NUM]; // スコア用テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffResultScore[RESULT_SCORE_NUM]; //頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9 m_pTextureResultLogo; // ロゴ用テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffResultLogo; // ロゴの頂点バッファへのポインタ
	int result_wait_time;
	static int m_result_score[RESULT_SCORE_NUM];
public:
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
};

#endif
/*** インクルードガード終了 ***/