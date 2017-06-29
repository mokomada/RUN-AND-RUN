/*******************************************************************************
* タイトル チュートリアル画像表示
* ファイル名 tutorial.cpp
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/07/19
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "mode.h"
#include "fade.h"
#include "tutorial.h"
#include "game.h"
#include "result.h"
#include "title.h"
#include "scene.h"
#include "input.h"
#include "sound.h"


/*******************************************************************************
* 関数名：InitTutorial
* 引数：なし
* 戻り値：なし
* 説明：ゲーム画面の初期化
*******************************************************************************/

void CTutorial::Init(void) {
	// 変数宣言
	VERTEX_2D *pVtx;

	// 変数の初期化
	m_timeCount = 0;
	m_alphaChange = 255.0f / (float)TUTORIAL_PRESSENTER_BLINK_TIME;
	m_PEAlpha = D3DXCOLOR(255, 255, 255, 255);

	// 変数の取得
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	m_pTexture = NULL;
	m_pVtxBuff = NULL;

	// テクスチャ設定
	D3DXCreateTextureFromFile(device, TUTORIAL_TEXTURE_NAME, &m_pTexture);

	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * TUTORIAL_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// 頂点情報へのポインタを取得
	m_pVtxBuff -> Lock(0, 0, (void**)&pVtx, 0);

	// 座標設定
	pVtx[0].pos = D3DXVECTOR3(0, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	for(int i = 0; i < 4; i++) {
		pVtx[i].rhw = 1.0f;

		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// テクスチャUV座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff -> Unlock( );

	// テクスチャ設定
	D3DXCreateTextureFromFile(device, TUTORIAL_PRESSENTER_TEXTURE_NAME, &m_pPETexture);

	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * TUTORIAL_PRESSENTER_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pPEVtxBuff, NULL);

	// 頂点情報へのポインタを取得
	m_pPEVtxBuff -> Lock(0, 0, (void**)&pVtx, 0);

	// 座標設定
	pVtx[0].pos = D3DXVECTOR3( (SCREEN_WIDTH - TUTORIAL_PRESSENTER_WIDTH) * 0.5f, TUTORIAL_PRESSENTER_POSY, 0.0f);
	pVtx[1].pos = D3DXVECTOR3( (SCREEN_WIDTH - TUTORIAL_PRESSENTER_WIDTH) * 0.5f + TUTORIAL_PRESSENTER_WIDTH, TUTORIAL_PRESSENTER_POSY, 0.0f);
	pVtx[2].pos = D3DXVECTOR3( (SCREEN_WIDTH - TUTORIAL_PRESSENTER_WIDTH) * 0.5f, TUTORIAL_PRESSENTER_POSY + TUTORIAL_PRESSENTER_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3( (SCREEN_WIDTH - TUTORIAL_PRESSENTER_WIDTH) * 0.5f + TUTORIAL_PRESSENTER_WIDTH, TUTORIAL_PRESSENTER_POSY + TUTORIAL_PRESSENTER_HEIGHT, 0.0f);

	for(int i = 0; i < 4; i++) {
		pVtx[i].rhw = 1.0f;

		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}

	// テクスチャUV座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pPEVtxBuff -> Unlock( );
}


/*******************************************************************************
* 関数名：UninitTutorial
* 引数：なし
* 戻り値：なし
* 説明：ゲーム画面の終了処理
*******************************************************************************/

void CTutorial::Uninit(void) {
	if(m_pTexture != NULL) {
		m_pTexture -> Release( );
		m_pTexture = NULL;
	}

	if(m_pVtxBuff != NULL) {
		m_pVtxBuff -> Release( );
		m_pVtxBuff = NULL;
	}

		if(m_pPETexture != NULL) {
		m_pPETexture -> Release( );
		m_pPETexture = NULL;
	}

	if(m_pPEVtxBuff != NULL) {
		m_pPEVtxBuff -> Release( );
		m_pPEVtxBuff = NULL;
	}
}


/*******************************************************************************
* 関数名：UpdateTutorial
* 引数：なし
* 戻り値：なし
* 説明：ゲーム画面の更新
*******************************************************************************/

void CTutorial::Update(void) {
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

	if(fade == FADE_NONE) {
		// 点滅処理
		if(m_timeCount % (TUTORIAL_PRESSENTER_BLINK_TIME * 4) >= (TUTORIAL_PRESSENTER_BLINK_TIME * 3) ) {
			m_PEAlpha.a += m_alphaChange;;
			if(m_PEAlpha.a > 255) m_PEAlpha.a = 255;
		}
		else if(m_timeCount % (TUTORIAL_PRESSENTER_BLINK_TIME * 4) >= (TUTORIAL_PRESSENTER_BLINK_TIME * 2) ) {
			m_PEAlpha.a -= m_alphaChange;
			if(m_PEAlpha.a < 0) m_PEAlpha.a = 0;
		}

		m_timeCount++;

		// エンターorスペースで場面遷移。1週目のみ3秒待機（連打でスキップしない用）。2週目以降はフェードが終わった瞬間から入力受付
		if( (input -> GetKeyboardTrigger(DIK_RETURN) || input -> GetKeyboardTrigger(DIK_SPACE) ) && (!CManager::m_firstTimeFlag || (CManager::m_firstTimeFlag && m_timeCount >= 180) ) ) {
			CFade::SetFade(FADE_OUT, new CGame);
			CManager::m_firstTimeFlag = false;
		}
	}
	else if(fade == FADE_OUT) {
		if(m_timeCount % 10 >= 5) {
			m_PEAlpha.a += m_alphaChange;;
			if(m_PEAlpha.a > 255) m_PEAlpha.a = 255;
		}
		else{
			m_PEAlpha.a -= m_alphaChange;
			if(m_PEAlpha.a < 0) m_PEAlpha.a = 0;
		}

		m_timeCount++;
	}

	m_pPEVtxBuff -> Lock(0, 0, (void**)&pVtx, 0);

	for(int i = 0; i < 4; i++) {
		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, (int)m_PEAlpha.a);
	}

	m_pPEVtxBuff -> Unlock( );
}


/*******************************************************************************
* 関数名：DrawTutorial
* 引数：なし
* 戻り値：なし
* 説明：ゲーム画面の描画
*******************************************************************************/

void CTutorial::Draw(void) {
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D) );

	// テクスチャの設定
	device -> SetTexture(0, m_pTexture);

	// 2Dポリゴンの描画
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		FADE_TRIANGLE_NUM);

	// PRESS ENTER表示
	// ブレンド設定
	device ->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);// 引数の成分を乗算する
	device ->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_pPEVtxBuff, 0, sizeof(VERTEX_2D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	device -> SetTexture(0, m_pPETexture);

	// 2Dポリゴンの描画
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		TITLE_TRIANGLE_NUM);

	// ブレンド設定を戻す
	device ->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1 );// 引数の成分を乗算する
	device ->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_CURRENT);
}
