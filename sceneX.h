/*******************************************************************************
* ファイル名 sceneX.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/04/26
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _SCENEX_H_
#define _SCENEX_H_


/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define MODEL_MOVE_SPEED (0.5f) // モデルの移動速度
#define MODEL_ROT_SPEED (0.01f) // モデルの回転速度
#define MODEL_JUMP_SPEED (1.5f) // モデルのジャンプ力
#define MODEL_FALL_SPEED (0.1f) // モデルの落下速度


/*******************************************************************************
* 構造体定義
*******************************************************************************/

typedef struct
{
	D3DXVECTOR3 pos; // 位置情報
	D3DXVECTOR3 move; // 移動量情報
	D3DXQUATERNION rot; // 回転情報
	D3DXVECTOR3 scl; // スケール値　基本的に全て1.0fで固定
	int shadow_num; // 自身の影の番号
	bool jump; // ジャンプ中か否か
	bool onObject;
} MODEL;


/*******************************************************************************
* 3Dポリゴンクラス
*******************************************************************************/

class CSceneX : public CScene{
protected:
	LPDIRECT3DTEXTURE9 m_pTexture;
	LPD3DXMESH m_pMesh; // メッシュ情報
	LPD3DXBUFFER m_pBuffMat; // マテリアル情報
	DWORD m_numMat; // マテリアルの数
	D3DXMATRIX m_mtxWorld; // ワールドマトリックス

	MODEL m_model; // モデル情報
	MODEL m_oldModel; // 前フレームのモデル情報
	float m_newRot; // 進行方向保存用

	void SetNewWay(float set);

public:
	CSceneX(int priority = 1, OBJTYPE objType = OBJTYPE_NONE); // コンストラクタ
	~CSceneX( ); // デストラクタ

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CSceneX *Create(void); // 必要に応じて初期位置や角度等の引数を

	MODEL GetModel(void) {return m_model;} // モデル情報を返す
};


#endif
/*** インクルードガード終了 ***/