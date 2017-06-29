/*******************************************************************************
* ファイル名 meshwater.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/5/18
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _MESHWATER_H_
#define _MESHWATER_H_

/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define MESHWATER_X_BLOCK (25) // X方向のブロック数
#define MESHWATER_Z_BLOCK (25) // Z方向のブロック数

#define MESHWATER_TEXTURE_X_BLOCK (1) // 何ブロックで1枚のテクスチャを使うか(X方向)
#define MESHWATER_TEXTURE_Z_BLOCK (1) // 何ブロックで1枚のテクスチャを使うか(Z方向)

#define MESHWATER_WATER_WIDTH (2000) // フィールドの幅
#define MESHWATER_WATER_HEIGHT (2000) // フィールドの高さ

#define MESHWATER_MAX_HEIGHT (340) // 水面の最大値
#define MESHWATER_MIN_HEIGHT (335) // 水面の最小値

#define MESHWATER_MOVE_TIME (10) // 水面の移動速度

#define MESHWATER_WATER_X (-1000) // フィールドの左上端のX座標
#define MESHWATER_WATER_Z (1000) // フィールドの左上端のZ座標

#define MESHWATER_VERTEX_NUM ( (MESHWATER_X_BLOCK + 1) * (MESHWATER_Z_BLOCK + 1) ) // 描画する頂点の個数
#define MESHWATER_INDEX_NUM ( (MESHWATER_X_BLOCK + 1) * MESHWATER_Z_BLOCK * 2 + (MESHWATER_Z_BLOCK - 1) * 2) // インデックスの個数
#define MESHWATER_TRIANGLE_NUM (MESHWATER_X_BLOCK * MESHWATER_Z_BLOCK * 2 + (MESHWATER_Z_BLOCK - 1) * 4) // 描画する三角形の個数

#define MESHWATER_TEXTURE_NAME NULL // テクスチャーに使う画像指定
//#define MESHWATER_TEXTURE_NAME NULL // テクスチャー邪魔な時用


/*******************************************************************************
* メッシュフィールドクラス
*******************************************************************************/

class CMeshwater : public CScene{
private:
	LPDIRECT3DTEXTURE9 m_pTextureMeshwater; // テクスチャへのポインタ
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMeshwater; //頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffMeshwater; // インデックスバッファへのポインタ

	D3DXVECTOR3 m_posMeshwater; // 位置情報
	D3DXVECTOR3 m_rotMeshwater; // 回転情報
	D3DXVECTOR3 m_sclMeshwater; // スケール値　基本的に全て1.0fで固定

	D3DXMATRIX m_mtxWorldMeshwater; // ワールドマトリックス

	int m_time;

public:
	CMeshwater(int priority = 2, OBJTYPE objType = OBJTYPE_LIQUID); // コンストラクタ
	~CMeshwater( ); // デストラクタ

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshwater *Create(void);
	static float GetWaterHeight(D3DXVECTOR3 pos);
};

#endif
/*** インクルードガード終了 ***/