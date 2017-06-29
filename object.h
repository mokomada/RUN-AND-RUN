/*******************************************************************************
* ファイル名 object.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/09/27
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _OBJECT_H_
#define _OBJECT_H_


/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define OBJECT_NAME "data/MODEL/dog.x" // 読み込むXファイル名
#define OBJECT_TEXTURE_NAME NULL // テクスチャ名

#define OBJECT_SCALE_X (1.0f) // スケール値 X軸
#define OBJECT_SCALE_Y (1.0f) // スケール値 Y軸
#define OBJECT_SCALE_Z (1.0f) // スケール値 Z軸

#define OBJECT_ROT_X (0.0f) // 回転量 X軸
#define OBJECT_ROT_Y (0.0f) // 回転量 Y軸
#define OBJECT_ROT_Z (0.0f) // 回転量 Z軸

#define OBJECT_POS_X (0.0f) // 位置情報 X軸
#define OBJECT_POS_Y (0.0f) // 位置情報 Y軸
#define OBJECT_POS_Z (0.0f) // 位置情報 Z軸

#define OBJECT_X_BLOCK (2) // X方向のブロック数
#define OBJECT_Y_BLOCK (2) // Y方向のブロック数

#define OBJECT_TEXTURE_X_BLOCK (1) // 何ブロックで1枚のテクスチャを使うか(X方向)
#define OBJECT_TEXTURE_Y_BLOCK (1) // 何ブロックで1枚のテクスチャを使うか(Y方向)

#define OBJECT_VERTEX_NUM ( (OBJECT_X_BLOCK + 1) * (OBJECT_Y_BLOCK + 1) ) // 描画する頂点の個数
#define OBJECT_INDEX_NUM ( (OBJECT_X_BLOCK + 1) * OBJECT_Y_BLOCK * 2 + (OBJECT_Y_BLOCK - 1) * 2) // インデックスの個数
#define OBJECT_TRIANGLE_NUM (OBJECT_X_BLOCK * OBJECT_Y_BLOCK * 2 + (OBJECT_Y_BLOCK - 1) * 4) // 描画する三角形の個数


/*******************************************************************************
* 構造体定義
*******************************************************************************/



/*******************************************************************************
* オブジェクトクラス
*******************************************************************************/

class CObject : public CScene{
private:
	//LPD3DXMESH m_pMesh; // メッシュ情報
	//LPD3DXBUFFER m_pBuffMat; // マテリアル情報
	//DWORD m_numMat; // マテリアルの数
	D3DXMATRIX m_mtxWorld; // ワールドマトリックス
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;
	LPDIRECT3DINDEXBUFFER9 m_pIdxBuff;
	LPDIRECT3DVERTEXBUFFER9 m_pVtxTopBuff;
	LPDIRECT3DINDEXBUFFER9 m_pIdxTopBuff;
	MODEL m_objMng;
	MODEL m_object[5];
	float m_height;
	float m_width;
	D3DXVECTOR3 m_oldPlayerPos;
	bool m_playerOnFlag;
	bool m_alphaFlag;

public:
	CObject(int priority = 1, OBJTYPE objType = OBJTYPE_SOLID); // コンストラクタ
	~CObject( ); // デストラクタ

	void Init(void);
	void Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	OBJHITCHK HitCheck(D3DXVECTOR3 pos, D3DXVECTOR3 oldPos);
	void SetAlphaFlag(bool onoff);
	bool GetAlphaFlag(void) {return m_alphaFlag;}

	static CObject *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float height, float width, float depth); // 必要に応じて初期位置や角度等の引数を
	static MODEL GetObject(void);
};


#endif
/*** インクルードガード終了 ***/