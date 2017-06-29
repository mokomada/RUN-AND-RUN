/*******************************************************************************
* ファイル名 sceneX.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/04/26
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _ENEMY_H_
#define _ENEMY_H_


/*******************************************************************************
* マクロ定義
*******************************************************************************/

#define ENEMY_NAME "data/MODEL/dogE.x" // 読み込むXファイル名
#define ENEMY_TEXTURE_NAME NULL // テクスチャ名

#define ENEMY_MOVE_SPEED (0.5f) // モデルの移動速度
#define ENEMY_ROT_SPEED (0.03f) // モデルの回転速度
#define ENEMY_JUMP_SPEED (1.5f) // モデルのジャンプ力
#define ENEMY_FALL_SPEED (0.1f) // モデルの落下速度

#define ENEMY_SCALE_X (1.0f) // スケール値 X軸
#define ENEMY_SCALE_Y (1.0f) // スケール値 Y軸
#define ENEMY_SCALE_Z (1.0f) // スケール値 Z軸

#define ENEMY_ROT_X (0.0f) // 回転量 X軸
#define ENEMY_ROT_Y (0.0f) // 回転量 Y軸
#define ENEMY_ROT_Z (0.0f) // 回転量 Z軸

#define ENEMY_POS_X (0.0f) // 位置情報 X軸
#define ENEMY_POS_Y (0.0f) // 位置情報 Y軸
#define ENEMY_POS_Z (0.0f) // 位置情報 Z軸

#define ENEMY_MOVE_MAX (3.0f) // 最大移動量
#define ENEMY_MOVE_REDUCE (0.0f) // 移動量減衰量
#define ENEMY_FALL_REDUCE (1.0f) // 落下速度減衰量

#define ENEMY_VIEW_DISTANCE (0.0f) // モデルの中心点から注視点までの距離

#define ENEMY_MOVE_CHANGE (30) // 移動方向を変更するまでの時間
#define ENEMY_RESPAWN_TIME (300) // リスポーンまでの時間
#define ENEMY_START_DISTANCE (20.0f) // 初期位置のプレイヤーとの最低距離

#define ENEMY_SEARCH_START (70) // 索敵開始距離（この距離以下まで近づくと逃げ出す）
#define ENEMY_SEARCH_STOP (200) // 索敵終了距離（この距離以上まで離れると逃げるのをやめる）

#define ENEMY_MAX (3) // エネミーの最大数

/*******************************************************************************
* 構造体定義
*******************************************************************************/


/*******************************************************************************
* 3Dポリゴンクラス
*******************************************************************************/

class CEnemy : public CSceneX{
private:
	int m_moveChange;
	int m_respawn;
	bool m_moveFlag;
	bool m_jumpFlag;
	int m_rnd;
	bool m_use;

public:
	CEnemy(int priority = 1, OBJTYPE objType = OBJTYPE_ENEMY); // コンストラクタ
	~CEnemy( ); // デストラクタ

	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CEnemy *Create(void); // 必要に応じて初期位置や角度等の引数を
	bool ChkEnemy(void) {return m_use;}
};


#endif
/*** インクルードガード終了 ***/