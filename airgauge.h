/*******************************************************************************
* ファイル名 airgauge.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2017/01/26
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _AIRGAUGE_H_
#define _AIRGAUGE_H_


/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define AIRGAUGE_VERTEX_NUM (4) /* 描画する頂点の個数 */
#define AIRGAUGE_TRIANGLE_NUM (2) /* 描画する三角形の個数 */

#define AIRGAUGE_X (70) // ポリゴンの描画位置（X座標）
#define AIRGAUGE_Y (160) // ポリゴンの描画位置（Y座標）
#define AIRGAUGE_WIDTH (30) // ポリゴンの幅
#define AIRGAUGE_HEIGHT (100) // ポリゴンの高さ


/*******************************************************************************
* 構造体定義
*******************************************************************************/


/*******************************************************************************
* 2Dポリゴンクラス
*******************************************************************************/

class CAirgauge : public CScene{
private:
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffAirgauge; // 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9 m_TextureAirgauge; // テクスチャへのポインタ

	static bool m_drawGaugeFlag;

public:
	CAirgauge(int priority = 2, OBJTYPE objType = OBJTYPE_NONE); // コンストラクタ
	~CAirgauge( ); // デストラクタ

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CAirgauge *Create(void); // 必要に応じて初期位置や角度等の引数を
	static void SetGauge(float height);
	static float GetGauge(void);
};

#endif
/*** インクルードガード終了 ***/