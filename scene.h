/*******************************************************************************
* ファイル名 scene.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/04/19
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _SCENE_H_
#define _SCENE_H_


/*******************************************************************************
* マクロ定義
*******************************************************************************/

// 2Dポリゴンの頂点フォーマット
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// 3Dポリゴンの頂点フォーマット
#define FVF_VERTEX_3D (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)

#define SCENE_PRIORITY_NUM (3) // 優先度の数


/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct{
	D3DXVECTOR3 pos; // 頂点座標
	float rhw; // 座標交換用係数
	D3DCOLOR col; // 頂点カラー
	D3DXVECTOR2 tex; // テクスチャ座標
} VERTEX_2D;

typedef struct{
	D3DXVECTOR3 pos; // 頂点座標
	D3DXVECTOR3 nor; // 法線
	D3DCOLOR col; // 頂点カラー
	D3DXVECTOR2 tex; // テクスチャ座標
} VERTEX_3D;

// オブジェクトタイプ
enum OBJTYPE{
	OBJTYPE_NONE,
	OBJTYPE_PLAYER,
	OBJTYPE_ENEMY,
	OBJTYPE_SOLID, // 貫通しない物体
	OBJTYPE_FIELD,
	OBJTYPE_LIQUID // 液体。通り抜け可
};

// オブジェクト当たり判定用
typedef struct{
	int type;
	D3DXVECTOR3 nor;
}OBJHITCHK;


/*******************************************************************************
* 基底クラス
*******************************************************************************/

class CScene {
protected:
	static CScene *m_top[3];
	static CScene *m_cur[3];
	CScene *m_prev;
	CScene *m_next;
	OBJTYPE m_objType;

	D3DXVECTOR3 m_position;
	D3DXVECTOR3 m_rotation;
	D3DXVECTOR3 m_scale;

public:
	CScene(int priority = 1, OBJTYPE objType = OBJTYPE_NONE); // コンストラクタ
	~CScene( ); // デストラクタ

	virtual void Init(void) = 0;
	virtual void Uninit(void) = 0;
	virtual void Update(void) = 0;
	virtual void Draw(void) = 0;

	static void UpdateAll(void);
	static void DrawAll(void);
	static void UninitAll(void);
	void Release(int priority);

	static OBJHITCHK HitCheckAll(D3DXVECTOR3 pos, D3DXVECTOR3 oldPos);
	static bool ChkEnemyAll(void);

	static CScene *GetList(int priority);
	CScene *GetNext(void);
	OBJTYPE GetObjType(void);

	void SetPosition(D3DXVECTOR3 pos);
	D3DXVECTOR3 GetPosition(void);
	void SetRotation(D3DXVECTOR3 rot);
	D3DXVECTOR3 GetRotation(void);
};


#endif
/*** インクルードガード終了 ***/