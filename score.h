/*******************************************************************************
* ファイル名 score.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/06/24
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _SCORE_H_
#define _SCORE_H_

/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define NUMBER_NUM (3) // 表示するスコアの桁数
#define SCORE_VERTEX_NUM (4 * NUMBER_NUM) // 描画する頂点の個数
#define SCORE_TRIANGLE_NUM (4 * NUMBER_NUM - 2) // 描画する三角形の個数
#define SCORE_TEXTURE_NAME ("data/TEXTURE/number000.png") // テクスチャ名

#define BOX_WIDTH (35) // 1つの数字の幅
#define BOX_HEIGHT (70) // 1つの数字の高さ
#define TEX_LENGTH (0.1) // テクスチャの数字の間隔

#define SCORE_MIN (0) // スコアの最小値
#define SCORE_MAX (999 * 60) // スコアの最大値


/*******************************************************************************
* スコアクラス
*******************************************************************************/

class CScore {
private:
	LPDIRECT3DTEXTURE9 m_pTextureScore; // スコア用テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffScore; //頂点バッファへのポインタ
	static int m_nScore; /* スコアの値 */

public:
	CScore(int priority = 2, OBJTYPE objType = OBJTYPE_NONE); // コンストラクタ
	~CScore( ); // デストラクタ

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);
	static void AddScore(int score);
	static int GetScore(void);
	static CScore *Create(void);
};

#endif
/*** インクルードガード終了 ***/