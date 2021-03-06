/*******************************************************************************
* ファイル名 game.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/07/19
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _GAME_H_
#define _GAME_H_

#include "mode.h"

/*******************************************************************************
* マクロ定義
*******************************************************************************/


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/

class CScene; // 前方宣言
class CCamera; // 前方宣言
class CLight; // 前方宣言
class CScore; // 前方宣言
class CPlayer; // 前方宣言
class CEnemy; // 前方宣言
class CAirgauge; // 前方宣言


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/
class CGame : public CMode{
private:
	static CCamera *m_camera; // カメラ
	static CLight *m_light; // ライト
	static CScore *m_score; // スコア情報
	static CPlayer *m_player; // プレイヤー情報
	static CEnemy *m_enemy[ ]; // 敵情報
	static CAirgauge *m_airgauge; // 空気ゲージ
	static bool m_gameEndFlag;
public:
	void Init(void);
	void Uninit(void);
	void Update(void);
	void Draw(void);

	static CCamera *GetCamera(void);
	static CPlayer *GetPlayer(void);
	static CEnemy *GetEnemy(void);
	static CAirgauge *GetAirgauge(void);
	static void EndGame(void);
};

#endif
/*** インクルードガード終了 ***/