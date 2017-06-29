/*******************************************************************************
* タイトル リザルト画面描画処理
* ファイル名 result.cpp
* 作成者 AT-13A-284 秋間雄太
* 作成日 2016/07/19
*******************************************************************************/

/*******************************************************************************
* インクルードファイル
*******************************************************************************/

#include "main.h"
#include "manager.h"
#include "renderer.h"
#include "scene.h"
#include "mode.h"
#include "result.h"
#include "input.h"
#include "fade.h"
#include "score.h"
#include "scene.h"
#include "game.h"
#include "title.h"


int CResult::m_result_score[RESULT_SCORE_NUM] = {100, 200, 300, 400, 500};

/*******************************************************************************
* グローバル変数
*******************************************************************************/


/*******************************************************************************
* 関数名：InitResult
* 引数：なし
* 戻り値：なし
* 説明：リザルト画面設定の初期化
*******************************************************************************/

void CResult::Init(void) {
	/* 変数宣言 */
	int i, j; // for文ループ用
	int nNumDigit; // スコア用変数
	int current_score;
	VERTEX_2D *pVtx;

	/* スコア桁数設定 */
	nNumDigit = 1;
	for(i = 0; i < NUMBER_NUM; i++) {
		nNumDigit *= 10;
	}

	/* グローバル変数の初期化 */
	result_wait_time = RESULT_WAIT_TIME;

	/* 変数の取得 */
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	for(int n = 0; n < RESULT_SCORE_NUM; n++) {
		m_pTextureResultScore[n] = NULL;
		m_pVtxBuffResultScore[n] = NULL;
	}

	current_score = CScore::GetScore( );
	CScore::AddScore(-current_score); // スコアが残っていると複数回更新されてしまう可能性があるので、スコアの中身を0に

	// ランキング更新
	for(i = 0; i < RESULT_SCORE_NUM; i++) {
		if(current_score < m_result_score[i]) {
			for(j = RESULT_SCORE_NUM - 1; j > i; j--) {
				m_result_score[j] = m_result_score[j - 1];
			}
			m_result_score[i] = current_score;
			break;
		}
	}

	for(i = 0; i < RESULT_SCORE_NUM; i++) {
		// スコアテクスチャ設定
		D3DXCreateTextureFromFile(device, SCORE_TEXTURE_NAME, &m_pTextureResultScore[i]);

		// 頂点バッファの生成
		device -> CreateVertexBuffer(sizeof(VERTEX_2D) * SCORE_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffResultScore[i], NULL);

		// スコア桁数設定
		nNumDigit = 1;
		for(j = 0; j < NUMBER_NUM; j++) {
			nNumDigit *= 10;
		}

		// 頂点情報へのポインタを取得
		m_pVtxBuffResultScore[i] -> Lock(0, 0, (void**)&pVtx, 0);

		// 座標設定
		for(j = 0; j < NUMBER_NUM; j++) {
			pVtx[(j * 4) + 0].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) * 0.5f + j * BOX_WIDTH, (float) (BOX_HEIGHT + RESULT_SCORE_SPACE) * (i + 2) - SCREEN_HEIGHT, 0.0f);
			pVtx[(j * 4) + 1].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) * 0.5f + (j + 1) * BOX_WIDTH, (float) (BOX_HEIGHT + RESULT_SCORE_SPACE) * (i + 2) - SCREEN_HEIGHT, 0.0f);
			pVtx[(j * 4) + 2].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) * 0.5f + j * BOX_WIDTH, (float) BOX_HEIGHT + (BOX_HEIGHT + RESULT_SCORE_SPACE) * (i + 2) - SCREEN_HEIGHT, 0.0f);
			pVtx[(j * 4) + 3].pos = D3DXVECTOR3( (float) (SCREEN_WIDTH - NUMBER_NUM * BOX_WIDTH) * 0.5f + (j + 1) * BOX_WIDTH, (float) BOX_HEIGHT + (BOX_HEIGHT + RESULT_SCORE_SPACE) * (i + 2) - SCREEN_HEIGHT, 0.0f);
		}

		for(j = 0; j < SCORE_VERTEX_NUM; j++) {
			pVtx[j].rhw = 1.0f;

			pVtx[j].col = D3DCOLOR_RGBA(255, 255, 128, 255);
		}

		// テクスチャUV座標設定
		for(j = 0; j < NUMBER_NUM; j++) {
			pVtx[(j * 4) + 0].tex = D3DXVECTOR2( (float) ( ( (m_result_score[i] % nNumDigit) / (nNumDigit / 10) ) * TEX_LENGTH), 0.0f);
			pVtx[(j * 4) + 1].tex = D3DXVECTOR2( (float) ( ( (m_result_score[i] % nNumDigit) / (nNumDigit / 10) + 1) * TEX_LENGTH), 0.0f);
			pVtx[(j * 4) + 2].tex = D3DXVECTOR2( (float) ( ( (m_result_score[i] % nNumDigit) / (nNumDigit / 10) ) * TEX_LENGTH), 1.0f);
			pVtx[(j * 4) + 3].tex = D3DXVECTOR2( (float) ( ( (m_result_score[i] % nNumDigit) / (nNumDigit / 10) + 1) * TEX_LENGTH), 1.0f);
			nNumDigit /= 10;
		}

		m_pVtxBuffResultScore[i] -> Unlock( );
	}

// ロゴテクスチャ設定
	D3DXCreateTextureFromFile(device, RESULT_LOGO_TEXTURE_NAME, &m_pTextureResultLogo);

	// 頂点バッファの生成
	device -> CreateVertexBuffer(sizeof(VERTEX_2D) * RESULT_LOGO_VERTEX_NUM, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuffResultLogo, NULL);

	m_pVtxBuffResultLogo -> Lock(0, 0, (void**)&pVtx, 0);

	// 座標設定
	pVtx[0].pos = D3DXVECTOR3( (SCREEN_WIDTH - RESULT_LOGO_WIDTH) * 0.5f, 0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3( (SCREEN_WIDTH - RESULT_LOGO_WIDTH) * 0.5f + RESULT_LOGO_WIDTH, 0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3( (SCREEN_WIDTH - RESULT_LOGO_WIDTH) * 0.5f, RESULT_LOGO_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3( (SCREEN_WIDTH - RESULT_LOGO_WIDTH) * 0.5f + RESULT_LOGO_WIDTH, RESULT_LOGO_HEIGHT, 0.0f);


	for(i = 0; i < RESULT_VERTEX_NUM; i++) {
		pVtx[i].rhw = 1.0f;

		pVtx[i].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	}
	
	// テクスチャUV座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuffResultLogo -> Unlock( );
}


/*******************************************************************************
* 関数名：UninitResult
* 引数：なし
* 戻り値：なし
* 説明：リザルト画面設定の終了処理
*******************************************************************************/

void CResult::Uninit(void) {
	/* 変数の宣言 */
	int i;

	for(i = 0; i < RESULT_SCORE_NUM; i++) {
		if(m_pTextureResultScore[i] != NULL) {
			m_pTextureResultScore[i] -> Release( );
			m_pTextureResultScore[i] = NULL;
		}

		if(m_pVtxBuffResultScore[i] != NULL) {
			m_pVtxBuffResultScore[i] -> Release( );
			m_pVtxBuffResultScore[i] = NULL;
		}
	}
	
	if(m_pTextureResultLogo != NULL) {
		m_pTextureResultLogo -> Release( );
		m_pTextureResultLogo = NULL;
	}

	if(m_pVtxBuffResultLogo != NULL) {
		m_pVtxBuffResultLogo -> Release( );
		m_pVtxBuffResultLogo = NULL;
	}
}


/*******************************************************************************
* 関数名：UpdataResult
* 引数：なし
* 戻り値：なし
* 説明：リザルト画面の情報更新
*******************************************************************************/

void CResult::Update(void) {
	/* 変数の宣言 */
	int i, j, k;
	VERTEX_2D *pVtx;

	/* 変数の取得 */
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	CFade *getFade;
	getFade = CManager::GetFade( );
	FADE fade = getFade -> GetFade( );

	CInput *input;
	input = CManager::GetInput( );

	if(fade == FADE_NONE) {
		if(input -> GetKeyboardTrigger(DIK_RETURN) || input -> GetKeyboardTrigger(DIK_SPACE) || result_wait_time == 0) {
			if(result_wait_time > RESULT_STOP_MOVE_TIME) {
				while(result_wait_time > RESULT_STOP_MOVE_TIME) {
					for(i = 0; i < RESULT_SCORE_NUM; i++) {
						// 頂点情報へのポインタを取得
						m_pVtxBuffResultScore[i] -> Lock(0, 0, (void**)&pVtx, 0);
						// 座標設定
						for(j = 0; j < NUMBER_NUM; j++) {
							for(k = 0; k < 4; k++) {
							pVtx[(j * 4) + k].pos.y += SCREEN_HEIGHT / (RESULT_WAIT_TIME - RESULT_STOP_MOVE_TIME);
							}
						}
						m_pVtxBuffResultScore[i] -> Unlock( );
					}
					result_wait_time--;
				}
			}
			else {
				CFade::SetFade(FADE_OUT, new CTitle);
			}
		}

		if(result_wait_time > RESULT_STOP_MOVE_TIME) {
			for(i = 0; i < RESULT_SCORE_NUM; i++) {
				// 頂点情報へのポインタを取得
				m_pVtxBuffResultScore[i] -> Lock(0, 0, (void**)&pVtx, 0);
				// 座標設定
				for(j = 0; j < NUMBER_NUM; j++) {
					for(k = 0; k < 4; k++) {
					pVtx[(j * 4) + k].pos.y += SCREEN_HEIGHT / (RESULT_WAIT_TIME - RESULT_STOP_MOVE_TIME);
					}
				}
				m_pVtxBuffResultScore[i] -> Unlock( );
			}
		}
		result_wait_time--;
	}
}


/*******************************************************************************
* 関数名：DrawResult
* 引数：なし
* 戻り値：なし
* 説明：リザルト画面の描画処理
*******************************************************************************/

void CResult::Draw(void) {
	/* 変数の宣言 */
	int i;
	CRenderer *renderer;
	renderer = CManager::GetRenderer( );
	LPDIRECT3DDEVICE9 device = renderer -> GetDevice( );

	// ロゴの描画
	// 頂点バッファをデータストリームにバインド
	device -> SetStreamSource(0, m_pVtxBuffResultLogo, 0, sizeof(VERTEX_2D) );

	// 頂点フォーマットの設定
	device -> SetFVF(FVF_VERTEX_2D);

	// テクスチャの設定
	device -> SetTexture(0, m_pTextureResultLogo);

	// ライト設定オフ
	device -> SetRenderState(D3DRS_LIGHTING, FALSE);

	// 2Dポリゴンの描画
	device -> DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0,
		TITLE_TRIANGLE_NUM);

	// ライト設定オン
	device -> SetRenderState(D3DRS_LIGHTING, TRUE);

	for(i = 0; i < RESULT_SCORE_NUM; i++) {
		device -> SetStreamSource(0, m_pVtxBuffResultScore[i], 0, sizeof(VERTEX_2D) );

		device -> SetTexture(0, m_pTextureResultScore[i]);

		device -> DrawPrimitive(
			D3DPT_TRIANGLESTRIP,
			0,
			SCORE_TRIANGLE_NUM);
	}
}
