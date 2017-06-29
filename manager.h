/*******************************************************************************
* ファイル名 manager.h
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/05/11
*******************************************************************************/

/*** インクルードガード開始 ***/
#ifndef _MANAGER_H_
#define _MANAGER_H_


#include <time.h>

/*******************************************************************************
* マクロ定義
*******************************************************************************/


/*******************************************************************************
* 構造体定義
*******************************************************************************/

enum MODE{
	MODE_TITLE = 0,
	MODE_TUTORIAL,
	MODE_GAME,
	MODE_RESULT
};


/*******************************************************************************
* プロトタイプ宣言
*******************************************************************************/

class CRenderer; // 前方宣言
class CInput; // 前方宣言
class CSound; // 前方宣言
class CMode; // 前方宣言
class CFade; // 前方宣言
class CCamera; // 前方宣言

/*******************************************************************************
* マネージャクラス
*******************************************************************************/

class CManager {
private:
	static CRenderer *m_renderer; // レンダラー
	static CInput *m_input; // 入力情報
	static CSound *m_sound; // サウンド情報
	static CMode *m_mode; // モード情報
	static CFade *m_fade; // フェード情報
	static CCamera *m_camera; // カメラ情報

public:
	CManager( );
	~CManager( );

	static HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	static void Uninit(void);
	static void Update(void);
	static void Draw(void);

	static CRenderer *GetRenderer(void);

	static CInput *GetInput(void);
	static void SetMode(CMode *mode);
	static CMode *GetMode(void);
	static CFade *GetFade(void);
	static CCamera *GetCamera(void);

	static bool m_firstTimeFlag; // 1周目フラグ（チュートリアルの表示時間管理用）
};


#endif
/*** インクルードガード終了 ***/