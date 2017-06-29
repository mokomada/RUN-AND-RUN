/*******************************************************************************
* ファイル名 sceneX.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/04/26
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _PLAYER_H_
#define _PLAYER_H_


/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define PLAYER_NAME "data/MODEL/dog.x" // 読み込むXファイル名
#define PLAYER_TEXTURE_NAME NULL // テクスチャ名

#define PLAYER_MOVE_SPEED (0.5f) // モデルの移動速度
#define PLAYER_ROT_SPEED (0.01f) // モデルの回転速度
#define PLAYER_JUMP_SPEED (1.5f) // モデルのジャンプ力
#define PLAYER_FALL_SPEED (0.1f) // モデルの落下速度

#define PLAYER_SCALE_X (1.0f) // スケール値 X軸
#define PLAYER_SCALE_Y (1.0f) // スケール値 Y軸
#define PLAYER_SCALE_Z (1.0f) // スケール値 Z軸

#define PLAYER_ROT_X (0.0f) // 回転量 X軸
#define PLAYER_ROT_Y (0.0f) // 回転量 Y軸
#define PLAYER_ROT_Z (0.0f) // 回転量 Z軸

#define PLAYER_POS_X (30.0f) // 位置情報 X軸
#define PLAYER_POS_Y (0.0f) // 位置情報 Y軸
#define PLAYER_POS_Z (30.0f) // 位置情報 Z軸

#define PLAYER_MOVE_MAX (3.0f) // 最大移動量
#define PLAYER_MOVE_REDUCE (0.0f) // 移動量減衰量
#define PLAYER_FALL_REDUCE (1.0f) // 落下速度減衰量

#define PLAYER_VIEW_DISTANCE (0.0f) // モデルの中心点から注視点までの距離

/*******************************************************************************
* 構造体定義
*******************************************************************************/


/*******************************************************************************
* 3Dポリゴンクラス
*******************************************************************************/

class CPlayer : public CSceneX{
private:
	bool m_attackFlag;
	int m_attackCooltime;
	D3DXVECTOR3 m_moveReduce;
	float m_airLeft;

public:
	CPlayer(int priority = 1, OBJTYPE objType = OBJTYPE_PLAYER); // コンストラクタ
	~CPlayer( ); // デストラクタ

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CPlayer *Create(void); // 必要に応じて初期位置や角度等の引数を
};


#endif
/*** インクルードガード終了 ***/