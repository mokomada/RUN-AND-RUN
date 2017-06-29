/*******************************************************************************
* タイトル マネージャークラス
* ファイル名 manager.cpp
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/05/11
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "mode.h"
#include "input.h"
#include "sound.h"
#include "camera.h"
#include "title.h"
#include "fade.h"
#include "game.h"
#include "tutorial.h"


/*******************************************************************************
* スタティック変数初期化
*******************************************************************************/

CRenderer *CManager::m_renderer = NULL; // レンダラー
CInput *CManager::m_input = NULL; // 入力情報
CSound *CManager::m_sound = NULL; // サウンド情報
CMode *CManager::m_mode = NULL; // モード情報
CFade *CManager::m_fade = NULL; // フェード情報
CCamera *CManager::m_camera = NULL; // カメラ情報
bool CManager::m_firstTimeFlag = true; // 1週目フラグ



/*******************************************************************************
* 関数名：CManager
* 引数：なし
* 戻り値：なし
* 説明：クラス生成時処理
*******************************************************************************/

CManager::CManager( ) {

}


/*******************************************************************************
* 関数名：~CManager
* 引数：なし
* 戻り値：なし
* 説明：クラス破棄時処理
*******************************************************************************/

CManager::~CManager( ) {

}


/*******************************************************************************
* 関数名：Init
* 引数：なし
* 戻り値：なし
* 説明：カメラの初期化
*******************************************************************************/

HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow) {
	// 乱数設定
	srand( (unsigned) time(NULL) );

	// オブジェクト生成
	m_renderer = new CRenderer;
	m_renderer -> Init(hInstance, hWnd, bWindow);

	m_input = new CInput;
	m_input -> Init(hInstance, hWnd);

	m_sound = new CSound;
	m_sound -> Init(hWnd);

	m_fade = new CFade;
	m_fade -> Init( );

	m_camera = new CCamera;
	m_camera -> Init( );

	SetMode(new CTitle);

	return S_OK;
}


/*******************************************************************************
* 関数名：Uninit
* 引数：なし
* 戻り値：なし
* 説明：カメラの終了処理
*******************************************************************************/

void CManager::Uninit(void) {
	// オブジェクト破棄
	if(m_renderer != NULL) {
		m_renderer -> Uninit( );
		delete m_renderer;
		m_renderer = NULL;
	}

	if(m_input != NULL) {
		m_input -> Uninit( );
		delete m_input;
		m_input = NULL;
	}

	if(m_sound != NULL) {
		m_sound -> Uninit( );
		delete m_sound;
		m_sound = NULL;
	}

	if(m_fade != NULL) {
		m_fade -> Uninit( );
		delete m_fade;
		m_fade = NULL;
	}

	if(m_camera != NULL) {
		m_camera -> Uninit( );
		delete m_camera;
		m_camera = NULL;
	}
}

/*******************************************************************************
* 関数名：Update
* 引数：なし
* 戻り値：なし
* 説明：カメラの更新処理
*******************************************************************************/

void CManager::Update(void) {
	m_input -> Update( );

	m_renderer -> Update( );

	m_mode -> Update( );

	m_fade -> Update( );
}


/*******************************************************************************
* 関数名：Draw
* 引数：なし
* 戻り値：なし
* 説明：描画処理
*******************************************************************************/

void CManager::Draw(void) {

/*#ifdef _DEBUG
	DrawFPS( );
#endif*/
	m_renderer -> StartDraw( );

	m_mode -> Draw( );

	m_fade -> Draw( );

	m_renderer -> EndDraw( );
}


/*******************************************************************************
* 関数名：GetRenderer
* 引数：なし
* 戻り値：m_renderer
* 説明：m_rendererのアドレスを返す
*******************************************************************************/

CRenderer *CManager::GetRenderer(void) {
	return m_renderer;
}


/*******************************************************************************
* 関数名：GetInput
* 引数：なし
* 戻り値：m_input
* 説明：m_inputのアドレスを返す
*******************************************************************************/

CInput *CManager::GetInput(void) {
	return m_input;
}


/*******************************************************************************
* 関数名：GetMode
* 引数：なし
* 戻り値：
* 説明：
*******************************************************************************/

CMode *CManager::GetMode(void) {
	return m_mode;
}

/*******************************************************************************
* 関数名：GetFade
* 引数：なし
* 戻り値：
* 説明：
*******************************************************************************/

CFade *CManager::GetFade(void) {
	return m_fade;
}


/*******************************************************************************
* 関数名：GetCamera
* 引数：なし
* 戻り値：
* 説明：
*******************************************************************************/

CCamera *CManager::GetCamera(void) {
	return m_camera;
}


/*******************************************************************************
* 関数名：SetMode
* 引数：
* 戻り値：なし
* 説明：
*******************************************************************************/

void CManager::SetMode(CMode *mode) {
	if(m_mode != NULL) {
		m_mode -> Uninit( );
		delete m_mode;
	}

	m_mode = mode;
	m_mode -> Init( );
}

