/*******************************************************************************
* ファイル名 meshfield.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/5/18
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _MESHFIELD_H_
#define _MESHFIELD_H_

/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define MESHFIELD_X_BLOCK (40) // X方向のブロック数
#define MESHFIELD_Z_BLOCK (40) // Z方向のブロック数

#define MESHFIELD_TEXTURE_X_BLOCK (1) // 何ブロックで1枚のテクスチャを使うか(X方向)
#define MESHFIELD_TEXTURE_Z_BLOCK (1) // 何ブロックで1枚のテクスチャを使うか(Z方向)

#define MESHFIELD_FIELD_WIDTH (500) // フィールドの幅
#define MESHFIELD_FIELD_HEIGHT (500) // フィールドの高さ

#define MESHFIELD_FIELD_X (-250) // フィールドの左上端のX座標
#define MESHFIELD_FIELD_Z (250) // フィールドの左上端のZ座標

#define MESHFIELD_VERTEX_NUM ( (MESHFIELD_X_BLOCK + 1) * (MESHFIELD_Z_BLOCK + 1) ) // 描画する頂点の個数
#define MESHFIELD_INDEX_NUM ( (MESHFIELD_X_BLOCK + 1) * MESHFIELD_Z_BLOCK * 2 + (MESHFIELD_Z_BLOCK - 1) * 2) // インデックスの個数
#define MESHFIELD_TRIANGLE_NUM (MESHFIELD_X_BLOCK * MESHFIELD_Z_BLOCK * 2 + (MESHFIELD_Z_BLOCK - 1) * 4) // 描画する三角形の個数

#define MESHFIELD_TEXTURE_NAME "data/TEXTURE/field001.jpg" // テクスチャーに使う画像指定
//#define MESHFIELD_TEXTURE_NAME NULL // テクスチャー邪魔な時用

#define MESHFIELD_DIFFERENCE_MAX (3.0f) // 隣の頂点との高さの差の最大値


/*******************************************************************************
* メッシュフィールドクラス
*******************************************************************************/

class CMeshfield : public CScene{
private:
	LPDIRECT3DTEXTURE9 m_pTextureMeshfield; // テクスチャへのポインタ
	static LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffMeshfield; //頂点バッファへのポインタ
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuffMeshfield; // インデックスバッファへのポインタ

	D3DXVECTOR3 m_posMeshfield; // 位置情報
	D3DXVECTOR3 m_rotMeshfield; // 回転情報
	D3DXVECTOR3 m_sclMeshfield; // スケール値　基本的に全て1.0fで固定

	D3DXMATRIX m_mtxWorldMeshfield; // ワールドマトリックス

	float m_HeightMap[MESHFIELD_X_BLOCK + 1][MESHFIELD_Z_BLOCK + 1]; // 各座標の高さ

public:
	CMeshfield(int priority = 1, OBJTYPE objType = OBJTYPE_FIELD); // コンストラクタ
	~CMeshfield( ); // デストラクタ

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CMeshfield *Create(void);
	static float GetFieldHeight(D3DXVECTOR3 pos);
	static D3DXVECTOR3 GetFieldNorm(D3DXVECTOR3 pos);
};

#endif
/*** インクルードガード終了 ***/