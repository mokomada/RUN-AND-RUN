/*******************************************************************************
* タイトル タイトル画面描画処理
* ファイル名 title.cpp
* 作成者 AT-12C-245 秋間雄太
* 作成日 2015/06/24
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "mode.h"
#include "fade.h"
#include "title.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "input.h"
#include "scene.h"
#include "sound.h"


/*******************************************************************************
* グローバル変数
*******************************************************************************/


/*******************************************************************************
* 関数名：InitTitle
* 引数：なし
* 戻り値：なし
* 説明：タイトル画面設定の初期化
*******************************************************************************/

void CTitle::Init(void) {
	// 変数宣言
	int i; // for文ループ用
	VERTEX_2D *pVtx;

	// 変数の初期化
	m_title_time_count = 0;
	m_alpha_change = 255.0f / (float)TITLE_LOGO_BLINK_TIME;
	m_title_logo_alpha = D3DXCOLOR(255, 255, 255, 255);
	m_pTextureTitle = NULL; // タイトル画面用テクスチャへのポインタ
	m_pTextureTitleLogo = NULL; // ロゴ用テクスチャへのポインタ
	m_pVtxBuffTitle = NULL; // タイトル画面の頂点バッファへのポインタ
	m_pVtxBuffTitleLogo = NULL; // ロゴの頂点バッファへのポインタ

	// 変数の取得
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	// タイトル画面テクスチャ設定
	D3DXCreateTextureFromFile(device, TITLE_TEXTURE_NAME, &m_pTextureTitle);

	// 頂点バッファの生成
	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * TITLE_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffTitle, NULL);

	// 頂点情報へのポインタを取得
	m_pVtxBuffTitle -> Lock(0, 0, (void**)&pVtx, 0);

	// 座標設定
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);


	for(i = 0; i < TITLE_VERTEX_NUM; i++) {
		pVtx[i].rhw = 1.0f;

		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// テクスチャUV座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuffTitle -> Unlock( );

	// ロゴテクスチャ設定
	D3DXCreateTextureFromFile(device, TITLE_LOGO_TEXTURE_NAME, &m_pTextureTitleLogo);

	// 頂点バッファの生成
	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * TITLE_LOGO_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffTitleLogo, NULL);

	m_pVtxBuffTitleLogo -> Lock(0, 0, (void**)&pVtx, 0);

	// 座標設定
	pVtx[0].pos = D3DXVECTOR3( (SCREEN_WIDTH - TITLE_LOGO_WIDTH) * 0.5f, TITLE_LOGO_POSY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3( (SCREEN_WIDTH - TITLE_LOGO_WIDTH) * 0.5f + TITLE_LOGO_WIDTH, TITLE_LOGO_POSY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3( (SCREEN_WIDTH - TITLE_LOGO_WIDTH) * 0.5f, TITLE_LOGO_POSY + TITLE_LOGO_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3( (SCREEN_WIDTH - TITLE_LOGO_WIDTH) * 0.5f + TITLE_LOGO_WIDTH, TITLE_LOGO_POSY + TITLE_LOGO_HEIGHT, 0.0f);


	for(i = 0; i < TITLE_VERTEX_NUM; i++) {
		pVtx[i].rhw = 1.0f;

		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// テクスチャUV座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuffTitleLogo -> Unlock( );

	CSound::StopSound(SOUND_LABEL_BGM000);
	CSound::PlaySound(SOUND_LABEL_BGM001);
}


/*******************************************************************************
* 関数名：UninitTitle
* 引数：なし
* 戻り値：なし
* 説明：タイトル画面設定の終了処理
*******************************************************************************/

void CTitle::Uninit(void) {
	if(m_pTextureTitle != NULL) {
		m_pTextureTitle -> Release( ); // テクスチャの開放
		m_pTextureTitle = NULL;
	}

	if(m_pVtxBuffTitle != NULL) {
		m_pVtxBuffTitle -> Release( ); // 頂点バッファの開放
		m_pVtxBuffTitle = NULL;
	}

	if(m_pTextureTitleLogo != NULL) {
		m_pTextureTitleLogo -> Release( );
		m_pTextureTitleLogo = NULL;
	}

	if(m_pVtxBuffTitleLogo != NULL) {
		m_pVtxBuffTitleLogo -> Release( );
		m_pVtxBuffTitleLogo = NULL;
	}
}


/*******************************************************************************
* 関数名：UpdataTitle
* 引数：なし
* 戻り値：なし
* 説明：タイトル画面の情報更新
*******************************************************************************/

void CTitle::Update(void) {
	// 変数の宣言
	VERTEX_2D *pVtx;

	// 変数の取得
	CInput *input;
	input = CManager::GetInput( );

	CFade *getFade;
	getFade = CManager::GetFade( );
	FADE fade = getFade -> GetFade( );

	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	// フェードイン中でなければPRESS ENTERの点滅処理とキー入力による画面遷移判定
	if(fade == FADE_NONE) {
		// 点滅処理
		if(m_title_time_count % (TITLE_LOGO_BLINK_TIME * 4) >= (TITLE_LOGO_BLINK_TIME * 3) ) {
			m_title_logo_alpha.a += m_alpha_change;;
			if(m_title_logo_alpha.a > 255) m_title_logo_alpha.a = 255;
		}
		else if(m_title_time_count % (TITLE_LOGO_BLINK_TIME * 4) >= (TITLE_LOGO_BLINK_TIME * 2) ) {
			m_title_logo_alpha.a -= m_alpha_change;
			if(m_title_logo_alpha.a < 0) m_title_logo_alpha.a = 0;
		}

		m_title_time_count++;

		// 特定のキーが押されたらチュートリアルに以降
		if(input -> GetKeyboardTrigger(DIK_RETURN) || input -> GetKeyboardTrigger(DIK_SPACE) ) {
			CFade::SetFade(FADE_OUT, new CTutorial);
			m_alpha_change = 255.0f / 5.0f;
			m_title_logo_alpha.a = 255;
			m_title_time_count = 0;
		}
	}
	else if(fade == FADE_OUT) {
		if(m_title_time_count % 10 >= 5) {
			m_title_logo_alpha.a += m_alpha_change;;
			if(m_title_logo_alpha.a > 255) m_title_logo_alpha.a = 255;
		}
		else{
			m_title_logo_alpha.a -= m_alpha_change;
			if(m_title_logo_alpha.a < 0) m_title_logo_alpha.a = 0;
		}

		m_title_time_count++;
	}

	m_pVtxBuffTitleLogo -> Lock(0, 0, (void**)&pVtx, 0);

	for(int i = 0; i < TITLE_VERTEX_NUM; i++) {
//		pVtx[i].col = D3DCOLOR_RGBA( (int)m_title_logo_alpha.a, (int)m_title_logo_alpha.a, (int)m_title_logo_alpha.a, 255);
		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, (int)m_title_logo_alpha.a);
	}

	m_pVtxBuffTitleLogo -> Unlock( );
}


/*******************************************************************************
* 関数名：DrawTitle
* 引数：なし
* 戻り値：なし
* 説明：タイトル画面の描画処理
*******************************************************************************/

void CTitle::Draw(void) {
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	device -> SetStreamSource(0, m_pVtxBuffTitle, 0, sizeof(VERTEX_2D) );

	device -> SetTexture(0, m_pTextureTitle);

	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		TITLE_TRIANGLE_NUM);

	// ロゴ（と言う名のPRESS ENTER）の描画
	// ブレンド設定
	device ->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);// 引数の成分を乗算する
	device ->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_pVtxBuffTitleLogo, 0, sizeof(VERTEX_2D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	device -> SetTexture(0, m_pTextureTitleLogo);

	// 2Dポリゴンの描画
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		TITLE_TRIANGLE_NUM);

	// ブレンド設定を戻す
	device ->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );// 引数の成分を乗算する
	device ->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT);
}
