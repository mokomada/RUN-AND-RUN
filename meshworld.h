/*******************************************************************************
* ファイル名 meshworld.h
* 作成者 AT-12C-245 秋間雄太
* 作成日 2015/12/10
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _MESHWORLD_H_
#define _MESHWORLD_H_

/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define MESHWORLD_NUM (2) // 外郭の数

#define MESHWORLD_X_BLOCK (32) // X方向のブロック数 1以上の整数
#define MESHWORLD_Y_BLOCK (32) // Y方向のブロック数 1以上の整数

#define MESHWORLD_TEXTURE_X_BLOCK (32) // 何ブロックで1枚のテクスチャを使うか(X方向)
#define MESHWORLD_TEXTURE_Y_BLOCK (16) // 何ブロックで1枚のテクスチャを使うか(Y方向)
#define MESHWORLD_TEXTURE_START_X (0) // 何ブロック目からテクスチャを適用し始めるか(X方向)
#define MESHWORLD_TEXTURE_START_Y (16) // 何ブロック目からテクスチャを適用し始めるか(Y方向)
#define MESHWORLD_TEXTURE_ADJUST (0.003f) // テクスチャ調整用（一番上からテクスチャを適用開始するブロックまで、テクスチャのどの位置を引き延ばすか）

#define MESHWORLD_FIELD_X (-MESHWORLD_FIELD_WIDTH * 0.5f) // 外郭の左上端のX座標
#define MESHWORLD_FIELD_Y (MESHWORLD_FIELD_HEIGHT) // 外郭の左上端のY座標

#define MESHWORLD_VERTEX_NUM ( (MESHWORLD_X_BLOCK + 1) * (MESHWORLD_Y_BLOCK + 1) ) // 描画する頂点の個数
#define MESHWORLD_INDEX_NUM ( (MESHWORLD_X_BLOCK + 1) * MESHWORLD_Y_BLOCK * 2 + (MESHWORLD_Y_BLOCK - 1) * 2) // インデックスの個数
#define MESHWORLD_TRIANGLE_NUM (MESHWORLD_X_BLOCK * MESHWORLD_Y_BLOCK * 2 + (MESHWORLD_Y_BLOCK - 1) * 4) // 描画する三角形の個数

#define MESHWORLD_DISTANCE (2000) // 外郭の半径

#define MESHWORLD_VERTEX_FAN_NUM ( (MESHWORLD_X_BLOCK + 2) ) // 描画する頂点の個数(蓋）
#define MESHWORLD_TRIANGLE_FAN_NUM (MESHWORLD_X_BLOCK) // 描画する三角形の個数(蓋)


#define MESHWORLD_TEXTURE_NAME "data/TEXTURE/sky000.jpg" // テクスチャーに使う画像指定


/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct{
	D3DXVECTOR3 pos; // 位置情報
	D3DXVECTOR3 rot; // 回転情報
	D3DXVECTOR3 scl; // スケール値　基本的に全て1.0fで固定
} MESHWORLD;


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/

class CMeshworld : public CScene{
private:
	LPDIRECT3DTEXTURE9 m_pTexture; // テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff; //頂点バッファへのポインタ
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffLid; //頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff; // インデックスバッファへのポインタ

	D3DXVECTOR3 m_pos; // 位置情報
	D3DXVECTOR3 m_rot; // 回転情報
	D3DXVECTOR3 m_scl; // スケール値　基本的に全て1.0fで固定

	D3DXMATRIX m_mtxWorld; // ワールドマトリックス

	MESHWORLD m_meshworld; // 外郭情報

public:
	CMeshworld(int priority = 2, OBJTYPE objType = OBJTYPE_FIELD); // コンストラクタ
	~CMeshworld( ); // デストラクタ

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshworld *Create(void);
};
#endif
/*** インクルードガード終了 ***/