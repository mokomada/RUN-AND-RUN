/*******************************************************************************
* ファイル名 object.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/09/27
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _STENCILSHADOW_H_
#define _STENCILSHADOW_H_


/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define STENCILSHADOW_NAME "data/MODEL/dog.x" // 読み込むXファイル名
#define STENCILSHADOW_TEXTURE_NAME NULL // テクスチャ名

#define STENCILSHADOW_SCALE_X (1.0f) // スケール値 X軸
#define STENCILSHADOW_SCALE_Y (1.0f) // スケール値 Y軸
#define STENCILSHADOW_SCALE_Z (1.0f) // スケール値 Z軸

#define STENCILSHADOW_ROT_X (0.0f) // 回転量 X軸
#define STENCILSHADOW_ROT_Y (0.0f) // 回転量 Y軸
#define STENCILSHADOW_ROT_Z (0.0f) // 回転量 Z軸

#define STENCILSHADOW_POS_X (0.0f) // 位置情報 X軸
#define STENCILSHADOW_POS_Y (0.0f) // 位置情報 Y軸
#define STENCILSHADOW_POS_Z (0.0f) // 位置情報 Z軸

#define STENCILSHADOW_X_BLOCK (20) // X方向のブロック数
#define STENCILSHADOW_Y_BLOCK (4) // Y方向のブロック数

#define STENCILSHADOW_TEXTURE_X_BLOCK (1) // 何ブロックで1枚のテクスチャを使うか(X方向)
#define STENCILSHADOW_TEXTURE_Y_BLOCK (1) // 何ブロックで1枚のテクスチャを使うか(Y方向)

#define STENCILSHADOW_VERTEX_NUM ( (STENCILSHADOW_X_BLOCK + 1) * (STENCILSHADOW_Y_BLOCK + 1) ) // 描画する頂点の個数
#define STENCILSHADOW_INDEX_NUM ( (STENCILSHADOW_X_BLOCK + 1) * STENCILSHADOW_Y_BLOCK * 2 + (STENCILSHADOW_Y_BLOCK - 1) * 2) // インデックスの個数
#define STENCILSHADOW_TRIANGLE_NUM (STENCILSHADOW_X_BLOCK * STENCILSHADOW_Y_BLOCK * 2 + (STENCILSHADOW_Y_BLOCK - 1) * 4) // 描画する三角形の個数

#define STENCILSHADOW_VERTEX_FAN_NUM ( (STENCILSHADOW_X_BLOCK + 2) ) // 描画する頂点の個数(蓋）
#define STENCILSHADOW_TRIANGLE_FAN_NUM (STENCILSHADOW_X_BLOCK) // 描画する三角形の個数(蓋)

#define STENCILSHADOW_DISTANCE (4) // 半径

/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct{
	D3DXVECTOR3 pos; // 位置情報
	D3DXVECTOR3 rot; // 回転情報
	D3DXVECTOR3 scl; // スケール値　基本的に全て1.0fで固定
} STENCILSHADOW;

/*******************************************************************************
* オブジェクトクラス
*******************************************************************************/

class CStencilShadow : public CScene{
private:
	LPD3DXMESH m_pMesh; // メッシュ情報
	//LPD3DXBUFFER m_pBuffMat; // マテリアル情報
	//DWORD m_numMat; // マテリアルの数
	D3DXMATRIX m_mtxWorld; // ワールドマトリックス
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffLid;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuffScreen;
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
	MODEL m_objMng;
	MODEL m_object[5];
	float m_height;
	float m_width;
	D3DXVECTOR3 m_oldPlayerPos;

	STENCILSHADOW m_stencilshadow; // 外郭情報

public:
	CStencilShadow(int priority = 1, OBJTYPE objType = OBJTYPE_SOLID); // コンストラクタ
	~CStencilShadow( ); // デストラクタ

	void Init(void);
	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CStencilShadow *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth); // 必要に応じて初期位置や角度等の引数を
	static MODEL GetObject(void);
};


#endif
/*** インクルードガード終了 ***/