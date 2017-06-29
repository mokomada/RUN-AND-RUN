/*******************************************************************************
* ファイル名 scene3D.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/04/20
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _SCENE3D_H_
#define _SCENE3D_H_


/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define VERTEX_NUM (4) // 描画する頂点の個数
#define TRIANGLE_NUM (2) // 描画する三角形の個数

#define FIELD_X (-100) // ポリゴンの描画位置（X座標）
#define FIELD_Z (-100) // ポリゴンの描画位置（Z座標）
#define FIELD_WIDTH (200) // ポリゴンの幅
#define FIELD_HEIGHT (200) // ポリゴンの奥行き

//#define FIELD_TEXTURE_NAME ("data/TEXTURE/field003.jpg") // テクスチャ名
#define FIELD_TEXTURE_NAME (NULL) // テクスチャ名

#define SCENE3D_PRIORITY_NUM (1) // 優先値

/*******************************************************************************
* 構造体定義
*******************************************************************************/


/*******************************************************************************
* 3Dポリゴンクラス
*******************************************************************************/

class CScene3D : public CScene{
private:
	LPDIRECT3DVERTEXBUFFER9 m_VertexBuffer; // 頂点バッファへのポインタ
	LPDIRECT3DTEXTURE9 m_TexturePolygon; // テクスチャへのポインタ

	D3DXMATRIX m_mtxWorld; // ワールドマトリクス


public:
	CScene3D(int priority = 1, OBJTYPE objType = OBJTYPE_NONE); // コンストラクタ
	~CScene3D( ); // デストラクタ

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CScene3D *Create(void); // 必要に応じて初期位置や角度等の引数を
};


#endif
/*** インクルードガード終了 ***/